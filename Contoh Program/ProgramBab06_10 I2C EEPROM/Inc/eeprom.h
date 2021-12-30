/*
 * eeprom.h
 *
 *  Created on: Jun 29, 2019
 *      Author: Kang Usman
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROM_ADDR		0xA0

uint8_t EEPROM_Read(uint16_t Addr,uint8_t *buf,uint16_t num);
uint8_t EEPROM_Write(uint16_t Addr,uint8_t *buf,uint16_t num);


#endif /* EEPROM_H_ */
