/**
 ******************************************************************************
  * File Name          : jdata_conf.h
  * Description        : This file provides header to "jdata_conf.h" module.
  *                      It implements also file based read/write functions.
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
#include "ff.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*This defines the memory allocation methods.*/
#define JMALLOC   malloc
#define JFREE     free

/*This defines the File data manager type.*/
#define JFILE            FIL

size_t read_file (FIL  *file, uint8_t *buf, uint32_t sizeofbuf);
size_t write_file (FIL  *file, uint8_t *buf, uint32_t sizeofbuf) ;

#define JFREAD(file,buf,sizeofbuf)  \
read_file (file,buf,sizeofbuf)

#define JFWRITE(file,buf,sizeofbuf)  \
write_file (file,buf,sizeofbuf)

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
