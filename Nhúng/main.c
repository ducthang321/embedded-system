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
#include "liquidcrystal_i2c.h"
#include <stdio.h>
#include "string.h"
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
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t h_flg = 0, t_flg = 0;
uint8_t h_val, t_val;
uint8_t relay;
uint8_t data;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
uint16_t Dec_Hex(uint8_t val);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t DHT11_flg = 1;

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_8
uint8_t RHI, RHD, TCI, TCD, SUM;
uint32_t pMillis, cMillis;
float temperature;
float humidity;
uint8_t TFI = 0;
uint8_t TFD = 0;
char strCopy[20];
uint8_t DHT11_flg;
void Usart_Init();
void Usart_Trans(uint8_t data);

void microDelay (uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(&htim1, 0);
  while (__HAL_TIM_GET_COUNTER(&htim1) < delay);
}

uint8_t DHT11_Start (void)
{
  uint8_t Response = 0;
  GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
  GPIO_InitStructPrivate.Pin = DHT11_PIN;
  GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate); // set the pin as output
  HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 0);   // pull the pin low
  HAL_Delay(20);   // wait for 20ms
  HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN, 1);   // pull the pin high
  microDelay (30);   // wait for 30us
  GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate); // set the pin as input
  microDelay (40);
  if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
  {
    microDelay (80);
    if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
  }
  pMillis = HAL_GetTick();
  cMillis = HAL_GetTick();
  while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
  {
    cMillis = HAL_GetTick();
  }
  return Response;
}

uint8_t DHT11_Read (void)
{
  uint8_t a,b;
  for (a=0;a<8;a++)
  {
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
    {  // wait for the pin to go high
      cMillis = HAL_GetTick();
    }
    microDelay (40);   // wait for 40 us
    if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
      b&= ~(1<<(7-a));
    else
      b|= (1<<(7-a));
    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) && pMillis + 2 > cMillis)
    {  // wait for the pin to go low
      cMillis = HAL_GetTick();
    }
  }
  return b;
}
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
  MX_TIM1_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
    HD44780_Init(2);
    HD44780_Clear();
    HD44780_SetCursor(0,0);
    HD44780_PrintStr("Initializing...");
    HAL_Delay(2000);
    HD44780_Clear();
//    Usart_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if ((DHT11_flg)&&(h_flg == 0)&&(t_flg == 0)) {
	  if(DHT11_Start())
	       {
	         RHI = DHT11_Read(); // Relative humidity integral
	         RHD = DHT11_Read(); // Relative humidity decimal
	         TCI = DHT11_Read(); // Celsius integral
	         TCD = DHT11_Read(); // Celsius decimal
	         SUM = DHT11_Read(); // Check sum
	         if (RHI + RHD + TCI + TCD == SUM)
	              {
	                HD44780_SetCursor(0, 0);
	                sprintf(strCopy,"Temperature: %d C   ", TCI);
	                HD44780_PrintStr(strCopy);
	                HD44780_SetCursor(0, 1);
	                sprintf(strCopy, "Humidity: %d %%", RHI);
	                HD44780_PrintStr(strCopy);
	              }
	         if (!relay)  HAL_InitTick(TICK_INT_PRIORITY);
	         DHT11_flg = 0;
	            }
	  else
	  {
		  HD44780_Clear();
		  HD44780_SetCursor(0, 0);
		  HD44780_PrintStr("No sensor");
	  }
//	  for (uint8_t i=0; i<8;i++)
//	  {
//		  buff_tx[i] = ((RHI>>(7-i))&(0x01));
//	  }
//	  for (uint8_t i=8; i<16;i++)
//	  	  {
//	  		  buff_tx[i] = ((RHD>>(15-i))&(0x01));
//	  	  }
//	  for (uint8_t i=16; i<24;i++)
//	  	  {
//	  		  buff_tx[i] = ((TCI>>(23-i))&(0x01));
//	  	  }
//	  for (uint8_t i=24; i<32;i++)
//	  	  {
//	  		  buff_tx[i] = ((TCD>>(31-i))&(0x01));
//	  	  }
//	  for (uint8_t i=32; i<40;i++)
//	  	  {
//	  		  buff_tx[i] = ((SUM>>(39-i))&(0x01));
//	  	  }
//	  temperature = (float)TCI + (float)(TCD/10);
//	  humidity = (float)RHI + (float)(RHD/10);
//	  char data[50];
//	  sprintf(data, "%.2f,%.2f\r\n", temperature, humidity);

	          // Gửi dữ liệu qua UART
//	          HAL_UART_Transmit(&huart2, (uint8_t *)data, strlen(data), 10);
	  HAL_UART_Transmit(&huart1,&RHI,sizeof(RHI),10);
	  HAL_UART_Transmit(&huart1,&TCI,sizeof(TCI),10);

	  }
	  if ((h_flg == 1) && (t_flg == 0)) //�?o am
	       	  {
	      	 	 h_val=0;
	       		  HD44780_Clear();
	       		  HD44780_SetCursor(0, 0);
	       		  HD44780_PrintStr("Setting");
	       		  HD44780_SetCursor(0, 1);
	       		  sprintf(strCopy, "Humidity: %d %%", RHI);
	       		  HD44780_PrintStr(strCopy);
	       		  while ((h_flg == 1) && (t_flg == 0)) {
	       		  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))) //Tang
	       		  {
	       			  for (uint8_t i = 50; i >= 1; i--)//Nhan
	       			  {
	       				  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))
	       					  break;
	       			  }
	       			  for (uint8_t i = 50; i >= 1; i--)//Nha
	       			  {
	       				  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)))
	       					  i = 50;
	       			  }
	       			  if (RHI<100) {
	  					  h_val++;
	  					  RHI++;
	  					  HD44780_Clear();
	  					  HD44780_SetCursor(0, 0);
	  					  HD44780_PrintStr("Setting");
	  					  HD44780_SetCursor(0, 1);
	  					  sprintf(strCopy, "Humidity: %d %%", RHI);
	  					  HD44780_PrintStr(strCopy);
	       			  }
	       			  else
	       			  {
	       				 HD44780_Clear();
	       				 HD44780_SetCursor(0, 0);
	       				 HD44780_PrintStr("Setting");
	       				 HD44780_SetCursor(0, 1);
	       				 sprintf(strCopy, "Humidity: %d %%", RHI);
	       				 HD44780_PrintStr(strCopy);
	       			  }
	       		  }
	       		  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))) //Giam
	       		  {
	       			  for (uint8_t i = 50; i >= 1; i--)//Nhan
	       			  {
	       			  		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
	       			  			break;
	       			  }
	       			  for (uint8_t i = 50; i >= 1; i--)//Nha
	       			  {
	       				  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)))
	       			  			i = 50;
	       			  }
	       			 if ((0<=RHI))
	       			 {
	  					  RHI--;
	  					  h_val--;
	  					  HD44780_Clear();
	  					  HD44780_SetCursor(0, 0);
	  					  HD44780_PrintStr("Setting");
	  					  HD44780_SetCursor(0, 1);
	  					  sprintf(strCopy, "Humidity: %d %%", RHI);
	  					  HD44780_PrintStr(strCopy);
	       			 }
	       			 else
	       			 {
	  					  HD44780_Clear();
	       			      HD44780_SetCursor(0, 0);
	       			      HD44780_PrintStr("Setting");
	       			      HD44780_SetCursor(0, 1);
	       			      sprintf(strCopy, "Humidity: %d %%", RHI);
	       			      HD44780_PrintStr(strCopy);
	       			 }
	       		  }

	       	  }
	       		  HD44780_Clear();
	       	  }
	       	  if ((t_flg == 1) && (h_flg == 0))//Nhiet do
	       	  {
	       		  t_val=0;
	       		  HD44780_Clear();
	       		  HD44780_SetCursor(0, 0);
	       		  HD44780_PrintStr("Setting");
	       		  HD44780_SetCursor(0, 1);
	       		  sprintf(strCopy, "Temperature: %d C ", TCI);
	       		  HD44780_PrintStr(strCopy);
	       		  while ((t_flg == 1) && (h_flg == 0)) {
	       		  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))) //Tang
	       		  {
	       			  for (uint8_t i = 50; i >= 1; i--)//Nhan
	       			  {
	       				  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))
	       		  			break;
	       			  }
	       			  for (uint8_t i = 50; i >= 1; i--)//Nha
	       			  {
	       				  if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)))
	       					  i = 50;
	       			  }
	       			  if (TCI<40)
	       			  {
	  					  t_val++;
	  					  TCI++;
	  					  HD44780_Clear();
	  					  HD44780_SetCursor(0, 0);
	  					  HD44780_PrintStr("Setting");
	  					  HD44780_SetCursor(0, 1);
	  					  sprintf(strCopy, "Temperature: %d C ", TCI);
	  					  HD44780_PrintStr(strCopy);
	       			  }
	       			  else
	       			  {
	       				 HD44780_Clear();
	       				 HD44780_SetCursor(0, 0);
	       				 HD44780_PrintStr("Setting");
	       				 HD44780_SetCursor(0, 1);
	       				 sprintf(strCopy, "Temperature: %d C ", TCI);
	       				 HD44780_PrintStr(strCopy);
	       			  }
	       		  	}
	       		  	if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))) //Giam
	       		  	{
	       		  		for (uint8_t i = 50; i >= 1; i--)//Nhan
	       		  		{
	       		  			 if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3))
	       		  			  	break;
	       		  		}
	       		  		for (uint8_t i = 50; i >= 1; i--)//Nha
	       		  		{
	       		  			if (!(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)))
	       		  			  	i = 50;
	       		  		}
	       		  		if (0<TCI)
	       		  		{
	       		  			 t_val--;
	       		  			 TCI--;
	       		  			 HD44780_Clear();
	       		  			 HD44780_SetCursor(0, 0);
	       		  			 HD44780_PrintStr("Setting");
	       		  			 HD44780_SetCursor(0, 1);
	       		  			 sprintf(strCopy, "Temperature: %d C ", TCI);
	       		  			 HD44780_PrintStr(strCopy);
	       		  		}
	       		  		else
	       		  		{
	       		  			HD44780_Clear();
	       		  			HD44780_SetCursor(0, 0);
	       		  			HD44780_PrintStr("Setting");
	       		  			HD44780_SetCursor(0, 1);
	       		  			sprintf(strCopy, "Temperature: %d C ", TCI);
	       		  			HD44780_PrintStr(strCopy);
	       		  		}

	       		  	}
	       		  }
	       		  HD44780_Clear();
	       	  	  }
	  if ((RHI <= h_val) || (TCI <= t_val))
	  {
	  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
	  	relay = 1;
	  	HAL_InitTick(TICK_INT_PRIORITY);
	  }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 400000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 71;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_8;
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
//  GPIO_InitStruct.Pin = GPIO_PIN_9;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  GPIO_InitStruct.Pin = GPIO_PIN_10;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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

//void Usart_Init()
//{
//	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
//	    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
//
//	    // Bước 2: Cấu hình chân GPIO cho TX và RX
//	    GPIOA->MODER |= GPIO_MODER_MODE9_1; // TX (PA9) - Alternate function mode
//	    GPIOA->MODER |= GPIO_MODER_MODE10_1; // RX (PA10) - Alternate function mode
//	    GPIOA->AFR[1] |= (7 << GPIO_AFRH_AFSEL9_Pos); // PA9: AF7 (UART1_TX)
//	    GPIOA->AFR[1] |= (7 << GPIO_AFRH_AFSEL10_Pos); // PA10: AF7 (UART1_RX)
//
//	    // Bước 3: Cấu hình thanh ghi UART1
//	    USART1->BRR = (SystemCoreClock / 115200); // Baudrate 9600
//	    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable TX and RX
//	    USART1->CR1 |= USART_CR1_UE; // Enable UART
//}
//void Usart_Trans(uint8_t data)
//{
//	USART1->DR = data;
//	while (!(USART1->SR & USART_SR_TC));
//}

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
