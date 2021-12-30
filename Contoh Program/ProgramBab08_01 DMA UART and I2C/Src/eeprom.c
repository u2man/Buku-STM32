/*
 * eeprom.c
 *
 *  Created on: Aug 13, 2019
 *      Author: Kang Usman
 */


#include "global.h"

extern I2C_HandleTypeDef hi2c2;

uint8_t EEPROM_Read(uint16_t Addr,uint8_t *buf,uint16_t num){
	uint8_t temp;
	uint32_t WaitTimeout=0;


	temp = HAL_I2C_Mem_Read_DMA(&hi2c2, EEPROM_ADDR, Addr,I2C_MEMADD_SIZE_16BIT, buf, num);
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){
		if(WaitTimeout>100){
			break;
		}
		HAL_Delay(1);
	}
	return temp;

}

uint8_t EEPROM_Write(uint16_t Addr,uint8_t *buf,uint16_t num){
	uint8_t temp;
	uint32_t WaitTimeout=0;

	temp = HAL_I2C_Mem_Write_DMA(&hi2c2, EEPROM_ADDR, Addr,I2C_MEMADD_SIZE_16BIT, buf, num);
	while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY){
		if(WaitTimeout++>100)
			break;
		HAL_Delay(1);
	}
	return temp;

}
