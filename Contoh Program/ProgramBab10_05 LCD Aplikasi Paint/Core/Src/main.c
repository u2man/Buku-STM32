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
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "global.h"
#include "color.h"
#include "save.h"
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
static uint32_t Radius = 2;
static uint32_t x = 0, y = 0;
Coordinate touch;

const uint32_t aBMPHeader[14]=
{0x2C364D42,
 0x00000001, /*0x224A6: (240-60)*(320-60)*3 + 54*/
 0x00360000, 0x00280000,
 0x00F00000, /* Y=(320-60)=0x104*/
 0x00A00000, /* X=(240-60)=0xB4*/
 0x00010000, 0x00000018, 0xF5400000, 0x00000006, 0x00000000, 0x00000000, 0x00000000, 0x0000};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void Draw_Menu(void);
void GetPosition(void);
static void Save_Picture(void);
static void Update_Color(void);
static void Update_Size(uint8_t size);

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
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
   if (BSP_LCD_Init()!=LCD_OK){
	 printf("LCD Error\r\n");
	 for(;;);
   }

   //Touch Panel Calibration
   touch_calibration();

   //Draw_Menu();
   BSP_LCD_Clear(LCD_COLOR_BLUE);
   BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
   BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
   BSP_LCD_FillCircle(150, 150, 50);
   for(;;){
	   touch_readPosition(&touch);
	      if(touch.status==TOUCH_PRESSED){
	    	  x = touch.x;
	    	  y=touch.y;
	    	    if((x > 125) & (y > 125) & ( x < 175) & (y < 175))
	    	    {
	    	    	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	    	    	BSP_LCD_FillCircle(150, 150, 50);
	    	    	HAL_Delay(100);
    	    	    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
	    	    	BSP_LCD_FillCircle(150, 150, 50);

	    	    }
	      }


   }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  GetPosition();
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

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
static void Draw_Menu(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Draw color image */
  BSP_LCD_DrawBitmap(0, 0, (uint8_t *)color);

  /* Draw save image */
  BSP_LCD_DrawBitmap(160, (BSP_LCD_GetYSize() - 50), (uint8_t *)save);

  /* Set Black as text color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  /* Draw working window */
  BSP_LCD_DrawRect(61, 0, (BSP_LCD_GetXSize()-61), (BSP_LCD_GetYSize()-60));
  BSP_LCD_DrawRect(63, 3, (BSP_LCD_GetXSize()-66), (BSP_LCD_GetYSize()-66));
  BSP_LCD_DrawRect(65, 5, (BSP_LCD_GetXSize()-70), (BSP_LCD_GetYSize()-70));
  BSP_LCD_DrawRect(67, 7, (BSP_LCD_GetXSize()-74), (BSP_LCD_GetYSize()-74));

  /* Draw size icons */
  BSP_LCD_FillRect(60, (BSP_LCD_GetYSize() - 46), 90, 44);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(80, (BSP_LCD_GetYSize() - 24), 10);
  BSP_LCD_FillCircle(110, (BSP_LCD_GetYSize() - 24), 5);
  BSP_LCD_FillCircle(135, (BSP_LCD_GetYSize() - 24), 2);

  BSP_LCD_SetTextColor(LCD_COLOR_DARKRED);
  BSP_LCD_SetFont(&Font8);
  BSP_LCD_DisplayStringAt(210, (BSP_LCD_GetYSize() - 55), (uint8_t *)"Selected Color  Size", LEFT_MODE);
  BSP_LCD_DisplayStringAt(60, (BSP_LCD_GetYSize() - 55), (uint8_t *)"Select Pen Size", LEFT_MODE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(220, (BSP_LCD_GetYSize() - 40), 30, 30);
  BSP_LCD_FillCircle(290, (BSP_LCD_GetYSize() - 24), Radius);
}

void GetPosition(void){
	  static uint32_t color_width;
	  static uint32_t color;


	  color_width = 18;

	  //Get Touch screen position
      touch_readPosition(&touch);

      if(touch.status==TOUCH_PRESSED){
    	  x = touch.x;
    	  y=touch.y;
    	    if((x > (67 + Radius)) & (y > (7 + Radius) ) & ( x < (BSP_LCD_GetXSize() - (7 + Radius ))) & (y < (BSP_LCD_GetYSize() - (67 + Radius ))))
    	    {
    	      BSP_LCD_FillCircle((x), (y), Radius);
    	    }
    	    else if((x > 0 ) & ( x < 50 ))
    	    {
    	      if((y > 0 ) & (y < color_width ))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    	        Update_Size(Radius);
    	      }
    	      else if((y > color_width ) & (y < (2 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (2 * color_width)) & (y < (3 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (3 * color_width)) & (y < (4 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_LIGHTMAGENTA);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (4 * color_width)) & (y < (5 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (5 * color_width)) &(y < (6 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (6 * color_width)) &(y < (7 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_BROWN);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (7 * color_width)) & (y < (8 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_RED);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (8 * color_width)) & (y < (9 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_DARKMAGENTA);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (9 * color_width)) & (y < (10 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_CYAN);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (10 * color_width)) & (y < (11 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (11 * color_width)) & (y < (12 * color_width)))
    	      {
    	        BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    	        Update_Size(Radius);
    	      }
    	      else if((y > (12 * color_width)) & (y < (13 * color_width)))
    	      {
    	        /* Get the current text color */
    	        color = BSP_LCD_GetTextColor();
    	        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    	        /* Clear the working window */
    	        BSP_LCD_FillRect(68, 8, 245, 164);
    	        BSP_LCD_SetTextColor(color);
    	      }
    	      else
    	      {
    	        x = 0;
    	        y = 0;
    	      }
    	      Update_Color();
    	    }
    	    else if((x > 70) & (y > (BSP_LCD_GetYSize() - 50)) & (y < (BSP_LCD_GetYSize()) ) & ( x < 100))
    	    {
    	      Radius = 10;
    	      Update_Size(Radius);
    	    }
    	    else if((x > 100) & (y > (BSP_LCD_GetYSize() - 50)) & (y < (BSP_LCD_GetYSize()) ) & ( x < 130))
    	    {
    	      Radius = 5;
    	      Update_Size(Radius);
    	    }
    	    else if((x > 130) & (y > (BSP_LCD_GetYSize() - 50)) & (y < (BSP_LCD_GetYSize()) ) & ( x < 160))
    	    {
    	      Radius = 2;
    	      Update_Size(Radius);
    	    }
    	    else if(((x > (BSP_LCD_GetXSize()-5) ) & (y > (12 * color_width)) & (y < (13 * color_width))) | ((x < 55) & (y < 5)))
    	    {
    	      touch.x = 0;
    	      touch.y = 0;
    	    }
    	    else if((x > 160) & (y > (BSP_LCD_GetYSize() - 50)) & (x < 210) & (y < BSP_LCD_GetYSize()))
    	    {
    	      Save_Picture();
    	    }
    	    touch.status=TOUCH_RELEASED;
    	  }
}

void Save_Picture(void)
{
  FRESULT res1;             /* FatFs function common result code */
  uint32_t byteswritten = 0;     /* File write count */
  static uint32_t counter = 0;
  uint8_t str[30];

  uint32_t x_counter = 0, y_counter = 0;
  uint16_t tmp = 0,x;
  uint8_t filebuffer[1024];
  uint8_t r,g,b;

  printf("Save Picture\r\n");
  /* Check if the SD card is plugged in the slot */
  if(BSP_SD_IsDetected() != SD_PRESENT)
  {
    Error_Handler();
  }
  else
  {
    /* Format the string */
    sprintf((char *)str, "image_%lu.bmp", counter);

    /*##-1- Prepare the image to be saved ####################################*/
   // Prepare_Picture();

    /*##-2- Create and Open a new bmp file object with write access ##########*/
    if(f_open(&SDFile, (const char*)str, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
      /* 'image.bmp' file Open for write Error */
      printf("Save picture error\r\n");
      Error_Handler();
    }
    else
    {
      /*##-3- Write data to the BMP file #####################################*/
      /* Write the BMP header */
      res1 = f_write(&SDFile, (uint32_t *)aBMPHeader, 54, (void *)&byteswritten);

      /* Write the BMP file */
      for(y_counter = (BSP_LCD_GetYSize() - 70); y_counter >10 ; y_counter--)
        {
          x=0;
          for(x_counter = 70; x_counter < (BSP_LCD_GetXSize() - 10); x_counter++)
          {
            /* Write data to the SRAM memory */
            tmp = BSP_LCD_ReadPixel(x_counter, y_counter);
            /* Color conversion */
            r = (tmp & 0xF800) >> 11;
            g = (tmp & 0x07E0) >> 5;
            b = tmp & 0x001F;

            filebuffer[x++]=(r * 255)/31;
            filebuffer[x++]=(g * 255)/63;
            filebuffer[x++]=(b * 255)/31;

          }

    	res1=f_write(&SDFile, filebuffer, x,(void *)&byteswritten);
    	if((res1!=FR_OK)&&(byteswritten==0))
    		break;
      }

      if((res1 != FR_OK) || (byteswritten == 0))
      {
        /* 'image.bmp' file Write or EOF Error */
        Error_Handler();
        printf("Save picture failed\r\n");
      }
      else
      {
        /*##-4- Close the open BMP file ######################################*/
        f_close(&SDFile);
        printf("Save picture OK\r\n");
        /* Wait for 2s */
        HAL_Delay(2000);
        counter++;
      }
    }
  }
}


static void Update_Color(void)
{
  static uint32_t color;

  /* Get the current text color */
  color = BSP_LCD_GetTextColor();

  /* Update the selected color icon */
  BSP_LCD_FillRect(220, (BSP_LCD_GetYSize() - 40), 30, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(220, (BSP_LCD_GetYSize() - 40), 30, 30);
  BSP_LCD_SetTextColor(color);
}

static void Update_Size(uint8_t size)
{
  static uint32_t color;

  /* Get the current text color */
  color = BSP_LCD_GetTextColor();

  /* Update the selected size icon */
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillCircle(290, (BSP_LCD_GetYSize() - 24), 20);
  BSP_LCD_SetTextColor(color);
  BSP_LCD_FillCircle(290, (BSP_LCD_GetYSize() - 24), size);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawCircle(290, (BSP_LCD_GetYSize() - 24), size);
  BSP_LCD_SetTextColor(color);
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
