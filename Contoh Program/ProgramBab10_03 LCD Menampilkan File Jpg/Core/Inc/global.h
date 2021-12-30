/*
 * global.h
 *
 *  Created on: Nov 5, 2019
 *      Author: Kang Usman
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include "fatfs.h"
#include "libjpeg.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ssd1289.h"
#include "lcd.h"
#include "fonts.h"

typedef struct RGB
{
  uint8_t B;
  uint8_t G;
  uint8_t R;
}RGB_typedef;

#define IMAGE_HEIGHT 240
#define IMAGE_WIDTH  320

uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength);

#endif /* INC_GLOBAL_H_ */
