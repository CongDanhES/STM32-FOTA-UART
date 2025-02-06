#line 1 "C:\\TTD\\CONG DANH\\FOTA\\App\\myLibs\\Flash.c"
#include "Flash.h"

/**
 * @brief  Retrieves the sector number for a given address(F407).
 * @param  Address: Flash memory address for which the sector is needed.
 * @retval uint32_t: Sector number corresponding to the given address.
 */
uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if((Address < 0x08003FFF) && (Address >= 0x08000000))
    {
        sector = FLASH_SECTOR_0;
    }
    else if((Address < 0x08007FFF) && (Address >= 0x08004000))
    {
        sector = FLASH_SECTOR_1;
    }
    else if((Address < 0x0800BFFF) && (Address >= 0x08008000))
    {
        sector = FLASH_SECTOR_2;
    }
    else if((Address < 0x0800FFFF) && (Address >= 0x0800C000))
    {
        sector = FLASH_SECTOR_3;
    }
    else if((Address < 0x0801FFFF) && (Address >= 0x08010000))
    {
        sector = FLASH_SECTOR_4;
    }
    else if((Address < 0x0803FFFF) && (Address >= 0x08020000))
    {
        sector = FLASH_SECTOR_5;
    }
    else if((Address < 0x0805FFFF) && (Address >= 0x08040000))
    {
        sector = FLASH_SECTOR_6;
    }
    else if((Address < 0x0807FFFF) && (Address >= 0x08060000))
    {
        sector = FLASH_SECTOR_7;
    }
    else if((Address < 0x0809FFFF) && (Address >= 0x08080000))
    {
        sector = FLASH_SECTOR_8;
    }
    else if((Address < 0x080BFFFF) && (Address >= 0x080A0000))
    {
        sector = FLASH_SECTOR_9;
    }
    else if((Address < 0x080DFFFF) && (Address >= 0x080C0000))
    {
        sector = FLASH_SECTOR_10;
    }
    else if((Address < 0x080FFFFF) && (Address >= 0x080E0000))
    {
        sector = FLASH_SECTOR_11;
    }

    return sector;
}

/**
 * @brief  Writes an array of data to flash memory starting at a specified address.
 * @param  StartPageAddress: The starting address in flash memory where the data will be written.
 *                           Must be aligned with the flash memory write requirements.
 * @param  Data: Pointer to the array of 32-bit data to be written to flash memory.
 * @param  numberofwords: Number of 32-bit words to write from the Data array.
 * @retval uint32_t: Returns HAL_OK if the operation is successful, or an error code otherwise.
 *                   The error code is defined in the HAL library.
 */
uint32_t Flash_Write_Data(uint32_t StartSectorAddress, uint32_t *Data, uint32_t numberofwords) {
    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SECTORError;
    int sofar = 0;

    /* Unlock the Flash to enable the flash control register access */
    HAL_FLASH_Unlock();

    /* Get the sector of the starting address */
    uint32_t StartSector = GetSector(StartSectorAddress);

    /* Check if the sector has been erased or if data can be written */
    if (*(uint32_t *)StartSectorAddress != 0xFFFFFFFF) {
        /* If the start address is not erased (0xFFFFFFFF is the erased state),
           then we need to erase the sector before writing */

        uint32_t EndSectorAddress = StartSectorAddress + numberofwords * 4;
        uint32_t EndSector = GetSector(EndSectorAddress);

        /* Fill EraseInit structure for the sector we want to erase */
        EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
        EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
        EraseInitStruct.Sector = StartSector;
        EraseInitStruct.NbSectors = (EndSector - StartSector) + 1;

        /* Erase the required sector */
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) {
            return HAL_FLASH_GetError(); // Error during erase
        }
    }

    /* Program the user Flash area word by word */
    while (sofar < numberofwords) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, Data[sofar]) == HAL_OK) {
            StartSectorAddress += 4; // Move to the next word address
            sofar++;
        } else {
            /* Error occurred while writing data in Flash memory */
            return HAL_FLASH_GetError();
        }
    }

    /* Lock the Flash to disable the flash control register access */
    HAL_FLASH_Lock();
    return 0;
}