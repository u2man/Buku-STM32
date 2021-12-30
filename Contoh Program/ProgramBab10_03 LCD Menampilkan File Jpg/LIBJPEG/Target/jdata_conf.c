/**
 ******************************************************************************
  * File Name          : jdata_conf.c
  * Description        : This file implements LibJPEG file based read/write functions.
  *
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics. All rights reserved.
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/

/*FatFS is chosen for File storage*/
#include "jdata_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

size_t read_file (FIL  *file, uint8_t *buf, uint32_t sizeofbuf)
{
static size_t BytesReadfile ;
f_read (file, buf , sizeofbuf, &BytesReadfile);
return BytesReadfile;
}

size_t write_file (FIL  *file, uint8_t *buf, uint32_t sizeofbuf)
{
static size_t BytesWritefile;
f_write (file, buf , sizeofbuf, &BytesWritefile);
return BytesWritefile;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
