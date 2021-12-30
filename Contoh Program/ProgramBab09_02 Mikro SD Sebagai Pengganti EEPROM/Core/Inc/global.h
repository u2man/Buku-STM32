/*
 * global.h
 *
 *  Created on: Aug 13, 2019
 *      Author: Kang Usman
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "main.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "led.h"
#include "pclink.h"


extern uint8_t UARTSendFinish;
extern uint8_t UartRxBuffer;

uint16_t parsing_delimiter_string(uint8_t *storebuffer, uint8_t *databuffer, uint8_t delimitter, uint16_t startpos, uint16_t len);
uint8_t CalculateCRC8(uint8_t* buffer, int length);

#endif /* GLOBAL_H_ */
