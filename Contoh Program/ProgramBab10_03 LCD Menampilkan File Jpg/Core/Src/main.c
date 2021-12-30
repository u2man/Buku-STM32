/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "libjpeg.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "global.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
RGB_typedef *RGB_matrix;
uint8_t _aucLine[5*2048];
uint32_t line_counter= 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Display_File_JPG_uSD(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_SDIO_SD_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_LIBJPEG_Init();
  /* USER CODE BEGIN 2 */
  printf("LCD Menampilkan file jpg\r\n");
   if (BSP_LCD_Init()==LCD_OK){
	   printf("LCD Init OK\r\n");
	   BSP_LCD_Clear(LCD_COLOR_BLACK);
	   BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	   BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	   BSP_LCD_SetFont(&Font12);

	   BSP_LCD_Debug((uint8_t*)"LCD Menampilkan file BMP");
	   BSP_LCD_Debug( (uint8_t*)"SD Card Mount...");
	   if (retSD!=FR_OK){
		   BSP_LCD_Debug( (uint8_t*)"SD Card Mount...ERROR");
		   for(;;);
	   }
	   BSP_LCD_Debug( (uint8_t*)"SD Card Mount...OK");

    }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  Display_File_JPG_uSD();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 20;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void Display_File_JPG_uSD(void){
	FRESULT res;
	DIR dir;
	FILINFO fno;

	fno.lfname = malloc(_MAX_LFN);
	fno.lfsize = _MAX_LFN-1;

	BSP_LCD_Debug( (uint8_t*)"Chaneg directori to /Picture...");
	//Change directory to /Picture
	if (f_chdir("/Picture")==FR_OK){
		BSP_LCD_Debug( (uint8_t*)"Chaneg directori to /Picture...OK");
		//search file *.jpg
		BSP_LCD_Debug( (uint8_t*)"Search .jpg and jpeg files...");
		res=f_findfirst(&dir, &fno, "","*.j*g");

		while((res == FR_OK) && (fno.fname[0])){
		 line_counter=0;
		 memset(_aucLine,'\0',sizeof(_aucLine));
		 if(fno.fattrib & AM_DIR){
		 printf("open file %s\r\n", fno.fname);
		 }
	     jpeg_decode((uint8_t*)fno.fname, IMAGE_WIDTH, _aucLine, Jpeg_CallbackFunction);
	     res = f_findnext(&dir,&fno);
		 HAL_Delay(2000);
		}

		f_closedir(&dir);
		free(fno.lfname);
		printf("Finish %d\r\n",res);

	}
	else{
		BSP_LCD_Debug( (uint8_t*)"Chaneg directori to /Picture...Error");
	}

	free(fno.lfname);


}

uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength)
{
  uint32_t i = 0;
  RGB_matrix =  (RGB_typedef*)Row;
  uint16_t  RGB16Buffer[IMAGE_WIDTH];

  for(i = 0; i < IMAGE_WIDTH; i++)
  {
    RGB16Buffer[i]  = (uint16_t)
    (
     ((RGB_matrix[i].R & 0x00F8) >> 3)|
     ((RGB_matrix[i].G & 0x00FC) << 3)|
     ((RGB_matrix[i].B & 0x00F8) << 8)
    );

    BSP_LCD_DrawPixel(i, line_counter, RGB16Buffer[i]);
  }
  line_counter++;
  return 0;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
