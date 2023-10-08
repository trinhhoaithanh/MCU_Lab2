/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "software_timer.h"
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
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void display7SEG ( int num ) {
	uint8_t led7Segment [10] = {
	0b00111111 , // 0
	0b00000110 , // 1
	0b01011011 , // 2
	0b01001111 , // 3
	0b01100110 , // 4
	0b01101101 , // 5
	0b01111101 , // 6
	0b00000111 , // 7
	0b01111111 , // 8
	0b01101111 // 9
	};

	HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_0 , !(( led7Segment [ num
	] >> 0) & 1) ) ;
	HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_1 , !(( led7Segment [ num
	] >> 1) & 1) ) ;
	HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_2 , !(( led7Segment [ num
	] >> 2) & 1) ) ;
	HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_3 , !(( led7Segment [ num
	] >> 3) & 1) ) ;
	HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_4 , !(( led7Segment [ num
	] >> 4) & 1) ) ;
	HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_5 , !(( led7Segment [ num
	] >> 5) & 1) ) ;
	HAL_GPIO_WritePin ( GPIOB , GPIO_PIN_6 , !(( led7Segment [ num
	] >> 6) & 1) ) ;

}

const int MAX_LED = 4;
int index_led = 0;
int led_buffer [4] = {1 , 2 , 3 , 4};

void update7SEG(int index){
	switch (index){
		case 0:
			// Display the first 7 SEG with led_buffer [0]
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, RESET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);

			display7SEG(led_buffer[index]);

			break;
		case 1:
			// Display the second 7 SEG with led_buffer [1]
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, RESET);
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN3_Pin, SET);
			display7SEG(led_buffer[index]);
			break ;
		case 2:
			// Display the third 7 SEG with led_buffer [2]
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN2_Pin, RESET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN3_Pin, SET);
			display7SEG(led_buffer[index]);
			break ;
		case 3:
			// Display the forth 7 SEG with led_buffer [3]
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN2_Pin, SET);
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN3_Pin, RESET);
			display7SEG(led_buffer[index]);
			break ;
		default :
			break ;
	}
}


int hour = 15, minute = 8, second = 50;
void updateClockBuffer (){
	led_buffer[0]=hour/10;
	led_buffer[1]=hour%10;
	led_buffer[2]=minute/10;
	led_buffer[3]=minute%10;
}

const int MAX_LED_MATRIX = 8;
const int MAX_SHIFT = 14;
static int index_led_matrix = 0;
static int state = 0;

static uint8_t rowBuffer[14][8] = {
{0x00,0xfc,0x32,0x33,0x33,0x32,0xfc,0x00},
{0xfc,0x32,0x33,0x33,0x32,0xfc,0x00,0x00},
{0x32,0x33,0x33,0x32,0xfc,0x00,0x00,0x00},
{0x33,0x33,0x32,0xfc,0x00,0x00,0x00,0x00},
{0x33,0x32,0xfc,0x00,0x00,0x00,0x00,0x00},
{0x32,0xfc,0x00,0x00,0x00,0x00,0x00,0x00},
{0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc},
{0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0x32},
{0x00,0x00,0x00,0x00,0x00,0xfc,0x32,0x33},
{0x00,0x00,0x00,0x00,0xfc,0x32,0x33,0x33},
{0x00,0x00,0x00,0xfc,0x32,0x33,0x33,0x32},
{0x00,0x00,0xfc,0x32,0x33,0x33,0x32,0xfc},
{0x00,0xfc,0x32,0x33,0x33,0x32,0xfc,0x00}};

static uint8_t colBuffer[14][8] = {
{0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x00},
{0x04,0x08,0x10,0x20,0x40,0x80,0x00,0x00},
{0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00},
{0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00},
{0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00},
{0x20,0x40,0x00,0x00,0x00,0x00,0x00,0x00},
{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01},
{0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02},
{0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04},
{0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x08},
{0x00,0x00,0x00,0x01,0x02,0x04,0x8,0x10},
{0x00,0x00,0x01,0x02,0x04,0x8,0x10,0x20},
{0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x00}};

void displayRow (int state, int num){
	  HAL_GPIO_WritePin(ROW0_GPIO_Port,ROW0_Pin, ((rowBuffer[state][num]>>0)&0x01));
	  HAL_GPIO_WritePin(ROW1_GPIO_Port,ROW1_Pin, ((rowBuffer[state][num]>>1)&0x01));
	  HAL_GPIO_WritePin(ROW2_GPIO_Port,ROW2_Pin, ((rowBuffer[state][num]>>2)&0x01));
	  HAL_GPIO_WritePin(ROW3_GPIO_Port,ROW3_Pin, ((rowBuffer[state][num]>>3)&0x01));
	  HAL_GPIO_WritePin(ROW4_GPIO_Port,ROW4_Pin, ((rowBuffer[state][num]>>4)&0x01));
	  HAL_GPIO_WritePin(ROW5_GPIO_Port,ROW5_Pin, ((rowBuffer[state][num]>>5)&0x01));
	  HAL_GPIO_WritePin(ROW6_GPIO_Port,ROW6_Pin, ((rowBuffer[state][num]>>6)&0x01));
	  HAL_GPIO_WritePin(ROW7_GPIO_Port,ROW7_Pin, ((rowBuffer[state][num]>>7)&0x01));
}

void displayCol (int state, int num){
	  HAL_GPIO_WritePin(ENM0_GPIO_Port, ENM0_Pin, ((colBuffer[state][num]>>0)&0x01));
	  HAL_GPIO_WritePin(ENM1_GPIO_Port, ENM1_Pin, ((colBuffer[state][num]>>1)&0x01));
	  HAL_GPIO_WritePin(ENM2_GPIO_Port, ENM2_Pin, ((colBuffer[state][num]>>2)&0x01));
	  HAL_GPIO_WritePin(ENM3_GPIO_Port, ENM3_Pin, ((colBuffer[state][num]>>3)&0x01));
	  HAL_GPIO_WritePin(ENM4_GPIO_Port, ENM4_Pin, ((colBuffer[state][num]>>4)&0x01));
	  HAL_GPIO_WritePin(ENM5_GPIO_Port, ENM5_Pin, ((colBuffer[state][num]>>5)&0x01));
	  HAL_GPIO_WritePin(ENM6_GPIO_Port, ENM6_Pin, ((colBuffer[state][num]>>6)&0x01));
	  HAL_GPIO_WritePin(ENM7_GPIO_Port, ENM7_Pin, ((colBuffer[state][num]>>7)&0x01));
}

void updateLedMatrix(int state, int num){
	displayCol(state, num);
	displayRow(state, num);
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  setTimer1(50);
  int status = 1;

  setTimer2(100);

  setTimer3(50);

  setTimer4(25);

  setTimer5(100);

  setTimer6(100);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(timer1_flag == 1){
//		  //TODO EX1
		  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);

		  switch(status){
			  case 1:
				  HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, RESET);
				  HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
				  HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
				  HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
				  display7SEG(status);
				  status = 2;
				  break;
			  case 2:
				  HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
				  HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, RESET);
				  HAL_GPIO_WritePin(EN0_GPIO_Port, EN2_Pin, SET);
				  HAL_GPIO_WritePin(EN1_GPIO_Port, EN3_Pin, SET);
				  display7SEG(status);
				  status = 3;
				  break;

			case 3:
				  HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
				  HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
				  HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, RESET);
				  HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, SET);
				  display7SEG(status);
				  status = 0;
				  break;
			case 0:
				  HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
				  HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
				  HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, SET);
				  HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, RESET);
				  display7SEG(status);
				  status = 1;
				  break;
			  default:
				  break;
			  }

		  setTimer1(50);
	  }

	  if(timer2_flag == 1){
	  		  //TODO EX2
		  HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
		  setTimer2(100);
	  }

	  if(timer4_flag == 1){
		  //TODO EX4
		  update7SEG(index_led);
		  index_led++;
		  if(index_led >= 4){
			  index_led = 0;
		  }
		  setTimer4(25);
	  }

	  if(timer5_flag == 1){
//		  TODO EX7
		  second++;
		  if(second>=60){
			  second = 0;
			  minute++;
		  }
		  if(minute>=60){
			  minute = 0;
			  hour++;
		  }
		  if(hour>=24){
			  hour = 0;
		  }
		  updateClockBuffer();
		  setTimer5(100);
	  }

	  if(timer6_flag == 1){
		updateLedMatrix(state, index_led_matrix);
		  index_led_matrix = (index_led_matrix+1);
		  if (index_led_matrix == MAX_LED_MATRIX){
			  index_led_matrix = 0;
			  state = state + 1;
			  if (state == MAX_SHIFT)
				  state = 0;
		  }
		  setTimer6(1);
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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : ENM0_Pin ENM1_Pin DOT_Pin LED_RED_Pin
                           EN0_Pin EN1_Pin EN2_Pin EN3_Pin
                           ENM2_Pin ENM3_Pin ENM4_Pin ENM5_Pin
                           ENM6_Pin ENM7_Pin */
  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|DOT_Pin|LED_RED_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG0_Pin SEG1_Pin SEG2_Pin ROW2_Pin
                           ROW3_Pin ROW4_Pin ROW5_Pin ROW6_Pin
                           ROW7_Pin SEG3_Pin SEG4_Pin SEG5_Pin
                           SEG6_Pin ROW0_Pin ROW1_Pin */
  GPIO_InitStruct.Pin = SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim)
{

	timerRun();
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
