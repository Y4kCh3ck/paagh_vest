#include "main.h"
#include "pv_button.h"
#include "pv_app.h"
#include <stdbool.h>

uint8_t buttonPressedCounter = 0;

void read_button(uint8_t pressMode)
{
    if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin))
    {
        if (RETURN_TO_STANDBY <= buttonPressedCounter)
        {   
            buttonPressedCounter = 0;
            instance_data.buttonPressed = true;            
        }        
        buttonPressedCounter++;
    }    
}