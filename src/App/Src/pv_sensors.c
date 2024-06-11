#include "pv_sensors.h"
#include "AHT20.h"

void sensor_init() {
    AHT20_Init();
}

float get_humidity() {
    float humidity;
    // TODO: Averaging
    AHT20_Read( NULL, &humidity );

    return humidity;
}

float get_temperature() {
    float temp;
    // TODO: Averaging
    AHT20_Read( &temp, NULL );

    return temp;
}
