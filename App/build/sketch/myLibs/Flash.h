#line 1 "C:\\TTD\\CONG DANH\\FOTA\\App\\myLibs\\Flash.h"
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
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_def.h"

uint32_t GetSector(uint32_t Address);
uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint32_t numberofwords);

#endif /* OTALIB_FLASH_H_ */
