/*
 * Global.h
 *
 *  Created on: Feb 5, 2025
 *      Author: NCPC
 */

#ifndef OTALIB_GLOBAL_H_
#define OTALIB_GLOBAL_H_

#define ADDRESS 			0x08020000
#define SLOT1 				0x08040000
#define SLOT2 				0x08060000
#define FLAG_ADDRESS 		0x08008000
#define RX_WORD 			0x0800C000

enum UART_MODE{
	OTA_INIT,
	OTA_START,
	OTA_END,
	OTA_PROCESS,
	OTA_ERROR,
	OTA_CRC
};

#define SIZE 5830
#endif /* OTALIB_GLOBAL_H_ */
