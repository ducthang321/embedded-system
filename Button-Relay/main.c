/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

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
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t h_flg = 0, t_flg = 0;
uint8_t h_val, t_val;
uint8_t relay;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
uint16_t Dec_Hex(uint8_t val);

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if ((h_flg == 1) && (t_flg == 0))
	  	  {
//		  GPIOA->ODR &= ~(1<<RS);
//		  Out_LCD2(0x01);
//		  LCD_Output(Set,0x80);
//		  LCD_Output(H,0xC0);
		  while ((h_flg == 1) && (t_flg == 0)) {
	  		  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))) //Tang
	  		  {
	  			  for (uint8_t i = 50; i >= 1; i--)
	  			  {
	  				  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))
	  					  break;
	  			  }
	  			  for (uint8_t i = 50; i >= 1; i--)
	  			  {
	  				  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)))
	  					  i = 50;
	  			  }
	  			  h_val++;
	  		  }
	  		  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))) //Giam
	  		  {
	  			  for (uint8_t i = 50; i >= 1; i--)
	  			  {
	  			  		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
	  			  			break;
	  			  }
	  			  for (uint8_t i = 50; i >= 1; i--)
	  			  {
	  				  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)))
	  			  			i = 50;
	  			  }
	  			  if (h_val > 0)
	  				  h_val--;
	  		  }
	  		  uint16_t data = Dec_Hex(h_val);
	  //		  LCD_Per(data,0x80,0x25);
	  	  }
//		  GPIOA->ODR &= ~(1<<RS);
//		  Out_LCD2(0x01);
	  	  }
	  	  if ((t_flg == 1) && (h_flg == 0))
	  	  {
//	  		GPIOA->ODR &= ~(1<<RS);
//	  		Out_LCD2(0x01);
//	  		LCD_Output(Set,0x80);
//	  		LCD_Output(T,0xC0);
	  		while ((t_flg == 1) && (h_flg == 0)) {
	  		  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))) //Tang
	  		  {
	  			  for (uint8_t i = 50; i >= 1; i--)
	  			  {
	  				  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))
	  		  			break;
	  			  }
	  			  for (uint8_t i = 50; i >= 1; i--)
	  			  {
	  				  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)))
	  					  i = 50;
	  			  }
	  			  t_val++;
	  		  	}
	  		  	if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))) //Giam
	  		  	{
	  		  		for (uint8_t i = 50; i >= 1; i--)
	  		  		{
	  		  			 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
	  		  			  	break;
	  		  		}
	  		  		for (uint8_t i = 50; i >= 1; i--)
	  		  		{
	  		  			if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)))
	  		  			  	i = 50;
	  		  		}
	  		  		if (t_val > 0)
	  		  			t_val--;
	  		  	}
	  		  	uint16_t data = Dec_Hex(t_val);
	  //		  	LCD_Per(data,0xC0,0x43);
	  	  }
//	  		GPIOA->ODR &= ~(1<<RS);
//	  		Out_LCD2(0x01);
	  	  }
//	  if ((H_byte1 <= h_val) || (T_byte1 <= t_val))
//	  	  {
//	  	  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
//	  	  	relay = 1;
//	  	  	HAL_InitTick(TICK_INT_PRIORITY);
//	  	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA2 PA4 PA5
                           PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0)
	{
		for (uint8_t i = 50; i >= 1; i--)
		{
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0))
				break;
		}
		for (uint8_t i = 50; i >= 1; i--)
		{
			if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)))
				i = 50;
		}
		h_flg++;
		if (t_flg) t_flg = 0;
		if (h_flg == 2)
			h_flg = 0;
	}
	else if (GPIO_Pin == GPIO_PIN_1)
	{
		for (uint8_t i = 50; i >= 1; i--)
		{
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1))
				break;
		}
		for (uint8_t i = 50; i >= 1; i--)
		{
			if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)))
				i = 50;
		}
		t_flg++;
		if (h_flg) h_flg = 0;
		if (t_flg == 2)
			t_flg = 0;
	}
}

uint16_t Dec_Hex(uint8_t val)
{
	uint16_t chr = val/10;
	uint8_t chr1 = (chr%10) << 4;
	chr = (chr/10) << 8;
	val %= 10;
	return chr | chr1 | val;
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
