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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX30102_ADDRESS          0x57 << 1
#define REG_INTR_ENABLE_1         0x02
#define REG_INTR_ENABLE_2         0x03
#define REG_FIFO_WR_PTR           0x04
#define REG_OVF_COUNTER           0x05
#define REG_FIFO_RD_PTR           0x06
#define REG_FIFO_DATA             0x07
#define REG_FIFO_CONFIG           0x08
#define REG_MODE_CONFIG           0x09
#define REG_SPO2_CONFIG           0x0A
#define REG_LED1_PA               0x0C
#define REG_LED2_PA               0x0D

#define MODE_HEART_RATE           0x02
#define BUFFER_SIZE 100
#define THRESHOLD 5000 //

//#define BUFFER_SIZE 100
#define MA_FILTER_SIZE 5

uint32_t red_buffer[BUFFER_SIZE];
//uint32_t ma_buffer[MA_FILTER_SIZE];
uint32_t buffer_index = 0;
uint32_t ma_index = 0;
uint32_t ma_sum = 0;

uint32_t ma_buffer[MA_FILTER_SIZE] = {0};
//uint32_t ma_index = 0;
//uint32_t ma_sum = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
void MAX30102_Init(void) {
    uint8_t data[2];

    // Reset the sensor
    data[0] = REG_MODE_CONFIG;
    data[1] = 0x40;
    HAL_I2C_Master_Transmit(&hi2c1, MAX30102_ADDRESS, data, 2, HAL_MAX_DELAY);
    HAL_Delay(10);

    // Enable the interrupt
    data[0] = REG_INTR_ENABLE_1;
    data[1] = 0xC0;
    HAL_I2C_Master_Transmit(&hi2c1, MAX30102_ADDRESS, data, 2, HAL_MAX_DELAY);

    // FIFO Configuration
    data[0] = REG_FIFO_CONFIG;
    data[1] = 0x0F;
    HAL_I2C_Master_Transmit(&hi2c1, MAX30102_ADDRESS, data, 2, HAL_MAX_DELAY);

    // Mode Configuration
    data[0] = REG_MODE_CONFIG;
    data[1] = MODE_HEART_RATE;
    HAL_I2C_Master_Transmit(&hi2c1, MAX30102_ADDRESS, data, 2, HAL_MAX_DELAY);

    // SPO2 Configuration
    data[0] = REG_SPO2_CONFIG;
    data[1] = 0x27;
    HAL_I2C_Master_Transmit(&hi2c1, MAX30102_ADDRESS, data, 2, HAL_MAX_DELAY);

    // LED Pulse Amplitude
    data[0] = REG_LED1_PA;
    data[1] = 0x24;
    HAL_I2C_Master_Transmit(&hi2c1, MAX30102_ADDRESS, data, 2, HAL_MAX_DELAY);
}

uint32_t MovingAverage(uint32_t new_value) {
    ma_sum -= ma_buffer[ma_index];
    ma_sum += new_value;
    ma_buffer[ma_index] = new_value;
    ma_index = (ma_index + 1) % MA_FILTER_SIZE;

    return ma_sum / MA_FILTER_SIZE;
}

//uint32_t red_buffer[BUFFER_SIZE];
//uint32_t buffer_index = 0;

// Read raw data from MAX30102
void MAX30102_ReadFIFO(uint32_t *red_led) {
    uint8_t data[6];
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDRESS, REG_FIFO_DATA, I2C_MEMADD_SIZE_8BIT, data, 6, HAL_MAX_DELAY);

    *red_led = (data[0] << 16) | (data[1] << 8) | data[2];
    *red_led &= 0x03FFFF;  // Mask to 18 bits
}

void CalculateHeartRate(void) {
    static uint32_t last_beat = 0;
    static uint32_t beats = 0;
    static uint32_t last_value = 0;
    uint32_t current_time = HAL_GetTick();
    uint32_t time_diff;
    float bpm;

    // Read new data
    MAX30102_ReadFIFO(&red_buffer[buffer_index]);
    uint32_t current_value = MovingAverage(red_buffer[buffer_index]);

    // Peak detection logic
    if (current_value > THRESHOLD && current_value > last_value) {
        if (current_time - last_beat > 700) { // period to avoid multiple detections per beat
            beats++;
            last_beat = current_time;
        }
    }

    // Calculate heart rate every 10 seconds
    if (current_time - last_beat >= 10000) {
        time_diff = current_time - last_beat;
        bpm = (beats / (time_diff / 60000.0)); // Calculate BPM
        beats = 0; // Reset beat count

        // Print BPM
        char msg[64];
        sprintf(msg, "Heart Rate: %.2f bpm\r\n", bpm);
        HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

        last_beat = current_time;
    }

    last_value = current_value;
    buffer_index = (buffer_index + 1) % BUFFER_SIZE;
}




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  MAX30102_Init();
  /* USER CODE END 2 */
  //printf("System Initialized.\r\n");

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  CalculateHeartRate();
	  HAL_Delay(100);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
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
