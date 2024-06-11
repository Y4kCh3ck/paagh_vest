/**************************************************
 *	Filename: pv_pm.h
 *	Description: Header file for power manager
 **************************************************
 */

#ifndef PV_PM_H
#define PV_PM_H

void power_off_gps();
void power_on_gps();
void hard_reset_gps();

void power_on_adc_sens();
void power_off_adc_sens();
void hard_reset_adc_sens();

void power_on_i2c_sens();
void power_off_i2c_sens();
void hard_reset_i2c_sens();

void power_on_heater();
void power_off_heater();

#endif