/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
FILINFO FilInfo;
uint8_t FileName[32], FileBuffer[512];
uint32_t FileOperation;
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  retSD=f_mount(&SDFatFS,(TCHAR const*)"",1);
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
	RTC_TimeTypeDef RTCTime;
	RTC_DateTypeDef RTCDate;

	HAL_RTC_GetTime(&hrtc, &RTCTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTCDate, RTC_FORMAT_BIN);

	  return   ((DWORD)((2000+RTCDate.Year) - 1980) << 25) /* Fixed to Jan. 1, 2011 */
	   | ((DWORD)RTCDate.Month << 21)
	   | ((DWORD)RTCDate.Date << 16)
	   | ((DWORD)RTCTime.Hours << 11)
	   | ((DWORD)RTCTime.Minutes << 5)
	   | ((DWORD)RTCTime.Seconds >> 1);

  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
void Load_LED_Mode_FromFile(void){
	FRESULT res;
	uint16_t i,j;
	uint8_t bfr[10];


	res = f_stat((char*)"Setting",&FilInfo);
	if(res==FR_NO_FILE){
		res=f_mkdir((char*)"/Setting");
		if(res==FR_OK){
			printf("Membuat direktori berhasil\r\n");
		}
		else{
			printf("Membuat direktori gagal\r\n");
		}

	}
	else{
		printf("Direktori sudah ada\r\n");
	}

	for(i=0;i<5;i++){
		sprintf((char*)FileName,"Setting/LED%d_Config.txt",i+1);
		res=f_open(&SDFile,(char*) FileName, FA_READ);
		if(res==FR_OK){
			res=f_read(&SDFile, FileBuffer, 256, (void*)&FileOperation);
			if((res==FR_OK)&&(FileOperation>0)){
				//status#mode#blinkrate
				j=parsing_delimiter_string(bfr, FileBuffer, '#', 0, strlen((char*)FileBuffer));
				LED[i].status=atoi((char*)bfr);
				j=parsing_delimiter_string(bfr, FileBuffer, '#', j, strlen((char*)FileBuffer));
				LED[i].mode=atoi((char*)bfr);
				j=parsing_delimiter_string(bfr, FileBuffer, '#', j, strlen((char*)FileBuffer));
				LED[i].blinkrate=atoi((char*)bfr);

			}
		}
		f_close(&SDFile);
	}

}

void Save_LED_Mode_ToFile(uint8_t LedIndex){
	FRESULT res;

	sprintf((char*)FileName,"Setting/LED%d_Config.txt",LedIndex+1);
	res=f_open(&SDFile,(char*) FileName, FA_WRITE|FA_CREATE_ALWAYS);
	if(res==FR_OK){
		f_printf(&SDFile,"%d#%d#%d",LED[LedIndex].status,LED[LedIndex].mode,LED[LedIndex].blinkrate);
	}

	f_close(&SDFile);

}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
