/*
 * ads7843.c
 *
 *  Created on: Nov 21, 2019
 *      Author: Kang Usman
 */


#include "global.h"

Coordinate *ads7843_readXY(void){
	static Coordinate screen;
	int16_t m0, m1, m2, temp[3];
	int8_t count=0;
	int16_t buffer[2][9] ={{0},{0}};

	//printf("Read XY\r\n");
	do{
		buffer[0][count]= ads7843_read(READ_X_VALUE);
		buffer[1][count]= ads7843_read(READ_Y_VALUE);
		count++;

	}while((!HAL_GPIO_ReadPin(TOUCH_IRQ_GPIO_Port,TOUCH_IRQ_Pin))&&(count<9));

	if(count==9){
	    temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
		temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
		temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;

		m0=temp[0]-temp[1];
		m1=temp[1]-temp[2];
		m2=temp[2]-temp[0];

		m0=m0>0?m0:(-m0);
	    m1=m1>0?m1:(-m1);
		m2=m2>0?m2:(-m2);

		if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ){
			return 0;
		}

		if(m0<m1)
		{
		  if(m2<m0)
		    screen.x=(temp[0]+temp[2])/2;
		  else
		    screen.x=(temp[0]+temp[1])/2;
		}
		else if(m2<m1)
		  screen.x=(temp[0]+temp[2])/2;
		else
		  screen.x=(temp[1]+temp[2])/2;

	    temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
		temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
		temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
		m0=temp[0]-temp[1];
		m1=temp[1]-temp[2];
		m2=temp[2]-temp[0];
		m0=m0>0?m0:(-m0);
		m1=m1>0?m1:(-m1);
		m2=m2>0?m2:(-m2);
		if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

		if(m0<m1)
		{
		  if(m2<m0)
		    screen.y=(temp[0]+temp[2])/2;
		  else
		    screen.y=(temp[0]+temp[1])/2;
	    }
		else if(m2<m1)
		   screen.y=(temp[0]+temp[2])/2;
		else
		   screen.y=(temp[1]+temp[2])/2;

		screen.status= TOUCH_PRESSED;
		//printf("Return OK\r\n");
		return &screen;
	}

	//printf("Return 0\r\n");
	screen.status= TOUCH_RELEASED;

	return 0;

}

int16_t ads7843_read(uint8_t Channel){
	int16_t temp=0;
	uint8_t bfr[2];

	//TP CS Low
	HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);

	//Send Command to read X Value
	bfr[0]= Channel;
	HAL_SPI_Transmit(&hspi2, bfr, 1, 1000);

	//Wait Busy Pin
	while(HAL_GPIO_ReadPin(TOUCH_BUSY_GPIO_Port, TOUCH_BUSY_Pin)){

	}
	HAL_SPI_Receive(&hspi2, bfr, 2, 1000);

	//TP CS HIGH
	HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);


	temp = bfr[0];
	temp = (temp << 8)| bfr[1];
	temp >>=3;

	return temp;
}

