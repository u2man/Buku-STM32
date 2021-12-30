/*
 * eeprom.c
 *
 *  Created on: Jun 29, 2019
 *      Author: Kang Usman
 */


#include "global.h"

extern I2C_HandleTypeDef hi2c2;

uint8_t EEPROM_Read(uint16_t Addr,uint8_t *buf,uint16_t num){
	uint8_t temp;

	temp = HAL_I2C_Mem_Read(&hi2c2, EEPROM_ADDR, Addr,I2C_MEMADD_SIZE_16BIT, buf, num, 0xFFFF);
	return temp;

}

uint8_t EEPROM_Write(uint16_t Addr,uint8_t *buf,uint16_t num){
	uint8_t temp;
	temp = HAL_I2C_Mem_Write(&hi2c2, EEPROM_ADDR, Addr,I2C_MEMADD_SIZE_16BIT, buf, num, 0xFFFF);
	HAL_Delay(10);

	return temp;

}
