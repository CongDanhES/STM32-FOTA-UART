/*
 * UART.h
 *
 *  Created on: Jan 16, 2025
 *      Author: NCPC
 */

#ifndef OTALIB_UART_H_
#define OTALIB_UART_H_

#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"
#include "main.h"
#include "OTA.h"
#include "Global.h"

// UART2 for receive Firmware
extern UART_HandleTypeDef huart2;

/*	RX buffer for OTA
 * 	rx_data: 1 data receive from UART
 *	Firmware_rx: firmware receive
 *	cur_index: current index
 *	cur_line: current line
 * */
extern uint8_t rx_data;
extern uint8_t Firmware_rx[SIZE+1][4];
extern uint16_t cur_index;
extern uint16_t cur_line;

extern uint8_t OTA_mode;
extern bool CRC_success;

void OTA_Start();
void OTA_End();
void CRC_True();
void CRC_Fail();

extern uint8_t CRC_check;


#endif /* OTALIB_UART_H_ */
