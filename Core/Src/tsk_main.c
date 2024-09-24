/*
 * tsk_main.c
 *
 *  Created on: 2024年9月22日
 *      Author: m2640
 */
#include "tsk_main.h"

#if !defined(DEBUG)
void Disable_SWD_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  return;
}
#endif

void tsk_main(void *argument)
{
  bool try_boot = false;

#if !defined(DEBUG)
  Disable_SWD_GPIO();
#endif

  uint16_t crc16 = (uint16_t) ~(iap_data.EntryIAP);
  if ( iap_data.EntryIAP == IAP_ENTRY_CODE && crc16 == IAP_NOT_ENTRY_CODE )
  {
    try_boot = true;
  }

  if ( try_boot != true )
  {
    API_JumpApplication();
  }

  /*
   * If the flash does not have firmware,
   * iap data assigns parameters to tell
   * the application that it has been updated.
   */
  iap_data.EntryIAP = IAP_ENTRY_CODE;
  iap_data.crc16	= IAP_NOT_ENTRY_CODE;

  /* Initialise Flash */
  FLASH_If_Init();
  /* Display main menu */
  Main_Menu();

  return;
}

/*
  check firmware CRC to see if it matches
 */
bool check_firmware(void)
{
  app_descriptor _ad;

  const uint8_t sig[8]	= APP_DESCRIPTOR_SIG;
  memcpy(&_ad, (uint32_t *) APP_DESCRIPTOR_ADDR, sizeof(app_descriptor));
  if( strncmp((char *) &_ad.sig[0], (char *) &sig[0], 8) != 0 )
  {
	// no application signature
	return false;
  }
  if ( _ad.image_size != USER_FLASH_SIZE )
  {
    return false;
  }
  return true;
}

/**
*	API_JumpApplication
**/
void API_JumpApplication(void)
{
	const __IO uint32_t *app_base = (const uint32_t *) ( APPLICATION_ADDRESS );
	// The value in app_base[0] must be smaller than the internal ram size.
	// STM32G431CB RAM = 32k = 0x8000, [Note] The top of the stack in STM32CubeIDE/Keil C is different...
	if ( ( ( app_base[0] & 0x2FFF0000 ) == 0x20000000 ) && check_firmware() )
	{
	  /*
       * We hold back the programming of the lead words until the upload
       * is marked complete by the host. So if they are not 0xffffffff,
       * we should try booting it.
       */
      for ( uint8_t i = 0; i < RESERVE_LEAD_WORDS; i++ )
      {
	    if ( app_base[i] == 0xffffffff )
	    {
	      goto exit;
	    }
      }

	  /*
       * The second word of the app is the entrypoint; it must point within the
       * flash area (or we have a bad flash).
       */
      if ( app_base[1] < APPLICATION_ADDRESS )
	  {
        goto exit;
      }

	  if ( app_base[1] >= ( APPLICATION_ADDRESS + USER_FLASH_SIZE ) )
	  {
        goto exit;
      }

	  /* Jump to user application */
	  SysTick->CTRL	= 0;
	  SysTick->LOAD	= 0;
	  SysTick->VAL	= 0;
	  __disable_irq();

	  /* Disable TRC */
	  CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;
	  /* Disable clock cycle counter */
	  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;

	  /* Peripheral Disable... */
	  HAL_UART_MspDeInit(&huart2);
	  HAL_RCC_DeInit();
	  HAL_DeInit();

	  for ( uint8_t i = 0; i < 8; i++ )
	  {
	    /*	Interrupt Clear Enable Register	*/
	    NVIC->ICER[i] = 0xFFFFFFFF;
	    /*	Interrupt Clear Pending Register	*/
	    NVIC->ICPR[i] = 0xFFFFFFFF;
	  }

	  void (*boot)(void) __attribute__((noreturn)) = (void *)(*(uint32_t*)( APPLICATION_ADDRESS + 4 ) );
	  /* Initialize user application's Stack Pointer */
	  __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
	  boot();
	}
exit:
	return;
}

