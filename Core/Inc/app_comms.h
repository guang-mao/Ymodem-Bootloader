/*
 * app_comms.h
 *
 *  Created on: 2024年9月22日
 *      Author: m2640
 */
#ifndef INC_APP_COMMS_H_
#define INC_APP_COMMS_H_
#include "main.h"

#define HAL_RAM0_START							0x20000000

#define APP_DESCRIPTOR_ADDR						0x0801FFF4

#define APP_DESCRIPTOR_SIZE						0x0000000c

#define APP_BOOTLOADER_COMMS_MAGIC				0xc544ad9a

#define APP_DESCRIPTOR_TOTAL_LENGTH 			12

struct app_bootloader_comms
{
	uint32_t magic;
	uint32_t reserved[4];
	uint8_t server_node_id;
	uint8_t my_node_id;
	uint8_t path[201];
};

/*
 the app_descriptor stored in flash in the main firmware and is used
 by the bootloader to confirm that the firmware is not corrupt and is
 suitable for this board. The build dependent values in this structure
 are filled in by set_app_descriptor() in the waf build
 */
typedef struct __attribute__((packed))
{
	uint8_t		sig[8];
	uint32_t	image_size;
} app_descriptor;
static_assert(sizeof(app_descriptor) == APP_DESCRIPTOR_TOTAL_LENGTH, "app_descriptor incorrect length");

#endif /* INC_APP_COMMS_H_ */
