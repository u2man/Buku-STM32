/*
 * ssd1289.c
 *
 *  Created on: Nov 5, 2019
 *      Author: Kang Usman
 */


#include "global.h"


#define LCD_REG              (*((volatile unsigned short *) 0x68000000)) /* RS = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x68020000)) /* RS = 1 */


LCD_DrvTypeDef   ssd1289_drv =
{
  ssd1289_Init,
  ssd1289_ReadID,
  ssd1289_DisplayOn,
  ssd1289_DisplayOff,
  ssd1289_SetCursor,
  ssd1289_WritePixel,
  ssd1289_ReadPixel,
  ssd1289_SetDisplayWindow,
  ssd1289_DrawHLine,
  ssd1289_DrawVLine,
  ssd1289_GetLcdPixelWidth,
  ssd1289_GetLcdPixelHeight,
  ssd1289_DrawBitmap,
  ssd1289_DrawRGBImage,
};

static uint16_t ArrayRGB[320] = {0};


void ssd1289_Init(void){

	ssd1289_WriteReg(0x0000,0x0001);
	ssd1289_WriteReg(0x0001,0x3B3F);
	ssd1289_WriteReg(0x0001,0x3B3F); // 0x2B3F
    ssd1289_WriteReg(0x0002,0x0600);
    ssd1289_WriteReg(0x0003,0xA0A8);
    ssd1289_WriteReg(0x0005,0x0000);
    ssd1289_WriteReg(0x0006,0x0000);
    ssd1289_WriteReg(0x0007,0x0233);
    ssd1289_WriteReg(0x000B,0x0000);
    ssd1289_WriteReg(0x000C,0x0000);
    ssd1289_WriteReg(0x000D,0x000F);
    ssd1289_WriteReg(0x000E,0x2B00);
    ssd1289_WriteReg(0x000F,0x0000);
    ssd1289_WriteReg(0x0010,0x0000);
    ssd1289_WriteReg(0x0011,0x6018); // Up-Right
    ssd1289_WriteReg(0x0016,0xEF1C);
    ssd1289_WriteReg(0x0017,0x0003);
    ssd1289_WriteReg(0x001E,0x00B8);
    ssd1289_WriteReg(0x0023,0x0000);
    ssd1289_WriteReg(0x0024,0x0000);
    ssd1289_WriteReg(0x0025,0x8000);
    ssd1289_WriteReg(0x0030,0x0707);
    ssd1289_WriteReg(0x0031,0x0204);
    ssd1289_WriteReg(0x0032,0x0204);
    ssd1289_WriteReg(0x0033,0x0502);
    ssd1289_WriteReg(0x0034,0x0507);
    ssd1289_WriteReg(0x0035,0x0204);
    ssd1289_WriteReg(0x0036,0x0204);
    ssd1289_WriteReg(0x0037,0x0502);
    ssd1289_WriteReg(0x003A,0x0302);
    ssd1289_WriteReg(0x003B,0x0302);
    ssd1289_WriteReg(0x0041,0x0000);
    ssd1289_WriteReg(0x0042,0x0000);
    ssd1289_WriteReg(0x0044,0xEF00);
    ssd1289_WriteReg(0x0045,0x0000);
    ssd1289_WriteReg(0x0046,0x013F);
    ssd1289_WriteReg(0x0048,0x0000);
    ssd1289_WriteReg(0x0049,0x013F);
    ssd1289_WriteReg(0x004A,0x0000);
    ssd1289_WriteReg(0x004B,0x0000);
    ssd1289_WriteReg(0x004e,0);
    ssd1289_WriteReg(0x004f,0);

}

uint16_t ssd1289_ReadID(void){
   return (ssd1289_ReadReg(0x00));
}

void ssd1289_WriteReg(uint8_t LCDReg, uint16_t LCDRegValue){
  LCD_REG = LCDReg;
  LCD_RAM = LCDRegValue;

}

uint16_t ssd1289_ReadReg(uint8_t LCDReg){
	LCD_REG = LCDReg;
	return LCD_RAM;
}

void ssd1289_DisplayOn(void){

}

void ssd1289_DisplayOff(void){

}

void ssd1289_SetCursor(uint16_t Xpos, uint16_t Ypos){
 ssd1289_WriteReg(LCD_REG_78, Ypos);
 ssd1289_WriteReg(LCD_REG_79, 0x13F -Xpos);

}

void ssd1289_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode){

	/* Set Cursor */
	ssd1289_SetCursor(Xpos, Ypos);

	/* Prepare to write GRAM */
	LCD_REG = LCD_REG_34;

	/* Write 16-bit GRAM Reg */
	LCD_RAM = RGBCode;


}

uint16_t ssd1289_ReadPixel(uint16_t Xpos, uint16_t Ypos){
	/* Set Cursor */
	ssd1289_SetCursor(Xpos, Ypos);

	LCD_REG = LCD_REG_34;

	return LCD_RAM;

}

void ssd1289_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length){
	uint16_t counter = 0;

	ssd1289_SetCursor(Xpos, Ypos);

	LCD_REG = LCD_REG_34;

	  /* Sent a complete line */
	  for(counter = 0; counter < Length; counter++)
	  {
	    ArrayRGB[counter] = RGBCode;
	  }


	ssd1289_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);

}

void ssd1289_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length){
	uint16_t counter = 0;


	  /* set GRAM write direction and BGR = 1 */
	  /* I/D=01 (Horizontal : increment, Vertical : decrement) */
	  /* AM=1 (address is updated in vertical writing direction) */
	ssd1289_WriteReg(LCD_REG_17, 0x6010);

	ssd1289_SetCursor(Xpos, Ypos);

	LCD_REG = LCD_REG_34;

	  /* Fill a complete vertical line */
	  for(counter = 0; counter < Length; counter++)
	  {
	    ArrayRGB[counter] = RGBCode;
	  }

	  ssd1289_WriteMultipleData((uint8_t*)&ArrayRGB[0], Length * 2);

	  ssd1289_WriteReg(LCD_REG_17, 0x6018);


}

void ssd1289_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp){
	  uint32_t index = 0, size = 0;
	  /* Read bitmap size */
	  size = *(volatile uint16_t *) (pbmp + 2);
	  size |= (*(volatile uint16_t *) (pbmp + 4)) << 16;
	  /* Get bitmap data address offset */
	  index = *(volatile uint16_t *) (pbmp + 10);
	  index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;
	  size = (size - index)/2;
	  pbmp += index;

	  ssd1289_WriteReg(LCD_REG_17, 0x6008);

	  ssd1289_SetCursor(Xpos, Ypos);

	  LCD_REG = LCD_REG_34;

	  ssd1289_WriteMultipleData((uint8_t*)pbmp, size*2);

	  ssd1289_WriteReg(LCD_REG_17, 0x6018);

}

void ssd1289_prepareGRAM(void){
	LCD_REG = LCD_REG_34;
}

void ssd1289_WriteGRAM(uint16_t data){
	LCD_RAM = data;
}


void ssd1289_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint8_t *pdata){
	  uint32_t size = 0;

	  size = (Xsize * Ysize);
	  ssd1289_SetCursor(Xpos, Ypos);

	  LCD_REG = LCD_REG_34;

	  ssd1289_WriteMultipleData((uint8_t*)pdata, size*2);

}

void ssd1289_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height){
	ssd1289_WriteReg(LCD_REG_68, (Height - 1 + Ypos) << 8 | Ypos);
	ssd1289_WriteReg(LCD_REG_69, SSD1289_LCD_PIXEL_WIDTH - Width - Xpos);
	ssd1289_WriteReg(LCD_REG_70, SSD1289_LCD_PIXEL_WIDTH - 1 - Xpos);

}

void ssd1289_WriteMultipleData(uint8_t *pData, uint32_t Size){
  uint32_t counter;
  uint16_t *ptr = (uint16_t *) pData;

  for (counter = 0; counter < Size; counter+=2)
  {
    /* Write 16-bit Reg */
    LCD_RAM=*ptr;
    ptr++;
  }

}

uint16_t ssd1289_GetLcdPixelWidth(void){
	return (uint16_t)320;
}

uint16_t ssd1289_GetLcdPixelHeight(void){
	return (uint16_t)240;
}
