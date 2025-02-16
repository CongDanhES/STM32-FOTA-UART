/*
 * UART.c
 *
 *  Created on: Feb 5, 2025
 *      Author: NCPC
 */

#include "UART.h"

uint8_t rx_data = 0;
uint8_t uart_rx[30];
uint8_t uart_count= 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
    	if(rx_data == '\r' || rx_data == '\n'){
			if(strcmp((char* )uart_rx, "OTA_START") == 0){
				uint32_t OTA_FLAG = 10;
				Flash_Write_Data(0x08004000, &OTA_FLAG, 1);
//				ENTER_OTA_Mode();
			    NVIC_SystemReset();
			}
    	}
		uart_rx[uart_count]= rx_data;
		uart_count++;
	    HAL_UART_Receive_IT(&huart2, &rx_data, 1);
    }
}

//void ENTER_OTA_Mode(){
//	uint8_t data[] = "OTA ENTER";
//    HAL_UART_Transmit(&huart2, data, strlen((char *)data),100);
//}
