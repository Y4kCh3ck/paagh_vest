/*
 * usart.h
 *
 *  Created on: Jun 12, 2024
 *      Author: ferna
 */

/*
*#ifndef INC_USART_H_
*#define INC_USART_H_
*
*
*
*#endif /* INC_USART_H_ */


#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l0xx_hal.h"

extern UART_HandleTypeDef huart2;

void MX_USART2_UART_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H */
