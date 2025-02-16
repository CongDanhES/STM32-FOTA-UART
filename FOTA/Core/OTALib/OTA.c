/*
 * OTA.c
 *
 *  Created on: Jan 14, 2025
 *      Author: NCPC
 */

#include "OTA.h"

uint32_t word_to_write = 0;
uint32_t number_word = 0;

void Process_Bin_File() {
	number_word = cur_line-1;
	Flash_Write_Data(RX_WORD, &number_word, 1);

	uint32_t status = 0;
	Flash_Read_Data(FLAG_ADDRESS,&status, 1);

    uint32_t write_address = SLOT1;

    for (int i= 0; i < SIZE; i++) {
        uint32_t word_to_write =
            (Firmware_rx[i][3] << 24) |
            (Firmware_rx[i][2] << 16) |
            (Firmware_rx[i][1] << 8)  |
             Firmware_rx[i][0];

        Flash_Write_Data(write_address, &word_to_write, 1);
        write_address += 4;
    }

    // update Flag for bootloader
    uint32_t flag = 0x00000001;
    Flash_Write_Data(FLAG_ADDRESS, &flag, 1);

    // reset system
    NVIC_SystemReset();
}

// Copy SLOT 1||2 to APPLICATION ADDRESS
//void Copy_To_Application(uint32_t Address){
//	uint32_t target_Address= Address;
//    uint32_t application_Address = ADDRESS;
//	uint32_t copy_Buffer= 0;
//
//	Flash_Read_Data(target_Address,&copy_Buffer, 1);
////	while(copy_Buffer!= 0xFFFFFFFF){
////		Flash_Write_Data(application_Address, &copy_Buffer, 1);
////		target_Address+= 4;
////		application_Address += 4;
////		Flash_Read_Data(target_Address,&copy_Buffer, 1);
////	}
//	while(number_word > 0){
//		Flash_Write_Data(application_Address, &copy_Buffer, 1);
//		target_Address+= 4;
//		application_Address += 4;
//		Flash_Read_Data(target_Address,&copy_Buffer, 1);
//		number_word --;
//	}
//
//    uint32_t flag = 0x00000000;
//    Flash_Write_Data(FLAG_ADDRESS, &flag, 1);
//}
void Copy_To_Application(uint32_t Address) {
    uint32_t target_Address = Address;
    uint32_t application_Address = ADDRESS;
    uint32_t copy_Buffer = 0;

    uint32_t word = 0;
    Flash_Read_Data(RX_WORD, &word, 1);


    // Copy only the number of received words
    Flash_Read_Data(target_Address, &copy_Buffer, 1);
    for (uint32_t i = 0; i < word; i++) {

//        if (copy_Buffer == 0xFFFFFFFF) {
//            break; // Stop copying if invalid data is encountered
//        }

        Flash_Write_Data(application_Address, &copy_Buffer, 1);
        target_Address += 4;
        application_Address += 4;
        Flash_Read_Data(target_Address, &copy_Buffer, 1);
    }

    // Clear the flag to indicate bootloader is done
    uint32_t flag = 0x00000000;
    Flash_Write_Data(FLAG_ADDRESS, &flag, 1);
    Flash_Write_Data(RX_WORD, &flag, 1);
}


// Jump to Application
void Jump_To_Application(uint32_t appAddress)
{
    // set main stack pointer and update vector table
    __set_MSP(*(__IO uint32_t*)appAddress);
    SCB->VTOR = appAddress;

    // Jump to application
    typedef void (*pFunction)(void);
    pFunction JumpToApp = (pFunction)(*(__IO uint32_t*)(appAddress + 4));

    JumpToApp();
}

// CRC
uint16_t crc16_ccitt(uint8_t *data, uint16_t length) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}


