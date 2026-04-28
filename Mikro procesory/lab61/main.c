/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body - DMA Lab (Mem2Mem, PWM, RGB logic)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
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
DMA_HandleTypeDef hdma_tim2_ch1;

UART_HandleTypeDef huart2;

DMA_HandleTypeDef hdma_memtomem_dma2_stream0;
/* USER CODE BEGIN PV */
// --- ZMIENNE DO ZADANIA 1 (Mem2Mem) ---
#define BUFFER_SIZE 32
const uint8_t srcBuffer[BUFFER_SIZE] = "Test DMA Mem2Mem - F401RE\r\n";
uint8_t dstBuffer[BUFFER_SIZE];
volatile uint8_t transferComplete = 0;

// --- ZMIENNE DO ZADANIA 2 (PWM Breathing LED) ---
#define PWM_STEPS 100
uint32_t pwm_breathing_data[PWM_STEPS * 2]; // Tablica na wartości wypełnienia

// --- ZMIENNE DO ZADANIA 3 (RGB WS2812B Logic) ---
// Symulacja bufora dla 1 diody LED (24 bity: G, R, B) + Reset
#define LED_COUNT 1
#define RESET_SLOTS 50
uint32_t rgb_dma_buffer[LED_COUNT * 24 + RESET_SLOTS];

// Wartości PWM odpowiadające '0' i '1' dla WS2812B przy ARR=104 (84MHz)
#define WS2812_0 32
#define WS2812_1 68
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void TransferCompleteCallback(DMA_HandleTypeDef *hdma);
void PrepareBreathingData(void);
void Set_LED_Color(uint8_t Green, uint8_t Red, uint8_t Blue);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Callback po zakończeniu transferu Mem2Mem
void TransferCompleteCallback(DMA_HandleTypeDef *hdma) {
    transferComplete = 1;
}

// Funkcja przygotowująca dane dla "oddychania" diody
void PrepareBreathingData(void) {
    for (int i = 0; i < PWM_STEPS; i++) {
        // Liniowe narastanie
        pwm_breathing_data[i] = i * (1000 / PWM_STEPS);
        // Liniowe opadanie
        pwm_breathing_data[PWM_STEPS + i] = 1000 - (i * (1000 / PWM_STEPS));
    }
}

// Funkcja ustawiająca kolor diody (logika RGB do bufora DMA)
void Set_LED_Color(uint8_t Green, uint8_t Red, uint8_t Blue) {
    uint32_t color = (Green << 16) | (Red << 8) | Blue;

    for (int i = 0; i < 24; i++) {
        if (color & (1 << (23 - i))) {
            rgb_dma_buffer[i] = WS2812_1;
        } else {
            rgb_dma_buffer[i] = WS2812_0;
        }
    }
    // Reszta bufora to zera (sygnał RESET dla paska LED)
    for (int i = 24; i < (LED_COUNT * 24 + RESET_SLOTS); i++) {
        rgb_dma_buffer[i] = 0;
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  // ==========================================
  // ZADANIE 1: MEMORY TO MEMORY DMA TRANSFER
  // ==========================================
  char msg[64];
  sprintf(msg, "\r\nStart DMA Mem2Mem...\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

  // Rejestracja callbacka (funkcja wywoływana po zakończeniu transferu)
  HAL_DMA_RegisterCallback(&hdma_memtomem_dma2_stream0, HAL_DMA_XFER_CPLT_CB_ID, TransferCompleteCallback);

  // Start DMA (z src do dst)
  // UWAGA: W F401RE Mem2Mem musi być na DMA2
  if (HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream0, (uint32_t)srcBuffer, (uint32_t)dstBuffer, BUFFER_SIZE) != HAL_OK) {
      Error_Handler();
  }

  // Czekanie na flagę (w prawdziwym zastosowaniu CPU robiłby tu co innego)
  while (transferComplete == 0) {
      // Symulacja pracy CPU
      __NOP();
  }

  // Weryfikacja
  if (strcmp((char*)srcBuffer, (char*)dstBuffer) == 0) {
      sprintf(msg, "Sukces Mem2Mem: %s", dstBuffer);
  } else {
      sprintf(msg, "Blad transferu DMA!\r\n");
  }
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);


  // ==========================================
  // ZADANIE 2: PWM + DMA (Breathing LED PA5)
  // ==========================================
  // Ręczna korekta ustawień Timera 2, aby uzyskać 1kHz PWM przy zegarze 84MHz
  // Domyślny CubeMX init ustawił Prescaler=0 i Period=MAX, co jest złe dla LED.
  // Ustawiamy: Prescaler = 83 (Zegar Timera = 1MHz), Period = 999 (PWM = 1kHz)
  __HAL_TIM_SET_PRESCALER(&htim2, 83);
  __HAL_TIM_SET_AUTORELOAD(&htim2, 999);

  PrepareBreathingData();

  // Uruchomienie PWM z DMA w trybie CIRCULAR (pętla nieskończona)
  // TIM2 CH1 steruje pinem PA5 (Dioda LD2 na Nucleo)
  if(HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*)pwm_breathing_data, PWM_STEPS * 2) != HAL_OK)
  {
	  Error_Handler();
  }

  sprintf(msg, "PWM DMA Started on PA5 (LD2)...\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);

  // ==========================================
  // ZADANIE 3: PROPOZYCJA RGB (Logika)
  // ==========================================
  // Tylko przygotowanie danych (aby zobaczyć w Debuggerze w tablicy rgb_dma_buffer)
  Set_LED_Color(255, 0, 0); // Zielony

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // CPU jest wolne! Dioda mruga sama dzięki DMA.
    // Możemy np. wysyłać logi co 2s
    HAL_Delay(2000);
    HAL_UART_Transmit(&huart2, (uint8_t*)"CPU is idle, DMA controls LED...\r\n", 34, 100);

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
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

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
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  * hdma_memtomem_dma2_stream0
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma2_stream0 on DMA2_Stream0 */
  hdma_memtomem_dma2_stream0.Instance = DMA2_Stream0;
  hdma_memtomem_dma2_stream0.Init.Channel = DMA_CHANNEL_0;
  hdma_memtomem_dma2_stream0.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma2_stream0.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma2_stream0.Init.MemInc = DMA_MINC_ENABLE;
  hdma_memtomem_dma2_stream0.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_memtomem_dma2_stream0.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_memtomem_dma2_stream0.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma2_stream0.Init.Priority = DMA_PRIORITY_LOW;
  hdma_memtomem_dma2_stream0.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  hdma_memtomem_dma2_stream0.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  hdma_memtomem_dma2_stream0.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_memtomem_dma2_stream0.Init.PeriphBurst = DMA_PBURST_SINGLE;
  if (HAL_DMA_Init(&hdma_memtomem_dma2_stream0) != HAL_OK)
  {
    Error_Handler( );
  }

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration (DODANE DLA MEM2MEM) */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

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
