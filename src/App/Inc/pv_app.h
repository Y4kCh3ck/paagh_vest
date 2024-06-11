/**************************************************
 *	Filename: pv_app.h
 *	Description: Header file for main paagh_vest application
 **************************************************
 */

#ifndef PV_APP_H
#define PV_APP_H

typedef struct instance_data{
    size_t* additional_data;
    bool buttonPressed;
} instance_data_t;

void pv_run();

#endif
