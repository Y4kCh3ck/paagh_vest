#include "pv_sensors.h"
#include "AHT20.h"

uint8_t tempCounter = 0;
uint8_t humidityCounter = 0;

float averageTemp;
float tempSum;
float averageHumidity;
float humiditySum;

void sensor_init() {
    AHT20_Init();
}

float get_humidity() {    
    float humidity = 0;

    AHT20_Read( NULL, &humidity );

    if (INTERRUPT_COUNTER <= humidityCounter)
    {
        humiditySum = 0;
        humidityCounter = 0;
    }
    else
    {
        humiditySum += humidity;
        averageHumidity = humiditySum / humidityCounter;
        humidityCounter++;
    }

    return averageHumidity;
}

float get_temperature() {    
    float temp = 0;

    AHT20_Read( &temp, NULL );

    if (INTERRUPT_COUNTER <= tempCounter)
    {
        tempSum = 0;
        tempCounter = 0;
    }
    else
    {           
        tempSum += temp;
        averageTemp = tempSum / tempCounter;
        tempCounter++;
    }

    return averageTemp;
}