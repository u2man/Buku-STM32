/*
 * global.c
 *
 *  Created on: Jun 15, 2019
 *      Author: Kang Usman
 */

#include "global.h"

uint16_t parsing_delimiter_string(uint8_t *storebuffer, uint8_t *databuffer, uint8_t delimitter, uint16_t startpos, uint16_t len){
	uint16_t i,x;
	x=0;
	for(i=startpos; i<startpos+len;i++){
        if((databuffer[i]==0)||(databuffer[i]==delimitter))
        	break;
        storebuffer[x++]=databuffer[i];
	}
	storebuffer[x++]=0;

	return i+1;
}

