/*
 * def_variable.h
 *
 *  Created on: 2024年9月22日
 *      Author: m2640
 */
#ifndef INC_DEF_VARIABLE_H_
#define INC_DEF_VARIABLE_H_
#include "main.h"

#define APP_NODE_NAME				"B-G431B-ESC"
#define APP_DESCRIPTOR_SIG			"g431cbu"
/*	The string must be less than 8-bytes	*/
static_assert( sizeof(APP_DESCRIPTOR_SIG) <= 8, "Error length" );

/* Define the address from where user application will be loaded.
   Note: this area is reserved for the IAP code                  */
#define APPLICATION_ADDRESS     	(uint32_t) 0x08005000

/* Notable Flash addresses */
#define FLASH_START		            (uint32_t) 0x08000000
#define FLASH_END_ADDRESS           (uint32_t) 0x08020000
#define USER_FLASH_END_ADDRESS      FLASH_END_ADDRESS
/* Define the user application size */
#define USER_FLASH_SIZE             (uint32_t) ( USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS )

// keep back 32 bytes at the front of flash. This is long enough to allow for aligned
// access on STM32
#define RESERVE_LEAD_WORDS 			8

#define IAP_ENTRY_CODE				0x5866
#define IAP_NOT_ENTRY_CODE			0xA799

#define UartHandle					huart2
#define CrcHandle					hcrc

typedef struct __attribute__((packed)) IAP_DATA
{
  uint16_t	EntryIAP;
  uint16_t	crc16;  // where use complement
} IAP_DATA_t;

extern CRC_HandleTypeDef			hcrc;
extern UART_HandleTypeDef 			huart2;
extern volatile IAP_DATA_t			iap_data __attribute__((section(".noinit")));

#endif /* INC_DEF_VARIABLE_H_ */
