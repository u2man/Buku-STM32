/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "fatfs.h"
#include "global.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */
 uint8_t FileBuffer[1024];
 uint32_t FileOperation;
/* USER CODE END Variables */    

void MX_FATFS_Init(void) 
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  retSD=f_mount(&SDFatFS,(TCHAR const*)"",1);
  if(retSD==FR_OK){
	  printf("SD Mount OK\r\n");
  }
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */  
}

/* USER CODE BEGIN Application */
void Open_Bitmap_File(uint8_t* BMPFileName){
    uint8_t bfr[64];
    uint16_t Bfr16;
    uint32_t Bfr32,BMPWidth,BMPHeight,ImageOffset;
    uint32_t RowSize;
    uint16_t w,h,row, col;
    uint8_t r,g,b;
    uint32_t pos=0;
    uint16_t PixBfrId;
    static uint16_t ArrayRGB[320] = {0};


    sprintf((char*)bfr,"Open file %s",BMPFileName);
    BSP_LCD_Debug(bfr);
	retSD = f_open(&SDFile,(char*) BMPFileName, FA_READ);
	if(retSD==FR_OK){
	   BSP_LCD_Debug((uint8_t*)"Open File OK");
	   BSP_LCD_Debug((uint8_t*)"Read BMP Header");
	   if (f_read(&SDFile, FileBuffer, 512, (void*)&FileOperation)==FR_OK){

		   //Read "BM" 0x4D42
		   Bfr16 = ReadValueFromBuffer(FileBuffer, 2, 0);
           sprintf((char*)bfr,"Header BMP %#04X",Bfr16);
           BSP_LCD_Debug(bfr);

           //File Size
           Bfr32 = ReadValueFromBuffer(FileBuffer, 4, 2);
           sprintf((char*)bfr,"File Size %lu Bytes",Bfr32);
           BSP_LCD_Debug(bfr);

           BMPWidth = ReadValueFromBuffer(FileBuffer, 4, 18);
           sprintf((char*)bfr,"Width %lu",BMPWidth);
           BSP_LCD_Debug(bfr);

           BMPHeight = ReadValueFromBuffer(FileBuffer, 4, 22);
           sprintf((char*)bfr,"Height %lu",BMPHeight);
           BSP_LCD_Debug(bfr);

           if((BMPWidth>BSP_LCD_GetXSize())||(BMPHeight>BSP_LCD_GetYSize())){
        	   BSP_LCD_Debug((uint8_t*)"Format not supported");
               for(;;);
           }

           Bfr16 = ReadValueFromBuffer(FileBuffer, 2, 26);
           sprintf((char*)bfr,"Plane %d",Bfr16);
           BSP_LCD_Debug(bfr);
           if(Bfr16!=1){
        	   BSP_LCD_Debug((uint8_t*)"Format not supported");
               for(;;);
           }

           Bfr16 = ReadValueFromBuffer(FileBuffer, 2, 28);
           sprintf((char*)bfr,"Bit per Pixel %d",Bfr16);
           BSP_LCD_Debug(bfr);

           Bfr32 = ReadValueFromBuffer(FileBuffer, 4, 30);
           sprintf((char*)bfr,"Compression %lu",Bfr32);
           BSP_LCD_Debug(bfr);

           if((Bfr16!=24)||(Bfr32>0)){
        	   BSP_LCD_Debug((uint8_t*)"Format not supported");
               for(;;);
           }

           ImageOffset = ReadValueFromBuffer(FileBuffer, 4, 10);
           sprintf((char*)bfr,"Image Offset %lu",ImageOffset);
           BSP_LCD_Debug(bfr);
	   }
	   else{
		   BSP_LCD_Debug((uint8_t*)"Read File Error");
		   for(;;);
	   }

	   BSP_LCD_Debug((uint8_t*)"Displaying Image");
	   RowSize = (BMPWidth*3+3) & ~ 3;
	   w = BMPWidth;
	   h = BMPHeight;

	   for(row=0; row<h; row++){
		   pos = ImageOffset + (BMPHeight-1-row)*RowSize;
		   f_lseek(&SDFile, pos);
		   f_read(&SDFile,FileBuffer, 3*BMPWidth, (void*)&FileOperation);
		   PixBfrId=0;
		   for(col=0;col<w;col++){
              b = FileBuffer[PixBfrId++];
              g = FileBuffer[PixBfrId++];
              r = FileBuffer[PixBfrId++];
              //BSP_LCD_DrawPixel(col, row, Color565_Converter(r, g, b));
              ArrayRGB[col] = Color565_Converter(r, g, b);

		   }
		   ssd1289_SetCursor(col, pos);
		   ssd1289_WriteMultipleData((uint8_t*)&ArrayRGB[0], 320*2);
	   }
	}
	else{
		BSP_LCD_Debug((uint8_t*)"Open file Error\r\n");
	}
}

uint32_t ReadValueFromBuffer(uint8_t* Buffer, uint16_t Size, uint16_t Offset){
	uint32_t temp=0;
	uint16_t i;
	uint8_t bfr[4];

	memset(bfr,'\0',4);

	for(i=Offset;i<Offset+Size;i++){
		bfr[i-Offset]=Buffer[i];
	}

	if(Size==2){
		//return 16 byte
		temp = bfr[1];
		temp = (temp << 8) + bfr[0];
	}
	if(Size==4){
		temp = bfr[3];
		temp = (temp << 8) + bfr[2];
		temp = (temp << 8) + bfr[1];
		temp = (temp << 8) + bfr[0];
	}
	return temp;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
