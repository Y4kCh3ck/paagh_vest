/*
 * AHT20.c
 *
 *  Created on: Apr 4, 2024
 *      Author: kuba
 */

#include "AHT20.h"

HAL_StatusTypeDef AHT20_Master_Transmit(uint8_t *data, uint16_t size)
{
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Transmit(&hi2c1, AHT20_ADDR, data, size, HAL_MAX_DELAY);

	return status;
}

HAL_StatusTypeDef AHT20_Master_Receive(uint8_t *data, uint16_t size)
{
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Receive(&hi2c1, AHT20_READ_ADDR, data, size, HAL_MAX_DELAY);

	return status;
}

HAL_StatusTypeDef AHT20_Write_Reg(uint8_t reg, uint8_t write_value)
{
	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Write(&hi2c1, AHT20_ADDR, reg, 1, &write_value, sizeof(write_value), HAL_MAX_DELAY);

	return status;
}

uint8_t AHT20_Read_Reg(uint8_t reg)
{
	uint8_t read_value = 0;
	HAL_I2C_Mem_Read(&hi2c1, AHT20_ADDR, reg, 1, &read_value, sizeof(read_value), HAL_MAX_DELAY);

	return read_value;
}

void AHT20_Init()
{
	uint8_t init_cmd[] = {0xBE, 0x08, 0x00};
	if (AHT20_Master_Transmit(init_cmd, sizeof(init_cmd)) != HAL_OK)
	{
		printf("AHT20 initialisation error\n");
	}
	else
	{
		printf("AHT20 initialised\n");
		uint8_t value = 0x00;
		HAL_Delay(10);
		AHT20_Master_Receive(&value, sizeof(value));
		if ((value & 0x08) != 0x00) // check calibration bit
		{
			printf("AHT20 calibrated\n");
		}
		else
		{
			printf("AHT20 not calibrated\n");
		}
	}
}

void AHT20_Read(volatile float *Temp, volatile float *Hum)
{
	uint8_t trig_measure_cmd[] = {0xAC, 0x33, 0x00};
	if (AHT20_Master_Transmit(trig_measure_cmd, sizeof(trig_measure_cmd)) != HAL_OK)
	{
		printf("AHT20 trigger measurement error\n");
	}
	else
	{
		HAL_Delay(80); // wait for the measurements to be completed
		// reading measurements
		uint8_t read_buffer[6];
		AHT20_Master_Receive(read_buffer, sizeof(read_buffer));

		if(((read_buffer[0]) & 0x80) == 0x00)
		{
			uint32_t data = 0;
			data = (((uint32_t)read_buffer[3] >> 4) + ((uint32_t)read_buffer[2] << 4) + ((uint32_t)read_buffer[1] << 12));
			*Hum = data * 100.0f / (1 << 20);

			data = (((uint32_t)read_buffer[3] & 0x0F) << 16) + ((uint32_t)read_buffer[4] << 8) + ((uint32_t)read_buffer[5]);
			*Temp = data * 200.0f / (1 << 20) - 50;
		}
	}
}
