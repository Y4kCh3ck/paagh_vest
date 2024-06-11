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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <AHT20.h>
#include <NEO6.h>
#include <nRF24.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define LINE_MAX_LENGTH	80

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

NEO6_State GpsState;

bool ADC_EN = false;
bool GPS_EN = false;
bool I2C_EN = false;

volatile uint8_t Uart2ReceivedChar;
volatile float Temp = 0.0f;
volatile float Hum = 0.0f;
volatile uint32_t HeartBeatValue;

volatile uint8_t nrf24_rx_flag, nrf24_tx_flag, nrf24_mr_flag;
uint8_t Nrf24_Message[NRF24_PAYLOAD_SIZE];
uint8_t Message[32];
uint8_t MessageLength;
uint8_t Mess[80];

uint8_t value;

static uint8_t line_buffer[LINE_MAX_LENGTH + 1];
static uint32_t line_length;

void line_append(uint8_t value)
{
	if (value == '\r' || value == '\n') {
		// odebraliśmy znak końca linii
		if (line_length > 0) {
			// jeśli bufor nie jest pusty to dodajemy 0 na końcu linii
//			line_buffer[line_length] = '\0';
			// przetwarzamy dane
			printf("Otrzymano: %s\n", line_buffer);
			// zaczynamy zbieranie danych od nowa
			line_length = 0;
		}
	}
	else {
		if (line_length >= LINE_MAX_LENGTH) {
			// za dużo danych, usuwamy wszystko co odebraliśmy dotychczas
			line_length = 0;
		}
		// dopisujemy wartość do bufora
		line_buffer[line_length++] = value;
	}
}

int __io_putchar(int ch) // to pc
{
  if (ch == '\n') {
    __io_putchar('\r');
  }

  HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);

  return 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == GpsState.neo6_huart)
  {
    NEO6_ReceiveUartChar(&GpsState);
  }
  else if(huart == &hlpuart1)
  {
	  HAL_UART_Receive_IT(&hlpuart1, Mess, sizeof(Mess)); // receive fromm pcb
	  HAL_UART_Transmit_IT(&huart2, Mess, sizeof(Mess)); // send to pc
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim6) {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
//    AHT20_Read(&Temp, &Hum); // reads AHT20 measurements every second
//    printf("Temperature = ???\n Hum = ???\n");
    HAL_UART_Transmit(&huart2, ".", 1, 100);
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();
  MX_ADC_Init();
  MX_LPUART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//  AHT20_Init();
//  NEO6_Init(&GpsState, &huart1);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_UART_Receive_IT(&hlpuart1, Mess, sizeof(Mess));

  // RECEIVER
//  nRF24_Init(&hspi1);
//  nRF24_SetRXAddress(0, "Odb");
//  nRF24_SetTXAddress("Nad");
//  nRF24_RX_Mode();

  uint8_t value;
  uint8_t size = 0;

//  uint32_t Timer = HAL_GetTick();
//  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
//  HAL_ADC_Start_DMA(&hadc, (uint32_t*)&HeartBeatValue, 2);

  while (1)
  {
//	  printf("Heart Beat =  ???\n");
//	  HAL_Delay(100);\



//	  if (HAL_UART_Receive(&hlpuart1, &value, 1, 0) == HAL_OK)
//	  {
//		  line_append(value);
//	  }

//	  if(nRF24_RXAvailible())
//	  {
//		  nRF24_ReadRXPaylaod(Nrf24_Message, &size);
//		  MessageLength = sprintf(Message, "%c\n\r", &size);
//		  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//	  }

//	  MessageLength = sprintf((char*)Message, "\033[2J\033[;H"); // Clear terminal and home cursor
//	  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);

//	  NEO6_Task(&GpsState);
//
//	 	  if((HAL_GetTick() - Timer) > 1000)
//	 	  {
//	 		  MessageLength = sprintf((char*)Message, "\033[2J\033[;H"); // Clear terminal and home cursor
//	 		  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 		  if(NEO6_IsFix(&GpsState))
//	 		  {
//	 			  MessageLength = sprintf((char*)Message, "UTC Time: %02d:%02d:%02d\n\r", GpsState.Hour, GpsState.Minute, GpsState.Second);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 			  MessageLength = sprintf((char*)Message, "Date: %02d.%02d.20%02d\n\r", GpsState.Day, GpsState.Month, GpsState.Year);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 			  MessageLength = sprintf((char*)Message, "Latitude: %.2f %c\n\r", GpsState.Latitude, GpsState.LatitudeDirection);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 			  MessageLength = sprintf((char*)Message, "Longitude: %.2f %c\n\r", GpsState.Longitude, GpsState.LongitudeDirection);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 			  MessageLength = sprintf((char*)Message, "Altitude: %.2f m above sea level\n\r", GpsState.Altitude);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 			  MessageLength = sprintf((char*)Message, "Speed: %.2f knots, %f km/h\n\r", GpsState.SpeedKnots, GpsState.SpeedKilometers);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 			  MessageLength = sprintf((char*)Message, "Satelites: %d\n\r", GpsState.SatelitesNumber);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 			  MessageLength = sprintf((char*)Message, "Dilution of precision: %.2f\n\r", GpsState.Dop);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 			  MessageLength = sprintf((char*)Message, "Horizontal dilution of precision: %.2f\n\r", GpsState.Hdop);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//
//	 			  MessageLength = sprintf((char*)Message, "Vertical dilution of precision: %.2f\n\r", GpsState.Vdop);
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//	 		  }
//	 		  else
//	 		  {
//	 			  MessageLength = sprintf((char*)Message, "No Fix\n\r");
//	 			  HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
//	 		  }
//
//	 		  Timer = HAL_GetTick();
//	 	  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_8;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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