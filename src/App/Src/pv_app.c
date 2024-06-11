/**************************************************
 *	Filename: pv_app.c
 *	Description: Source file for main paagh_vest application
 **************************************************
 */

#include "pv_app.h"

#include "pv_pm.h"
#include "pv_sensors.h"

// Typedefs
typedef enum {
    STATE_INIT,
    STATE_STANDBY,
    STATE_RESCUE,
    STATE_EMERGENCY,
    STATE_TEST,
    STATE_ERROR,
    NUM_STATES,
} state_t;


typedef state_t state_func_t( instance_data_t *data );


// State functions
state_t do_state_init( instance_data_t *data ) {
    printf("state init\n");

    // Power manager setup
    hard_reset_gps();
    hard_reset_adc_sens();
    hard_reset_i2c_sens();
    power_off_heater();

    // Sensor init
    sensor_init();

    return STATE_STANDBY;

}

state_t do_state_standby( instance_data_t *data ) {

    printf("state standby\n");

    // Humidity, Temperature, Pressure, calucate average
    // If parameters not in proper range go to RESCUE
    // If user button held go to TEST
    // Low power

    
    int humidity, temperature;

    while( 1 ) {        
        humidity = get_humidity();
        temperature = get_temperature();

        if( temperature < 10 )
            return STATE_RESCUE;

    }
    
    return STATE_RESCUE;
}

state_t do_state_rescue( instance_data_t *data ) {
    printf("state rescue\n");
    // Low freq LED
    // Pulse read, pulsoximeter read? Time measurement, send SOS using NRF
    // If low pulse and low temperature go to EMERGENCY
    // If button held go to STANDBY

    return STATE_EMERGENCY;
}

state_t do_state_emergency( instance_data_t *data ) {
    // High freq LED
    // Activate Petlier
    // Try to send SOS over NRF
    // If button held goto STANDBY

    // HAL_GPIO_WritePin(HEATER_EN_GPIO_Port, HEATER_EN_Pin, GPIO_PIN_SET);

    //if it is possible, blinking should be done with interrupt from timer    
    // HAL_Delay(200);
    // HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

    read_button(RETURN_TO_STANDBY);
    if (data->buttonPressed){
        data->buttonPressed = false;
        return STATE_STANDBY;
    }

    printf("state emergency\n");
    return STATE_INIT;
}

state_t do_state_test( instance_data_t *data ) {
    // Simulation of other stated without GPS and Petlie
    // Low freq led?
    // If button held go to STANDBY
    
    return STATE_STANDBY;
}

state_t do_state_error( instance_data_t *data ) {
    // Well.. fuck
    // Constant LED
    // printf("state error");
    // Maybe something will come up later...

    printf("Error");
    while( 1 ) {
        ;;
    }
}



// State table
state_func_t* const state_table[ NUM_STATES ] = {
    do_state_init,
    do_state_standby,
    do_state_rescue,
    do_state_emergency,
    do_state_test,
    do_state_error,
};

state_t run_state( state_t current_state, instance_data_t *data ) {
    return state_table[ current_state ](data);
}


// Main loop
void pv_run() {

    state_t current_state = STATE_INIT;
    instance_data_t data = { NULL };
    while( 1 ) {
        current_state = run_state( current_state, &data );
    }

    run_state(STATE_ERROR, &data);

}
