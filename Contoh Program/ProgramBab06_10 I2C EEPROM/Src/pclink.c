/*
 * pclink.c
 *
 *  Created on: Jun 15, 2019
 *      Author: Kang Usman
 */

#include "global.h"

uint8_t pcbuffer[16];
uint16_t pcbuffercounter;
uint8_t pcframe;

void set_led_mode(uint8_t* commanddata);
void read_led_mode(uint8_t* commanddata);


void pc_link(void){
	uint8_t cmd[4];
	uint8_t commanddata[16];
	uint16_t i,j;

	//Get command
	i=parsing_delimiter_string(cmd,pcbuffer,'#',0,pcbuffercounter);

	//Get command data
	memset(commanddata,'\0',16);
	for(j=i;j<pcbuffercounter;j++){
		commanddata[j-4]=pcbuffer[j];
	}

	//Check command
	if(memcmp(cmd, "LED", 3)==0){
		set_led_mode(commanddata);
	}

	if(memcmp(cmd,"RLM",3)==0){
		read_led_mode(commanddata);
	}

	memset(pcbuffer,'\0', sizeof(pcbuffer));
	pcbuffercounter=0;
	pcframe=0;

}

void set_led_mode(uint8_t* commanddata){
	uint8_t ledindex;
	uint8_t ledmode;
	uint8_t ledstatus;
	uint16_t blinkrate;
	uint8_t bfr[8];
	uint16_t i;

	//Command Format
	//LED#LedIndex#LedMode#LedStatus#BlinkRate

	//Get LED Index
	i=parsing_delimiter_string(bfr, commanddata,'#',0,strlen((char*)commanddata));
	ledindex=atoi((char*)bfr);

	//LED Mode
	i=parsing_delimiter_string(bfr, commanddata,'#',i,strlen((char*)commanddata));
	ledmode=atoi((char*)bfr);

	//LED Status
	i=parsing_delimiter_string(bfr, commanddata,'#',i,strlen((char*)commanddata));
	ledstatus=atoi((char*)bfr);

	//Blink Rate
	i=parsing_delimiter_string(bfr, commanddata,'#',i,strlen((char*)commanddata));
	blinkrate=atoi((char*)bfr);

	Set_LED_Operation(ledindex-1, ledmode, blinkrate, ledstatus);

	printf("LED Command OK\r\n");

}

void read_led_mode(uint8_t* commanddata){
	uint8_t ledindex;
	uint8_t bfr[8];


	//Get LED Index
	parsing_delimiter_string(bfr, commanddata,'#',0,strlen((char*)commanddata));
	ledindex=atoi((char*)bfr);

	if(LED[ledindex-1].mode==LEDMODE_BLINK)
	   printf("RLM#%d#%d\r\n",LED[ledindex-1].mode, LED[ledindex-1].blinkrate);
	else
		printf("RLM#%d#%d\r\n",LED[ledindex-1].mode, LED[ledindex-1].status);

}


