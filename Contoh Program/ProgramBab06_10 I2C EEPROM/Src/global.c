/*
 * global.c
 *
 *  Created on: Jun 15, 2019
 *      Author: Kang Usman
 */

#include "global.h"

uint8_t CalculateCRC8(uint8_t* buffer, int length)
{
  uint8_t crc8,tmp,j,k;

  crc8 =0;
  if (!length)
	crc8= 0xFF;
  else
  {
	while (length--)
	{
	 tmp = (*buffer++);
	 for (k=0;k<7;k++)
	 {
		j = 1 & (tmp ^ crc8);
        crc8 = (crc8/2) & 0xFF;
        tmp =  (tmp/2) & 0xFF;
        if (j!=0)
          crc8 ^= 0x8C;
	 }
	}
  }
  return (crc8);
}

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
