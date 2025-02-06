/*
 * Flash.h
 *
 *  Created on: Jan 16, 2025
 *      Author: NCPC
 */

#ifndef OTALIB_FLASH_H_
#define OTALIB_FLASH_H_

#include "stdint.h"
#include "string.h"
#include "stm32f4xx.h"

uint32_t GetSector(uint32_t Address);
//void Flash_SectorErase(uint32_t Address);
uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords);
uint32_t Flash_Erase_Sector(uint32_t StartSectorAddress);

void Flash_Read_Data(uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords);

#endif /* OTALIB_FLASH_H_ */
