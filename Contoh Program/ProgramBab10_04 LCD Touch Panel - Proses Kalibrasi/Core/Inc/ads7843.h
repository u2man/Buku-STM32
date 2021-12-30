/*
 * ads7843.h
 *
 *  Created on: Nov 21, 2019
 *      Author: Kang Usman
 */

#ifndef INC_ADS7843_H_
#define INC_ADS7843_H_

typedef	struct POINT
{
   uint16_t x;
   uint16_t y;
   uint8_t status;
}Coordinate;



#define READ_X_VALUE  0x90
#define READ_Y_VALUE  0xD0

#define TOUCH_PRESSED  0
#define TOUCH_RELEASED 1

#define THRESHOLD 2

int16_t ads7843_read(uint8_t Channel);
Coordinate *ads7843_readXY(void);

#endif /* INC_ADS7843_H_ */
