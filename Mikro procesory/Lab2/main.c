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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// PRZEPISZ!
typedef enum
{
	Waiting,
	Command,
	Argument,
	number,
	End
} FrameDetection;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// --- ISTNIEJĄCE ZMIENNE (ZOSTAW JE) ---
uint8_t znak;
uint16_t komunikat, dlugosc;

#define BUF_LEN 512

uint8_t BUF_RX[BUF_LEN];
uint8_t BUF_TX[BUF_LEN];

uint8_t empty_RX=0;
uint8_t busy_RX=0;
uint8_t empty_TX=0;
uint8_t busy_TX=0;

volatile int ld2STATE = 0;
volatile uint16_t slow = 1;
volatile uint32_t mainLoopDelay = 0;

uint8_t tempINDX = 0;
char NumArr[128];
char cmd[128];
char CMDarg[128];
float nr = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void USART_fill_buf();
void USART_send(uint8_t message[]);
void USART_send_char();
int16_t USART_getChar();
void FrameRd();


void ExecuteCommand();
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
	MX_USART2_UART_Init();
	MX_TIM11_Init();
	/* USER CODE BEGIN 2 */
	// PRZEPISZ!
	HAL_UART_Receive_IT(&huart2, &znak, 1);
	// HAL_UART_Receive_IT(&huart2, &BUF_RX[0], 1);
	// PRZEPISZ!
	USART_send("\n\n\r");
	USART_send("Witaj, jestem Nucleo STM32!!!");

	HAL_TIM_Base_Start_IT(&htim11);
	Miganie(5000000);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		if (ld2STATE == 2) {
			if (slow > 0) {
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);




				uint32_t half_period = (1000 / slow) / 2;

				HAL_Delay(half_period);
			}
		}



		if (mainLoopDelay > 0) {
			HAL_Delay(mainLoopDelay);
		}


		if (ld2STATE != 2 && mainLoopDelay == 0) {
			HAL_Delay(10);
		}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		/* USER CODE END 3 */
	}
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

	/* USER CODE BEGIN 4 */

	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
		HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
	}

	// --- Funkcja parsująca dane z bufora kołowego ---
	void FrameRd() {
		static FrameDetection detection = Waiting;

		// Pobieramy znak (jeśli jest w buforze)
		int16_t ch_int = USART_getChar();

		while(ch_int != -1) {
			char c = (char)ch_int;

			// 1. GŁÓWNY PUNKT WYKONANIA - ŚREDNIK
			if (c == ';') {
				// Wykonaj komendę TYLKO JEŚLI byliśmy w trakcie jej pobierania (stan End lub po liczbie)
				if (detection == End || detection == number || detection == Argument) {
					ExecuteCommand();
				}

				// Reset maszyny stanów po średniku
				detection = Waiting;
				memset(cmd, 0, sizeof(cmd));
				memset(CMDarg, 0, sizeof(CMDarg));
				memset(NumArr, 0, sizeof(NumArr));
				tempINDX = 0;

				// Pobierz kolejny znak i kontynuuj
				ch_int = USART_getChar();
				continue;
			}

			// Ignorowanie spacji i znaków nowej linii (ENTER)
			if (c == ' ' || c == '\r' || c == '\n') {
				ch_int = USART_getChar();
				continue;
			}

			// Maszyna stanów
			switch (detection) {
			case Waiting:

				if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
					detection = Command;
					tempINDX = 0;
					cmd[tempINDX++] = c;
				}
				break;

			case Command:
				if (c == '[') {
					cmd[tempINDX] = '\0';
					detection = Argument;
					tempINDX = 0;
				} else if (tempINDX < 100) {
					cmd[tempINDX++] = c;
				}
				break;

			case Argument:
				if (c == ']') {
					CMDarg[tempINDX] = '\0';
					detection = End;
				} else if (c == ',') {
					CMDarg[tempINDX] = '\0';
					detection = number;
					tempINDX = 0;
				} else if (tempINDX < 100) {
					CMDarg[tempINDX++] = c;
				}
				break;

			case number:
				if (c == ']') {
					NumArr[tempINDX] = '\0';
					detection = End;
				} else if (tempINDX < 100) {
					NumArr[tempINDX++] = c;
				}
				break;

			case End:

				break;

			default:
				detection = Waiting;
				break;
			}

			// Pobranie następnego znaku z bufora
			ch_int = USART_getChar();
		}
	}

	void Miganie(int x){
		__HAL_TIM_SET_COUNTER(&htim11,0);
		__HAL_TIM_SET_AUTORELOAD(&htim11,x);
		__HAL_TIM_SET_PRESCALER(&htim11,1999);

		while(__HAL_TIM_GET_COUNTER(&htim11) < x);
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	}

	void ExecuteCommand() {
		char txBuf[64]; // Bufor pomocniczy

		if (strcmp(cmd, "LED") == 0) {
			if (strcmp(CMDarg, "ON") == 0) {
				ld2STATE = 1;
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
				USART_send((uint8_t*)"OK: LED ON\r\n");
			}
			else if (strcmp(CMDarg, "OFF") == 0) {
				ld2STATE = 0;
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
				USART_send((uint8_t*)"OK: LED OFF\r\n");
			}
			else if (strcmp(CMDarg, "BLINK") == 0) {
				ld2STATE = 2;

				slow = atoi(NumArr);
				if(slow == 0) slow = 1;
				USART_send((uint8_t*)"OK: LED BLINK\r\n");
			}
			else if (strcmp(CMDarg, "DELAY") == 0) {
				mainLoopDelay = atoi(NumArr);
				USART_send((uint8_t*)"OK: LED Delay Set\r\n");
			} else{
				USART_send((uint8_t*)"blad: zly argument LED\r\n");
			}
		}
	}



	void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
		if(busy_TX != empty_TX){
			uint8_t tmp = BUF_TX[busy_TX];
			busy_TX++;
			if(busy_TX >= sizeof(BUF_TX)) busy_TX = 0;
			HAL_UART_Transmit_IT(&huart2, &tmp, 1);
		}
	}

	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
		empty_RX++;
		if(empty_RX >= sizeof(BUF_RX))	empty_RX = 0;


		if(busy_RX == empty_RX) {
			busy_RX++;
			if(busy_RX >= sizeof(BUF_RX)) busy_RX = 0;
		}

		HAL_UART_Receive_IT(&huart2, &BUF_RX[empty_RX], 1);





		FrameRd();
	}

	void USART_fill_buf(){
		uint16_t idx = empty_TX;
		for(int i = 0; i <= BUF_LEN; i++ ) {
			BUF_TX[idx] = 0x11;
			idx++;
			if(idx >= sizeof(BUF_TX)) idx = 0;
		}
	}

	void USART_send(uint8_t message[]){
		uint16_t i, idx = empty_TX;
		for(i=0; message[i] != '\0'; i++){
			BUF_TX[idx] = message[i];
			idx++;
			if(idx >= sizeof(BUF_TX)) idx = 0;
		}

		__disable_irq();
		if (busy_TX == empty_TX) {
			empty_TX = idx;
			uint8_t tmp = BUF_TX[busy_TX];
			busy_TX++;
			if(busy_TX >= sizeof(BUF_TX)) busy_TX = 0;
			HAL_UART_Transmit_IT(&huart2, &tmp, 1);
		} else {
			empty_TX = idx;
		}
		__enable_irq();
	}

	void USART_send_char(){
		uint16_t idx = empty_TX;
		__disable_irq();
		if (busy_TX == empty_TX) {
			empty_TX = idx;
			uint8_t tmp = BUF_TX[busy_TX];
			busy_TX++;
			if(busy_TX >= sizeof(BUF_TX)) busy_TX = 0;
			HAL_UART_Transmit_IT(&huart2, &tmp, 1);
		} else {
			empty_TX = idx;
		}
		__enable_irq();
	}

	int16_t USART_getChar() {
		int16_t charTMP;
		if (empty_RX != busy_RX) {
			charTMP = BUF_RX[busy_RX];
			busy_RX++;
			if (busy_RX >= BUF_LEN) busy_RX = 0;
			return charTMP;
		}
		else return -1;
	}

	void blink1(uint16_t x){
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		HAL_Delay(x);
	}

	void blink2(){
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
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
#ifdef USE_FULL_ASSERT
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
