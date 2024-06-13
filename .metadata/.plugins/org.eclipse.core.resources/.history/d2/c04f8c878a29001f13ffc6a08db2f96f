/***************************************************
 *   Filename: pv_pm.c
 *   Description: Source file for Power Manager
 ***************************************************
 */

#include "pv_pm.h"

#include "main.h"

#define HARD_RESET_TIME_MS 200

// NOTE: gps, i2c and adc sensors are connected to PMOS, set pin HIGH to disable, and LOW to enable


void power_on_gps() {
    HAL_GPIO_WritePin( GPS_EN_GPIO_Port, GPS_EN_Pin , GPIO_PIN_RESET );
}

void power_off_gps() {
    HAL_GPIO_WritePin( GPS_EN_GPIO_Port, GPS_EN_Pin , GPIO_PIN_SET );
}

void hard_reset_gps() {
    power_off_gps();
    HAL_Delay( HARD_RESET_TIME_MS );
    power_on_gps();
}


void power_on_adc_sens() {
    HAL_GPIO_WritePin( ADC_EN_GPIO_Port, ADC_EN_Pin, GPIO_PIN_RESET );
}

void power_off_adc_sens() {
    HAL_GPIO_WritePin( ADC_EN_GPIO_Port, ADC_EN_Pin, GPIO_PIN_SET );
}

void hard_reset_adc_sens() {
    power_off_adc_sens();
    HAL_Delay( HARD_RESET_TIME_MS );
    power_on_adc_sens();
}


void power_on_i2c_sens() {
    HAL_GPIO_WritePin( I2C_EN_GPIO_Port, I2C_EN_Pin, GPIO_PIN_RESET );
}

void power_off_i2c_sens() {
    HAL_GPIO_WritePin( I2C_EN_GPIO_Port, I2C_EN_Pin, GPIO_PIN_SET );

}

void hard_reset_i2c_sens() {
    power_off_i2c_sens();
    HAL_Delay( HARD_RESET_TIME_MS );
    power_on_i2c_sens();
}


void power_on_heater() {
    HAL_GPIO_WritePin( HEATER_EN_GPIO_Port, HEATER_EN_Pin, GPIO_PIN_RESET );
}

void power_off_heater() {
    HAL_GPIO_WritePin( HEATER_EN_GPIO_Port, HEATER_EN_Pin, GPIO_PIN_RESET );
}
