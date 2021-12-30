/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * File Name          : libjpeg.c
  * Description        : This file provides code for the configuration
  *                      of the libjpeg instances.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "libjpeg.h"

/* USER CODE BEGIN 0 */
#include "global.h"
/* USER CODE END 0 */

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

/* Global variables ---------------------------------------------------------*/

/* USER CODE BEGIN 2 */
  /* This struct contains the JPEG decompression parameters */
  struct jpeg_decompress_struct cinfo;
  /* This struct represents a JPEG error handler */
  struct jpeg_error_mgr jerr;

/* USER CODE END 2 */

/* LIBJPEG init function */
void MX_LIBJPEG_Init(void)
{
/***************************************/
   /**
  */

  /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

}

/* USER CODE BEGIN 4 */
void jpeg_decode(uint8_t *filename, uint32_t width, uint8_t * buff, uint8_t (*callback)(uint8_t*, uint32_t)){
	  /* Decode JPEG Image */
	  JSAMPROW buffer[1] = {0}; /* Output row buffer */
	  uint32_t row_stride = 0; /* physical row width in image buffer */

	  FIL file;


	 // BSP_LCD_Debug((uint8_t*)"jpeg decode");

     if(f_open(&file, (char*)filename, FA_READ) != FR_OK)
     {
    	 BSP_LCD_Debug((uint8_t*)"open file error");
    	 return;
     }


	  buffer[0] = buff;

	  /* Step 1: allocate and initialize JPEG decompression object */
	  //BSP_LCD_Debug((uint8_t*)"init cinfo.err");
	  cinfo.err = jpeg_std_error(&jerr);

	  /* Initialize the JPEG decompression object */
	  //BSP_LCD_Debug((uint8_t*)"init cinfo");
	  jpeg_create_decompress(&cinfo);

      jpeg_stdio_src (&cinfo, &file);

	  /* Step 3: read image parameters with jpeg_read_header() */
      jpeg_read_header(&cinfo, TRUE);

	  /* Step 4: set parameters for decompression */
	  cinfo.dct_method = JDCT_IFAST;

	  /* Step 5: start decompressor */
	  if((cinfo.image_width>320)&&(cinfo.image_width<320*3)){
	   cinfo.scale_num =(uint16_t) (cinfo.image_width/320);
	   cinfo.scale_denom =(uint16_t)(cinfo.image_width/320)*2;
	  }
	  if(cinfo.image_width>=320*3){
		  cinfo.scale_num=1;
		  cinfo.scale_denom=8;
	  }

	  jpeg_start_decompress(&cinfo);
	  printf("image size %d x %d\r\n",cinfo.image_width, cinfo.image_height);
	  printf("outpur %d x %d\r\n",cinfo.output_width, cinfo.output_height);

	  //if((cinfo.image_width==320)&&(cinfo.image_height==240)){
	  row_stride = width * 3;

      BSP_LCD_Clear(LCD_COLOR_BLACK);
	  while (cinfo.output_scanline < cinfo.output_height)
	  {
	    (void) jpeg_read_scanlines(&cinfo, buffer, 1);

	    if (callback(buffer[0], row_stride) != 0)
	    {
	      break;
	    }
	  }

	  /* Step 6: Finish decompression */
	  jpeg_finish_decompress(&cinfo);

	  /* Step 7: Release JPEG decompression object */
	  jpeg_destroy_decompress(&cinfo);

	  f_close(&file);
}

/* USER CODE END 4 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
