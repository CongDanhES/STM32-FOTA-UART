/*
 * UART.h
 *
 *  Created on: Feb 5, 2025
 *      Author: NCPC
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include "string.h"
#include "Flash.h"

extern UART_HandleTypeDef huart2;
extern uint8_t rx_data;

void ENTER_OTA_Mode();
#endif /* INC_UART_H_ */
