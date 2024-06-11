#ifndef PV_BUTTON_H
#define PV_BUTTON_H

#define RETURN_TO_STANDBY 20

#include <stdint.h>

/*
uint8_t pressMode - defined in pv_button.h - declares how long button 
should be pressed to read it
This function should be invoked with 0.25 sec interrupt
Holding button for 5 seconds return to standby mode
*/
void read_button(uint8_t pressMode);

#endif
