#line 1 "C:\\TTD\\CONG DANH\\FOTA\\App\\src\\myLibs\\Flash\\Flash.hpp"
/*
 * Flash.h
 *
 *  Created on: Jan 16, 2025
 *      Author: NCPC
 */

#ifndef FLASH_HPP
#define FLASH_HPP

#include "stdint.h"
#include "string.h"
#include "stm32f4xx.h"

// uint32_t GetSector(uint32_t Address);
// //void Flash_SectorErase(uint32_t Address);
// uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords);
// uint32_t Flash_Erase_Sector(uint32_t StartSectorAddress);

// void Flash_Read_Data(uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords);

class STM32Flash {
public:

    STM32Flash();
    ~STM32Flash();
    static uint32_t GetSector(uint32_t Address);
    static uint32_t FlashWrite(uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords);
    static uint32_t FlashErase(uint32_t StartSectorAddress);
    static void FlashRead(uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords);
    
private:

    void UnlockFlash();
    void LockFlash();

friend class FOTA;
};

#endif /* OTALIB_FLASH_H_ */
