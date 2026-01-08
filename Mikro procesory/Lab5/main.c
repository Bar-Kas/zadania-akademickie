/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define NUM_DEV 4
uint8_t bufferCol[NUM_DEV*8];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

const uint8_t charData[][8] = {
  		{0x00, 0x1c, 0x22, 0x22, 0x22, 0x22, 0x1c, 0x00},  // 0
  		{0x08, 0x0c, 0x08, 0x08, 0x08, 0x08, 0x3e, 0x00},  // 1
  		{0x1c, 0x22, 0x20, 0x18, 0x04, 0x02, 0x3e, 0x00},  // 2
  		{0x1c, 0x22, 0x20, 0x1c, 0x20, 0x22, 0x1c, 0x00},  // 3
  		{0x20, 0x30, 0x28, 0x24, 0x7c, 0x20, 0x20, 0x00},  // 4
  		{0x3e, 0x02, 0x1e, 0x20, 0x20, 0x20, 0x1e, 0x00},  // 5
  		{0x18, 0x04, 0x02, 0x1e, 0x22, 0x22, 0x1c, 0x00},  // 6
  		{0x3e, 0x20, 0x10, 0x08, 0x04, 0x04, 0x04, 0x00},  // 7
  		{0x1c, 0x22, 0x22, 0x1c, 0x22, 0x22, 0x1c, 0x00},  // 8
  		{0x1c, 0x22, 0x22, 0x3c, 0x20, 0x10, 0x0c, 0x00},  // 9
  };
void clearDisplay (void)
{
	for (int i=0; i<NUM_DEV*8-1; i++)
	{
		bufferCol[i] = 0;
	}
	flushBuffer();
}


const uint8_t data[] =
{
0x00, 0x22, 0x49, 0x49, 0x49, 0x49, 0x36, 0x00
};
/* U
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void max7219_cmd (uint8_t Addr, uint8_t data)
{
	uint16_t writeData = (Addr<<8)|data;
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);  // enable slave
	for (int i=0; i<NUM_DEV; i++)
	{
		HAL_SPI_Transmit(&hspi, (uint8_t *)&writeData, 1, 100);
	}
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);  // disable slave
}

void max7219_write (uint8_t Addr, uint8_t data)
{
	uint16_t writeData = (Addr<<8)|data;
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);  // enable slave
	HAL_SPI_Transmit(&hspi, (uint8_t *)&writeData, 1, 100);
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);  // disable slave
}

void matrixInit (void)
{
	max7219_write(0x09, 0);  // no decoding
	max7219_write(0x0a, 0x01);  // 3/32 intensity
	max7219_write(0x0B, 0x07);  // scan all 7 columns
	max7219_write(0x0C, 0x01);  // normal operation
	max7219_write(0x0F, 0);     // No display test
}

void matrixData(int num)
{
	for (int i=1; i<=8; i++)
	{
		max7219_write(i, charData[num][8-i]);
	}

}

void max_write (int row, uint8_t data)
{
	int devTarget = (row - 1) / 8;  // find out which is the actual max, where we need to write the data
	int offset = devTarget * 8;  // The offset of the start byte for the devTarget in the buffer
	uint16_t writeData = 0;

	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0);  // Select the slave
			for (int dev = 0; dev < NUM_DEV; dev++)   // for loop for all the max connected
			{
				if (dev == devTarget)  // if this the target
				{
					writeData = ((row - offset)<<8)|data;  // send the column number and the data byte
					HAL_SPI_Transmit(&hspi, (uint8_t *)&writeData, 1, 1000);
				}
				else
				{
					writeData = 0;  // else send NOOP
					HAL_SPI_Transmit(&hspi, (uint8_t *)&writeData, 1, 1000);
				}
			}
			HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1);  // disable the slave

}

void flushBuffer (void)
{
	uint8_t bufferRow[NUM_DEV*8] = {0};  // buffer to store data column wise

	/* Convert Cols to Rows */
	for (int i=0; i<NUM_DEV*8; i++)  // for loop for all the bytes
	{
		int dev = i/8;  // 0,1,2,3..  // keep track of which max is being written
		for (int j=0; j<8; j++)  // for loop to extract bits
		{
			if ((bufferCol[i])&(1<<(j)))  // if the bit is 1 // start extracting from the 0th bit of C0
			{
				bufferRow[j+(8*dev)] |= (1<<(7-(i-(8*dev))));  // start writing it from the 7th bit of R0
			}
		}
	}


	for (int row=1; row<=(NUM_DEV*8); row++)  // write the column data into the columns
	{
		max_write(row, bufferRow[row-1]);
	}
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
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  matrixInit();
    clearDisplay();
    int indx=0;
    for (int i=5; i<13; i++)
    {
      bufferCol[i] = data[7-indx];
      indx++;
    }
    flushBuffer();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CS_Pin */
  GPIO_InitStruct.Pin = CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  * where the assert_param error has occurred.
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
