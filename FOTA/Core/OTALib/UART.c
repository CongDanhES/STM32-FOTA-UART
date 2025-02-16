/*
 * UART.c
 *
 *  Created on: Jan 16, 2025
 *      Author: NCPC
 */

#ifndef OTALIB_UART_C_
#define OTALIB_UART_C_

#include "UART.h"

#define CRC_PART_SIZE 400

uint8_t rx_data = 0;

uint8_t  Firmware_rx[SIZE+1][4];
uint16_t cur_index;
uint16_t cur_line;

uint16_t uart_count= 0;
uint8_t uart_rx[30];
uint8_t OTA_mode = OTA_INIT;

uint8_t crc_buffer[2];
uint8_t crc_index = 0;

bool CRC_success = 0;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
    	if(OTA_mode == OTA_INIT){
    		if(rx_data == '\r' || rx_data == '\n'){
    			uart_rx[uart_count] = '\0';
    			if(strcmp((char* )uart_rx, "OTA_START") == 0){
    				OTA_Start();
    				OTA_mode = OTA_START;
    			}
    			else{
    				OTA_mode = OTA_INIT;
    			}
				uart_count = 0;
				memset(uart_rx, 0 ,sizeof(uart_rx));
    		}else{
        		uart_rx[uart_count]= rx_data;
        		uart_count++;
    		}
    	}
    	else if (OTA_mode == OTA_START) {
    		Firmware_rx[cur_line][cur_index++] = rx_data;
    	    if (cur_index == 4 &&
    	        Firmware_rx[cur_line][0] == '#' &&
    	        Firmware_rx[cur_line][1] == '#' &&
    	        Firmware_rx[cur_line][2] == '#' &&
    	        Firmware_rx[cur_line][3] == '#') {

    	    	CRC_success = 1;
    	    	OTA_mode = OTA_CRC;
    	    }
    		if (cur_index >= 4) {
    			cur_index = 0;
    			cur_line++;
    			if(cur_line % 100 == 0){
    				OTA_mode = OTA_CRC;
    			}
    		}
    	}
    	else if (OTA_mode == OTA_CRC) {
    	    crc_buffer[crc_index++] = rx_data;

    	    if (crc_index == 2) {
    	        uint16_t received_crc = (crc_buffer[0] << 8) | crc_buffer[1];
    	        uint16_t crc;

    	        if (CRC_success == 0) {
    	            // Calculate CRC for the current firmware part
    	            crc = crc16_ccitt((uint8_t*)Firmware_rx[cur_line - (CRC_PART_SIZE / 4)], CRC_PART_SIZE);
    	        } else {
    	            // Calculate CRC for the remaining lines
    	            uint16_t remaining_lines = (cur_line - 1) % 100;
    	            crc = crc16_ccitt((uint8_t*)Firmware_rx[cur_line - 1 - remaining_lines], remaining_lines * 4);
    	        }

    	        // Compare received CRC with calculated CRC
    	        if (received_crc == crc) {
    	            CRC_True();
    	            if (CRC_success == 0) {
    	                OTA_mode = OTA_START; // Move to the next OTA mode
    	            } else {
    	                OTA_End();
    	                OTA_mode = OTA_END; // End OTA process
    	                Process_Bin_File(); // Process the binary file
    	            }
    	        } else {
    	            CRC_Fail();
    	            NVIC_SystemReset(); // Reset the system on CRC failure
    	        }

    	        // Reset CRC buffer and index
    	        crc_index = 0;
    	        memset(crc_buffer, 0, sizeof(crc_buffer));
    	    }
    	}
	    HAL_UART_Receive_IT(&huart2, &rx_data, 1);
    }
}

void OTA_Start(){
	uint8_t data[] = "OTA_START"; // Null-terminated string
    HAL_UART_Transmit(&huart2, data, strlen((char *)data),100);
}

void OTA_End(){
	uint8_t data[] = "OTA_END"; // Null-terminated string
    HAL_UART_Transmit(&huart2, data, strlen((char *)data),100);
}

void CRC_True(){
	uint8_t data[] = "CRC_OK"; // Null-terminated string
    HAL_UART_Transmit(&huart2, data, strlen((char *)data),100);
}

void CRC_Fail(){
	uint8_t data[] = "CRC_ERROR"; // Null-terminated string
    HAL_UART_Transmit(&huart2, data, strlen((char *)data),100);
}

#endif /* OTALIB_UART_C_ */
