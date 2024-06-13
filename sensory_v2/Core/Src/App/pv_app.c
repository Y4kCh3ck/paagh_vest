/**************************************************
 *	Filename: pv_app.c
 *	Description: Source file for main paagh_vest application
 **************************************************
 */

#include "App/pv_app.h"

#include <stdio.h>
#include "adc.h"
#include "spi.h"

#include "AHT20.h"
#include <NEO6.h>
#include <nRF24.h>

#include "App/pv_pm.h"
#include "App/pv_sensors.h"
#include "App/pv_pulse.h"

extern uint8_t MessageLength; // TODO: delete extern
extern uint8_t Message[255];
extern uint8_t pulse;

extern volatile uint32_t HeartBeatValue;
extern volatile uint32_t HeartBeatArray[255];
extern volatile uint8_t HeartBeatIndex;

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

typedef struct instance_data{
    size_t* additional_data;
} instance_data_t;

typedef state_t state_func_t( instance_data_t *data );

void clear_terminal()
{
	  MessageLength = sprintf((char*)Message, "\033[2J\033[;H"); // Clear terminal and home cursor
	  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);
}

void GPS_print(NEO6_State GpsState)
{
	clear_terminal();
	NEO6_Task(&GpsState);

	if((HAL_GetTick() - Timer) > 100)
	{
	  clear_terminal();

	  if(NEO6_IsFix(&GpsState))
	  {
		  MessageLength = sprintf((char*)Message, "UTC Time: %02d:%02d:%02d\n\r", GpsState.Hour, GpsState.Minute, GpsState.Second);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);

		  MessageLength = sprintf((char*)Message, "Date: %02d.%02d.20%02d\n\r", GpsState.Day, GpsState.Month, GpsState.Year);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);

		  MessageLength = sprintf((char*)Message, "Latitude: %.2f %c\n\r", GpsState.Latitude, GpsState.LatitudeDirection);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);

		  MessageLength = sprintf((char*)Message, "Longitude: %.2f %c\n\r", GpsState.Longitude, GpsState.LongitudeDirection);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);

		  MessageLength = sprintf((char*)Message, "Altitude: %.2f m above sea level\n\r", GpsState.Altitude);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);

		  MessageLength = sprintf((char*)Message, "Speed: %.2f knots, %f km/h\n\r", GpsState.SpeedKnots, GpsState.SpeedKilometers);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);

		  MessageLength = sprintf((char*)Message, "Satelites: %d\n\r", GpsState.SatelitesNumber);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);

		  MessageLength = sprintf((char*)Message, "Dilution of precision: %.2f\n\r", GpsState.Dop);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);

		  MessageLength = sprintf((char*)Message, "Horizontal dilution of precision: %.2f\n\r", GpsState.Hdop);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);

		  MessageLength = sprintf((char*)Message, "Vertical dilution of precision: %.2f\n\r", GpsState.Vdop);
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);
	  }
	  else
	  {
		  MessageLength = sprintf((char*)Message, "No Fix\n\r");
		  HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 1000);
	  }

	  Timer = HAL_GetTick();
	}
}

// State functions
state_t do_state_init( instance_data_t *data ) {
	clear_terminal();
	MessageLength = sprintf(Message, "state init\n\r");
	if(NRF24_TRANSMITTED_PACKET != nRF24_SendPacket(Message, MessageLength))
	{
		HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 100);
	}

    // Power manager setup
    hard_reset_gps();
    hard_reset_adc_sens();
    hard_reset_i2c_sens();
    power_off_heater();

    // Sensor init
    sensor_init();

    return STATE_STANDBY;

}

state_t do_state_standby( instance_data_t *data )
{
    // Humidity, Temperature, Pressure, calucate average
    // If parameters not in proper range go to RESCUE
    // If user button held go to TEST
    // Low power

    
    float humidity, temperature;

    while( 1 ) {
        humidity = get_humidity();
        temperature = get_temperature();

    	clear_terminal();
    	MessageLength = sprintf(Message, "state standby\n\r");
    	if(NRF24_TRANSMITTED_PACKET != nRF24_SendPacket(Message, MessageLength))
    	{
    		HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 100);
    	}

		MessageLength = sprintf(Message, "Temperature = %.1fC\n\r Hum = %.1f\n\r", temperature, humidity);
		HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 100);

        //if( temperature < 10  || humidity > 80 ) // debug
        {
        	return STATE_RESCUE;
        }
    }
    
    return STATE_STANDBY;
}

state_t do_state_rescue( instance_data_t *data )
{
    // Low freq LED
    // Pulse read, pulsoximeter read? Time measurement, send SOS using NRF
    // If low pulse and low temperature go to EMERGENCY
    // If button held go to STANDBY

	power_on_adc_sens();
    HAL_ADC_Start_DMA(&hadc, &HeartBeatValue, 2);

    GPS_print(GpsState);
    while( 1 )
    {
    	clear_terminal();
    	MessageLength = sprintf(Message, "state rescue\n\r");
    	if(NRF24_TRANSMITTED_PACKET != nRF24_SendPacket(Message, MessageLength))
    	{
    		HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 100);
    	}

        HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

        pulse = get_pulse((uint16_t*)HeartBeatArray, 255);
		MessageLength = sprintf(Message, "Pulse = %d\n\r", pulse);
		HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 100);

        //if( temperature < 10  || pulse < 40 ) // debug
        {
        	return STATE_RESCUE;
        }

		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin))
		{
			return STATE_STANDBY;
		}
    }

    return STATE_EMERGENCY;
}

state_t do_state_emergency( instance_data_t *data )
{
    // High freq LED
    // Activate Petlier
    // Try to send SOS over NRF
    // If button held goto STANDBY

    power_on_heater();

    while( 1 )
    {
    	clear_terminal();
    	MessageLength = sprintf(Message, "state emergency\n\r");
    	if(NRF24_TRANSMITTED_PACKET != nRF24_SendPacket(Message, MessageLength))
    	{
    		HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 100);
    	}

        HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);

		MessageLength = sprintf(Message, "      --SOS--      \n\r");
		if(NRF24_TRANSMITTED_PACKET != nRF24_SendPacket(Message, MessageLength))
		{
			HAL_UART_Transmit(&hlpuart1, Message, MessageLength, 100);
		}

		if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin))
		{
			return STATE_STANDBY;
		}
    }
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