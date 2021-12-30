/*
 * led.c
 *
 *  Created on: Jun 15, 2019
 *      Author: Kang Usman
 */

#include "global.h"


LEDTypedef LED[5];

void LED_Default(void){
	LED[0].mode = LEDMODE_BLINK;
	LED[0].status = LEDSTATUS_OFF;
	LED[0].timer = HAL_GetTick();
	LED[0].blinkrate = 50;

	LED[1].mode = LEDMODE_BLINK;
	LED[1].status = LEDSTATUS_OFF;
	LED[1].timer = HAL_GetTick();
	LED[1].blinkrate = 500;

	LED[2].mode = LEDMODE_BLINK;
	LED[2].status = LEDSTATUS_OFF;
	LED[2].timer = HAL_GetTick();
	LED[2].blinkrate = 1000;

	LED[3].mode = LEDMODE_ONOFF;
	LED[3].status = LEDSTATUS_ON;
	LED[3].timer = HAL_GetTick();
	LED[3].blinkrate = 500;

	LED[4].mode = LEDMODE_BLINK;
	LED[4].status = LEDSTATUS_OFF;
	LED[4].timer = HAL_GetTick();
	LED[4].blinkrate = 200;
}

void LED_Operation(void){
	uint8_t i;

	for(i=0;i<5;i++){
		if(LED[i].mode==LEDMODE_BLINK){
			if(HAL_GetTick() - LED[i].timer>LED[i].blinkrate){
				set_LED(i,LEDMODE_BLINK,0);
				LED[i].timer = HAL_GetTick();
			}
		}
		else{
			set_LED(i,LEDMODE_ONOFF,LED[i].status);
		}
	}

}

void set_LED(uint8_t LEDIndex, uint8_t mode, uint8_t status){

	switch (LEDIndex){
	case 0:
		if(mode==LEDMODE_BLINK){
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		}
		else{
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, status);
		}
		break;

	case 1:
		if(mode==LEDMODE_BLINK){
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		}
		else{
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, status);
		}
		break;

	case 2:
		if(mode==LEDMODE_BLINK){
			HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		}
		else{
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, status);
		}
		break;

	case 3:
		if(mode==LEDMODE_BLINK){
			HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);
		}
		else{
			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, status);
		}
		break;

	case 4:
		if(mode==LEDMODE_BLINK){
			HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);
		}
		else{
			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, status);
		}
		break;

	}
}

void Set_LED_Operation(uint8_t LEDIndex, uint8_t mode, uint16_t blinkrate, uint8_t ledstatus){

	LED[LEDIndex].mode = mode;
	LED[LEDIndex].blinkrate = blinkrate;
	LED[LEDIndex].status = ledstatus;

}

