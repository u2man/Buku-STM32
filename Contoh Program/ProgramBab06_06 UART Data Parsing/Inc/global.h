/*
 * global.h
 *
 *  Created on: Jun 15, 2019
 *      Author: Kang Usman
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "main.h"
#include "stm32f2xx_hal.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "led.h"
#include "pclink.h"

uint16_t parsing_delimiter_string(uint8_t *storebuffer, uint8_t *databuffer, uint8_t delimitter, uint16_t startpos, uint16_t len);



#endif /* GLOBAL_H_ */
