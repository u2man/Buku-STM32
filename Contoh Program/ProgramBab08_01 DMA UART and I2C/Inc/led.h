/*
 * led.h
 *
 *  Created on: Aug 13, 2019
 *      Author: Kang Usman
 */

#ifndef LED_H_
#define LED_H_

#define LEDMODE_ONOFF  0
#define LEDMODE_BLINK  1
#define LEDSTATUS_OFF  0
#define LEDSTATUS_ON   1

typedef struct{

	uint8_t  status;
	uint32_t timer;
	uint8_t  mode;
	uint16_t blinkrate;

}LEDTypedef;

extern LEDTypedef LED[5];

void Set_LED_Operation(uint8_t LEDIndex, uint8_t mode, uint16_t blinkrate, uint8_t ledstatus);
void LED_Operation(void);
void LED_Default(void);
void set_LED(uint8_t LEDIndex, uint8_t mode, uint8_t status);

void Write_LED_Setting(uint8_t LEDIndex);


#endif /* LED_H_ */
