/*
 * OTA.h
 *
 *  Created on: Jan 14, 2025
 *      Author: NCPC
 */

#ifndef OTALIB_OTA_H_
#define OTALIB_OTA_H_

#include "Flash.h"
#include "UART.h"
#include "Global.h"

extern uint32_t word_to_write;
extern uint32_t number_word ;

void Process_Bin_File();
void Jump_To_Application(uint32_t appAddress);
void Copy_To_Application();
uint16_t crc16_ccitt(uint8_t *data, uint16_t length);

#endif /* OTALIB_OTA_H_ */
