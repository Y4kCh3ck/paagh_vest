/**************************************************
 *	Filename: pv_sensors.h
 *	Description: Header file for sensors module
 **************************************************
 */

#ifndef PV_SENSORS_H
#define PV_SENSORS_H

void sensor_init();

// i2c sensors 
float get_humidity();
float get_temperature();

// adc sensors
int get_pulse();

// nrf
void send_nrf();

#endif