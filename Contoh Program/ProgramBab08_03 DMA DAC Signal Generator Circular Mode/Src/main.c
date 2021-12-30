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
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
uint8_t WaveForm;
uint16_t keypresstimer;
uint16_t Tim_Periodic=0x7FF;

uint16_t Sine_Wave_Data_Table[32] ={
		2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056,
		4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447,
		2047, 1647, 1263, 909, 599, 344, 155, 38,
		0, 38, 155, 344, 599, 909, 1263, 1647
};

uint16_t Sawtooth_Wave_Data_Table[32]={
		0, 132, 264, 396, 528, 660, 792, 924,
		1057, 1189, 1321, 1453, 1585, 1717, 1849, 1981,
		2113, 2245, 2377, 2509, 2641, 2773, 2905, 3037,
		3170, 3302, 3434, 3566, 3698, 3830, 3962, 4095
};

uint16_t Triangle_Wave_Data_Table[32]={
		0, 256, 512, 768, 1024, 1279, 1535, 1791,
		2047, 2303, 2559, 2815, 3071, 3326, 3582, 3838,
		4095, 3838, 3582, 3326, 3071, 2815, 2559, 2303,
		2047, 1791, 1535, 1279, 1024, 768, 512, 256
};

uint16_t Square_Wave_Data_Table[2]={
		0, 4095
};

uint8_t Escalator_Wave_Data_Table[6]={
		0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void DAC_Signal_Generator(void);
void TIM_Change_Periodic(void);

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
  MX_DMA_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim6);
  WaveForm=0;
  DAC_Signal_Generator();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(!HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)){
		  keypresstimer=0;
		  while(!HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)){
			  keypresstimer++;
			  HAL_Delay(1);
		  }
		  if(keypresstimer>10){
			  WaveForm++;
			  if(WaveForm>4)
				  WaveForm=0;
			  DAC_Signal_Generator();
			  }
	  }

	  if(!HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)){
		  keypresstimer=0;
		  while(!HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)){
			  keypresstimer++;
			  HAL_Delay(1);
		  }
		  if(keypresstimer>10){
			  Tim_Periodic+=50;
			  TIM_Change_Periodic();
		  }
	  }

	  if(!HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)){
		  keypresstimer=0;
		  while(!HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)){
			  keypresstimer++;
			  HAL_Delay(1);
		  }
		  if(keypresstimer>10){
			  Tim_Periodic-=50;
			  TIM_Change_Periodic();
		  }
	  }
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
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
void DAC_Signal_Generator(void){

	HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_2);
	switch (WaveForm){
	case 0:
		//Sine Wave
	  if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)Sine_Wave_Data_Table, 32, DAC_ALIGN_12B_R) != HAL_OK)
		{
		  /* Start DMA Error */
		 Error_Handler();
	    }

		break;
	case 1:
		//Sawtooth
		  if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)Sawtooth_Wave_Data_Table, 32, DAC_ALIGN_12B_R) != HAL_OK)
			{
			  /* Start DMA Error */
			 Error_Handler();
		    }

		break;
	case 2:
		//Escalator
		  if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)Escalator_Wave_Data_Table, 6, DAC_ALIGN_8B_R) != HAL_OK)
			{
			  /* Start DMA Error */
			 Error_Handler();
		    }

		break;
	case 3:
		//Trianglewave:
		  if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)Triangle_Wave_Data_Table, 32, DAC_ALIGN_12B_R) != HAL_OK)
			{
			  /* Start DMA Error */
			 Error_Handler();
		    }

		break;
	case 4:
		//Square wave
		  if(HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t*)Square_Wave_Data_Table, 2, DAC_ALIGN_12B_R) != HAL_OK)
			{
			  /* Start DMA Error */
			 Error_Handler();
		    }

		break;
	}

}

void TIM_Change_Periodic(void){
	  TIM_MasterConfigTypeDef sMasterConfig = {0};

	  HAL_TIM_Base_Stop(&htim6);
	  HAL_TIM_Base_DeInit(&htim6);

	  htim6.Instance = TIM6;
	  htim6.Init.Prescaler = 0;
	  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim6.Init.Period = Tim_Periodic;
	  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_TIM_Base_Start(&htim6);
	  DAC_Signal_Generator();

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
