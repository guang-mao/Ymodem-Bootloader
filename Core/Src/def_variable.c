/*
 * def_variable.c
 *
 *  Created on: 2024年9月22日
 *      Author: m2640
 */
#include "def_variable.h"

volatile IAP_DATA_t iap_data __attribute__((section(".noinit")));
