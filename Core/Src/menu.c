/*
 * menu.c
 *
 *  Created on: 25 July 2023
 *      Author: Akash Virendra
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "common.h"
#include "flash_if.h"
#include "menu.h"
#include "ymodem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction JumpToApplication;
uint32_t JumpAddress;
uint32_t FlashProtection = 0;
uint8_t aFileName[FILE_NAME_LENGTH];

/* Private function prototypes -----------------------------------------------*/
void SerialDownload(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
void SerialDownload(void)
{
  uint8_t number[11] = {0};
  uint32_t size = 0;
  COM_StatusTypeDef result;

  Serial_PutString((uint8_t *)"Waiting for the file to be sent ... (press 'a' to abort)\n\r");
  result = Ymodem_Receive( &size );
  if ( result == COM_OK )
  {
    Serial_PutString((uint8_t *)"\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
    Serial_PutString((uint8_t *)aFileName);
    Int2Str(number, size);
    Serial_PutString((uint8_t *)"\n\r Size: ");
    Serial_PutString((uint8_t *)number);
    Serial_PutString((uint8_t *)" Bytes\r\n");
    Serial_PutString((uint8_t *)"-------------------\n");
    // Jump To Applicaion...
    API_JumpApplication();
  }
  else if ( result == COM_LIMIT )
  {
    Serial_PutString((uint8_t *)"\n\n\rThe image size is higher than the allowed space memory!\n\r");
  }
  else if ( result == COM_DATA )
  {
    Serial_PutString((uint8_t *)"\n\n\rVerification failed!\n\r");
  }
  else if ( result == COM_ABORT )
  {
    Serial_PutString((uint8_t *)"\r\n\nAborted by user.\n\r");
    Serial_PutString((uint8_t *)"Try jumping to the applicaiton...\n\r");
    HAL_Delay(700);
    // Jump To Applicaion...
    API_JumpApplication();
    Serial_PutString((uint8_t *)"\r\n\nJump failed!!!...\n\r");
  }
  else
  {
    Serial_PutString((uint8_t *)"\n\rFailed to receive the file!\n\r");
  }
}

/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{
  char *major = NULL, *minor = NULL, *commit_cnt = NULL, *git_hash = NULL;
  char git_version[] = GIT_VERSION;
  uint8_t version[80] = {0x00};

  major = strtok(git_version, "-");

  minor = strtok(NULL, "-");

  commit_cnt = strtok(NULL, "-");

  git_hash = strtok(NULL, "-g");

  if ( major == NULL || minor == NULL || commit_cnt == NULL || git_hash == NULL )
  {
	sprintf( (char *) version, "\r\n= ST In-Application Programming Application  ( Version 1.0.00000000 ) =");
  }
  else
  {
    sprintf( (char *) version, "\r\n= ST In-Application Programming Application  ( Version %s.%s.%s ) =", major, minor, git_hash);
  }
  Serial_PutString((uint8_t *)"\r\n======================================================================");
  Serial_PutString((uint8_t *)"\r\n=              (C) COPYRIGHT 2023 STMicroelectronics                 =");
  Serial_PutString((uint8_t *)"\r\n=                                                                    =");
  Serial_PutString((uint8_t *) version);
  Serial_PutString((uint8_t *)"\r\n=                                                                    =");
  Serial_PutString((uint8_t *)"\r\n=                                            By STM Application Team =");
  Serial_PutString((uint8_t *)"\r\n=                                                                    =");
  Serial_PutString((uint8_t *)"\r\n=                                            Modify by Howard Zhan   =");
  Serial_PutString((uint8_t *)"\r\n======================================================================");
  Serial_PutString((uint8_t *)"\r\n\r\n");

  while (1)
  {
    /* Clean the input path */
    __HAL_UART_FLUSH_DRREGISTER(&UartHandle);
    __HAL_UART_CLEAR_IT(&UartHandle, UART_CLEAR_OREF);

    // Software update...s
    /* Download user application in the Flash */
    SerialDownload();
  }
}

