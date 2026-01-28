/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* Struktura danych pomiarowych zgodna z dokumentacją */
typedef struct __attribute__((packed)){
	uint32_t seq_num;
	uint32_t timestamp;
	int16_t accel[3];
	int16_t gyro[3];
} MeasurementData;

/* Stany parsera protokołu */
typedef enum{
	STATE_WAIT_SOF,
	STATE_ADDR,
	STATE_CMD,
	STATE_DATA,
	STATE_CRC,
	STATE_WAIT_CR,
	STATE_WAIT_LF
} ParserState;

/* Stany inicjalizacji MPU6500 */
typedef enum {
    MPU_INIT_STATE_IDLE,
    MPU_INIT_STATE_START,
    MPU_INIT_STATE_WAIT_PWR,
    MPU_INIT_STATE_WAKEUP,
    MPU_INIT_STATE_WAIT_CONFIG,
    MPU_INIT_STATE_CONFIG,
    MPU_INIT_STATE_READY
} MPU_InitState;

typedef struct {
	ParserState state;
	char buffer[128];
	uint8_t index;
} ProtocolParser;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MY_DEV_ID "ST"
#define HISTORY_SIZE 1000

/* Adresy rejestrów MPU6500 */
#define MPU_ADDR 0xD0 
#define PWR_MGMT_1_REG 0x6B
#define ACCEL_CONFIG_REG 0x1C
#define GYRO_CONFIG_REG 0x1B

#define USART_TXBUF_LEN 1512
#define USART_RXBUF_LEN 128

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* Bufory UART */
uint8_t USART_TxBuf[USART_TXBUF_LEN];
uint8_t USART_RxBuf[USART_RXBUF_LEN];

__IO int USART_TX_Empty=0;
__IO int USART_TX_Busy=0;
__IO int USART_RX_Empty=0;
__IO int USART_RX_Busy=0;

/* Bufor cykliczny pomiarów  */
MeasurementData history_buffer[HISTORY_SIZE];
__IO uint32_t head_idx = 0;
uint32_t current_seq = 0;

/* Zmienne sterujące akwizycją */
bool acquisition_running = false;
uint32_t acquisition_interval = 1000; // Domyślnie 1000ms
uint32_t last_acq_time = 0;

char last_sender_id[3] = "PC";

/* Bufor na surowe dane z I2C (14 bajtów: Accel, Temp, Gyro) */
uint8_t mpu_raw_data[14];

ProtocolParser parser = {.state = STATE_WAIT_SOF, .index =0};

MPU_InitState mpu_init_state = MPU_INIT_STATE_START;
uint32_t mpu_timer = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

uint8_t MX_USART2_UART_kbhit(void);
int16_t MX_USART2_UART_getchar(void);
uint8_t MX_USART2_UART_getline(char *buf);
void MX_USART2_UART_fsend(char* format,...);

void MPU6500_Maintain(void);
void MPU6500_StartRead(void);

void ProcessCommand(char* frame,int len);
uint16_t CalcCRC16(const char* data, int len);
void HexToBin(const char* hex, uint8_t* bin, int len);
void BinToHex(const uint8_t* bin, char* hex, int len);
bool IsValidHex(const char* str, int len);
void SendResponse(const char* cmd_code, const char* payload);
void FSM_ProcessByte(uint8_t byte);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* --- Implementacja UART (Ring Buffer) --- */

uint8_t MX_USART2_UART_kbhit(){
	if(USART_RX_Empty == USART_RX_Busy){
		return 0;
	}else{
		return 1;
	}
}

int16_t MX_USART2_UART_getchar(){
	int16_t temp;
	if(USART_RX_Empty != USART_RX_Busy){
		temp=USART_RxBuf[USART_RX_Busy];
		USART_RX_Busy++;
		if(USART_RX_Busy >= USART_RXBUF_LEN){
			USART_RX_Busy=0;
		}
		return temp;
	}else{
		return -1;
	}
}

uint8_t MX_USART2_UART_getline(char* buf){
	static uint8_t bf[128];
	static uint8_t idx=0;
	int i;
	uint8_t ret;

	while(MX_USART2_UART_kbhit()){
		bf[idx] = MX_USART2_UART_getchar();

		if(((bf[idx]=='\r')||(bf[idx]=='\n'))){
			bf[idx] = 0;
			for(i=0; i<=idx; i++){
				buf[i]=bf[i];
			}
			ret=idx;
			idx=0;
			return ret;
		}else{
			idx++;
			if(idx >=128) {
				idx=0;
			}
		}
	}
    return 0;
}

void MX_USART2_UART_fsend(char* format, ...){
	char tmp_rs[128];
	int i;
	__IO int idx;
	va_list arglist;
    va_start(arglist,format);
    vsprintf(tmp_rs,format,arglist);
    va_end(arglist);

    idx=USART_TX_Empty;
    for(i=0;i<strlen(tmp_rs);i++){
    	USART_TxBuf[idx]=tmp_rs[i];
    	idx++;
    	if(idx >= USART_TXBUF_LEN)idx=0;
    }

    __disable_irq();
    if((USART_TX_Empty==USART_TX_Busy)&&(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TXE)==SET)){
    	USART_TX_Empty=idx;
    	uint8_t tmp=USART_TxBuf[USART_TX_Busy];
    	USART_TX_Busy++;
    	if(USART_TX_Busy >= USART_TXBUF_LEN)USART_TX_Busy=0;
    	HAL_UART_Transmit_IT(&huart2, &tmp, 1);
    }else{
    	USART_TX_Empty=idx;
    }
    __enable_irq();
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart2){
		if(USART_TX_Empty!=USART_TX_Busy){
			uint8_t tmp=USART_TxBuf[USART_TX_Busy];

			USART_TX_Busy++;
			if(USART_TX_Busy >= USART_TXBUF_LEN) USART_TX_Busy=0;

            HAL_UART_Transmit_IT(&huart2, &tmp, 1);
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart2){
		USART_RX_Empty++;

		if(USART_RX_Empty>=USART_RXBUF_LEN) USART_RX_Empty=0;

		HAL_UART_Receive_IT(&huart2,&USART_RxBuf[USART_RX_Empty],1);
	}
}

/* --- Implementacja MPU6500 (Non-blocking I2C) --- */

/**
Maszyna stanów inicjalizacji MPU6500.
Wybudza układ i przygotowuje do pracy nie blokując CPU.
 */
void MPU6500_Maintain(void) {
    static uint8_t i2c_data = 0;

    switch (mpu_init_state) {
        case MPU_INIT_STATE_IDLE:
            break;

        case MPU_INIT_STATE_START:
            mpu_timer = HAL_GetTick();
            mpu_init_state = MPU_INIT_STATE_WAIT_PWR;
            break;

        case MPU_INIT_STATE_WAIT_PWR:
            // Czekaj 100ms na stabilizację zasilania
            if (HAL_GetTick() - mpu_timer > 100) {
                mpu_init_state = MPU_INIT_STATE_WAKEUP;
            }
            break;

        case MPU_INIT_STATE_WAKEUP:
            if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY) {
                // Wybudzenie: PWR_MGMT_1 = 0x00
                i2c_data = 0x00;
                HAL_I2C_Mem_Write_IT(&hi2c1, MPU_ADDR, PWR_MGMT_1_REG, 1, &i2c_data, 1);
                mpu_timer = HAL_GetTick();
                mpu_init_state = MPU_INIT_STATE_WAIT_CONFIG;
            }
            break;

        case MPU_INIT_STATE_WAIT_CONFIG:
            // Czekaj 10ms po wybudzeniu
            if (HAL_GetTick() - mpu_timer > 10) {
                mpu_init_state = MPU_INIT_STATE_CONFIG;
            }
            break;

        case MPU_INIT_STATE_CONFIG:
             if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY) {
                 mpu_init_state = MPU_INIT_STATE_READY;
             }
             break;

        case MPU_INIT_STATE_READY:
            // Urządzenie gotowe do pracy
            mpu_init_state = MPU_INIT_STATE_IDLE;
            break;
    }
}

void MPU6500_StartRead(void) {
	// Rozpocznij odczyt 14 bajtów od rejestru ACCEL_XOUT_H (0x3B)
    //  - Funkcje nie blokujące
	if (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY) {
		uint8_t reg = 0x3B;
		HAL_I2C_Mem_Read_IT(&hi2c1, MPU_ADDR, reg, 1, mpu_raw_data, 14);
	}
}

/* Callback wywoływany po zakończeniu transmisji I2C (odebranie danych) */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
	if (hi2c->Instance == I2C1){
		// Zapis do bufora cyklicznego
		MeasurementData *m = &history_buffer[head_idx];

		m->seq_num = current_seq++;
		m->timestamp = HAL_GetTick();

		// Konwersja surowych danych (Big Endian z czujnika na Little Endian CPU)
		m->accel[0] = (int16_t)((mpu_raw_data[0] << 8) | mpu_raw_data[1]);
		m->accel[1] = (int16_t)((mpu_raw_data[2] << 8) | mpu_raw_data[3]);
		m->accel[2] = (int16_t)((mpu_raw_data[4] << 8) | mpu_raw_data[5]);

		m->gyro[0] = (int16_t)((mpu_raw_data[8] << 8) | mpu_raw_data[9]);
		m->gyro[1] = (int16_t)((mpu_raw_data[10] << 8) | mpu_raw_data[11]);
		m->gyro[2] = (int16_t)((mpu_raw_data[12] << 8) | mpu_raw_data[13]);

		head_idx++;
		if (head_idx >= HISTORY_SIZE) {
			head_idx = 0;
		}
	}
}

/* --- Logika Protokołu --- */

void FSM_ProcessByte(uint8_t byte){
	if (parser.index >= 127){
		parser.state = STATE_WAIT_SOF;
		parser.index = 0;
	}

	if (byte == '$'){
		parser.index = 0;
		parser.buffer[parser.index++] = byte;
		parser.state = STATE_ADDR;
        return;
	}

	switch (parser.state){
		case STATE_WAIT_SOF:
			break;

		case STATE_ADDR:
			parser.buffer[parser.index++] =  byte;
			if (parser.index == 5){
				parser.state = STATE_CMD;
			}
			break;

		case STATE_CMD:
			parser.buffer[parser.index++] =  byte;
			if (parser.index == 7){
				parser.state = STATE_DATA;
			}
			break;

		case STATE_DATA:
			parser.buffer[parser.index++] =  byte;
			if (byte == ':'){
				parser.state = STATE_CRC;
			}
			break;

		case STATE_CRC:
			parser.buffer[parser.index++] = byte;
			// CRC ma zawsze 4 znaki Hex
			if(parser.index >= 5 && parser.buffer[parser.index - 5]==':'){
				parser.state = STATE_WAIT_CR;
			}
			break;

		case STATE_WAIT_CR:
			if (byte == '\r'){
				parser.state = STATE_WAIT_LF;
			} else {
				parser.state = STATE_WAIT_SOF;
			}
			break;

		case STATE_WAIT_LF:
			if(byte == '\n'){
				parser.buffer[parser.index] = '\0';
				ProcessCommand(parser.buffer, parser.index);
				parser.state = STATE_WAIT_SOF;
				parser.index = 0;
			} else{
				parser.state = STATE_WAIT_SOF;
			}
			break;

		default:
			parser.state = STATE_WAIT_SOF;
			break;

	}
}

/* Helpers */
bool IsValidHex(const char* str, int len) {
    for (int i = 0; i < len; i++) {
        char c = str[i];
        bool isDigit = (c >= '0' && c <= '9');
        bool isLetter = (c >= 'A' && c <= 'F');
        if (!(isDigit || isLetter)) {
        	return false;
        }
    }
    return true;
}

uint16_t CalcCRC16(const char* data, int len){
    // CRC-16-CCITT False, Poly 0x1021, Init 0xFFFF
	uint16_t crc = 0xFFFF;
	for (int i = 0; i < len; i++){
		crc ^= (uint16_t)data[i] << 8;
		for (int j = 0; j < 8; j++){
			if (crc & 0x8000) crc = (crc << 1) ^ 0x1021;
			else crc <<=1;
		}
	}
	return crc;
}

void HexToBin(const char* hex, uint8_t* bin, int len){
	char temp[3] = {0};
	for(int i=0; i<len; i++){
		temp[0] = hex[i*2];
		temp[1] = hex[i*2+1];
		bin[i] = (uint8_t)strtol(temp,NULL,16);
	}
}

void BinToHex(const uint8_t* bin,char* hex , int len){
	for(int i=0; i<len; i++){
		sprintf(hex + (i*2), "%02X",bin[i]);
	}
}

void SendResponse(const char* cmd_code, const char* payload){
	char frame_buf[128];
	sprintf(frame_buf, "$%s%s%s%s", MY_DEV_ID, last_sender_id, cmd_code, payload ? payload : "");

	// Oblicz CRC od pierwszego znaku po $ do : włącznie
	uint16_t crc = CalcCRC16(&frame_buf[1], strlen(&frame_buf[1]));

	uint8_t crc_bytes[2] = {crc & 0xFF, (crc >> 8) & 0xFF}; // Little Endian CRC
	char crc_str[5];
	BinToHex(crc_bytes, crc_str, 2);

	MX_USART2_UART_fsend("%s:%s\r\n", frame_buf, crc_str);
}

void ProcessCommand(char* frame, int len){
	// Weryfikacja podstawowa
	if (len < 12) return;
	if(frame[0] != '$') return;

	// Sprawdzenie czy adresat to ST
	if(strncmp(&frame[3], MY_DEV_ID,2) != 0){
		return;
	}

	// Zapisz ID nadawcy (do odpowiedzi)
	last_sender_id[0]=frame[1];
	last_sender_id[1]=frame[2];
	last_sender_id[2]='\0';

	// Znajdź separator CRC
	char* sep_ptr = strchr(frame, ':');
	if (sep_ptr == NULL) return;

	// Weryfikacja CRC
	uint16_t calced_crc = CalcCRC16(&frame[1], (sep_ptr - &frame[1]) + 1);

	char crc_str_rec[5];
	strncpy(crc_str_rec, sep_ptr + 1, 4);
	crc_str_rec[4] = 0;

	uint8_t crc_bytes[2];
	HexToBin(crc_str_rec, crc_bytes, 2);
	uint16_t recived_crc = (uint16_t)crc_bytes[0] | ((uint16_t)crc_bytes[1] << 8);

	if (calced_crc != recived_crc){
		SendResponse("E1", "01"); // ERR CRC
		return;
	}

	// Ekstrakcja komendy
	char cmd_str[3];
	strncpy(cmd_str,&frame[5],2);
	cmd_str[2]=0;

	if(!IsValidHex(cmd_str, 2)){
		SendResponse("E1", "02"); // ERR UNKNOWN
		return;
	}

	uint8_t cmd = (uint8_t)strtol(cmd_str,NULL,16);
	char * data_ptr = &frame[7]; // Dane zaczynają się po komendzie

	/* Obsługa komend zgodnie z tabelą */
	switch (cmd) {
		case 0x00: // SYS_PING
			SendResponse("E0", "00");
			break;

		case 0x01: // SYS_RESET
			SendResponse("E0", "01");
			while(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET) {}
			HAL_NVIC_SystemReset();
			break;

		case 0x02: // SYS_STATUS
        {
			uint32_t temp_status = 0;
            char temp_hex[9];

			if (HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_NONE) {
				temp_status |= 0x04; // ERR I2C mapping to bit
			}
			if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_ORE)) {
				temp_status |= 0x02;
				__HAL_UART_CLEAR_OREFLAG(&huart2);
			}

			BinToHex((uint8_t*)&temp_status, temp_hex, 4);
			SendResponse("E0", temp_hex);
			break;
        }

		case 0x10: // ACQ_SET_INT
        {
			uint8_t time_bytes[4];
			HexToBin(data_ptr, time_bytes, 4);
			// Little Endian [cite: 21]
			uint32_t new_interval = time_bytes[0] | (time_bytes[1] << 8) | (time_bytes[2] << 16) | (time_bytes[3] << 24);
			if (new_interval == 0 ) {
				SendResponse("E1", "03"); // ERR PARAM
			} else{
				acquisition_interval = new_interval;
				SendResponse("E0", "10");
			}
			break;
        }

		case 0x11: // ACQ_GET_INT
        {
			char time_hex[9];
			BinToHex((uint8_t*)&acquisition_interval, time_hex, 4);
			char payload[12];
			sprintf(payload ,"11%s", time_hex); // Zwraca CMD_ID + TIME
			SendResponse("E0", payload);
			break;
        }

		case 0x12: // ACQ_START
			acquisition_running = true;
			SendResponse("E0", "12");
			break;

		case 0x13: // ACQ_STOP
			acquisition_running = false;
			SendResponse("E0", "13");
			break;

		case 0x20: // DAT_GET_CUR
        {
			MeasurementData snapshot;
			__disable_irq(); // Sekcja krytyczna dla odczytu bufora
            uint32_t current_idx = head_idx;
            if(current_idx == 0) current_idx = HISTORY_SIZE - 1;
            else current_idx--;
			snapshot = history_buffer[current_idx];
			__enable_irq();

			char data_hex[41];
			BinToHex((uint8_t*)&snapshot, data_hex, 20); // 20 bajtów struktury
			SendResponse("E2", data_hex);
			break;
        }

		case 0x21: // DAT_GET_ARC
        {
			uint8_t idx_bytes[4];
			HexToBin(data_ptr, idx_bytes, 4);
			uint32_t req_seq = idx_bytes[0] | (idx_bytes[1]<<8) | (idx_bytes[2]<<16) | (idx_bytes[3]<<24);

			MeasurementData snapshot;
			bool found = false;

			// Proste przeszukiwanie bufora (wystarczające dla 1000 elementów)
			for(int i=0; i<HISTORY_SIZE; i++){
				if(history_buffer[i].seq_num == req_seq){
					snapshot = history_buffer[i];
					found = true;
                    break;
				}
			}

			if (found) {
				char data_hex[41];
				BinToHex((uint8_t*)&snapshot, data_hex, 20);
				SendResponse("E2", data_hex);
			} else {
				SendResponse("E1", "05"); // ERR OVERWRITTEN [cite: 44]
			}
			break;
        }

		default:
			SendResponse("E1", "02"); // ERR UNKNOWN
			break;
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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  // Uruchomienie odbioru UART w trybie przerwań
  HAL_UART_Receive_IT(&huart2, &USART_RxBuf[0], 1);

  // Inicjalizacja stanu MPU
  mpu_init_state = MPU_INIT_STATE_START;

  // Inicjalizacja czasu ostatniej akwizycji
  last_acq_time = HAL_GetTick();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      /* 1. Obsługa Komunikacji UART (Odbiór i Parsowanie) */
	  if (MX_USART2_UART_kbhit()) {
            int16_t c = MX_USART2_UART_getchar();
            if (c != -1) {
                // Przekazanie znaku do maszyny stanów parsującej ramkę
                FSM_ProcessByte((uint8_t)c);
            }
	  }

      /* 2. Obsługa MPU6500 (Inicjalizacja i utrzymanie) */
      MPU6500_Maintain();

      /* 3. Obsługa Akwizycji Danych (Cykliczna)  */
      if (acquisition_running && (mpu_init_state == MPU_INIT_STATE_IDLE)) {
          if (HAL_GetTick() - last_acq_time >= acquisition_interval) {
              last_acq_time = HAL_GetTick();
              MPU6500_StartRead();
          }
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
