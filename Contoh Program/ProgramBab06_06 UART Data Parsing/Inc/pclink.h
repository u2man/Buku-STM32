/*
 * pclink.h
 *
 *  Created on: Jun 15, 2019
 *      Author: Kang Usman
 */

#ifndef PCLINK_H_
#define PCLINK_H_

extern uint8_t pcbuffer[16];
extern uint16_t pcbuffercounter;
extern uint8_t pcframe;

void pc_link(void);


#endif /* PCLINK_H_ */
