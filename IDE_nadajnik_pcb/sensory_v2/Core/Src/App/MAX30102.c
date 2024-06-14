/**************************************************
 *	Filename: MAX30102.c
 *	Description: Source file for pulsoximeter
 **************************************************
 */


#include "App/MAX30102.h"

#include "adc.h"
#include "usart.h"
#include "i2c.h"

uint32_t red_buffer[BUFFER_SIZE];
uint32_t buffer_index = 0;
uint32_t ma_index = 0;
uint32_t ma_sum = 0;

uint32_t ma_buffer[MA_FILTER_SIZE] = {0};

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

// Read raw data from MAX30102
void MAX30102_ReadFIFO(uint32_t *red_led) {
    uint8_t data[6];
    HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDRESS, REG_FIFO_DATA, I2C_MEMADD_SIZE_8BIT, data, 6, HAL_MAX_DELAY);

    *red_led = (data[0] << 16) | (data[1] << 8) | data[2];
    *red_led &= 0x03FFFF;  // Mask to 18 bits
}

float CalculateHeartRate(void) {
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
//        char msg[64];
//        sprintf(msg, "Heart Rate: %.2f bpm\r\n", bpm);
//        HAL_UART_Transmit(&hlpuart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

        last_beat = current_time;
    }

    last_value = current_value;
    buffer_index = (buffer_index + 1) % BUFFER_SIZE;

    return bpm;
}








