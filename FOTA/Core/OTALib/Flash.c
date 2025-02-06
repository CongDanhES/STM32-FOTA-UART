/*
 * Flash.c
 *
 *  Created on: Jan 16, 2025
 *      Author: NCPC
 */

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
//uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords)
//{
//
//	static FLASH_EraseInitTypeDef EraseInitStruct;
//	uint32_t SECTORError;
//	int sofar=0;
//
//
//	 /* Unlock the Flash to enable the flash control register access *************/
//	 HAL_FLASH_Unlock();
//
//	 /* Erase the user Flash area */
//
////	  /* Get the number of sector to erase from 1st sector */
////
//	  uint32_t StartSector = GetSector(StartSectorAddress);
//	  uint32_t EndSectorAddress = StartSectorAddress + numberofwords*4;
//	  uint32_t EndSector = GetSector(EndSectorAddress);
////
////	  /* Fill EraseInit structure*/
//	  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
//	  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
//	  EraseInitStruct.Sector        = StartSector;
//	  EraseInitStruct.NbSectors     = (EndSector - StartSector) + 1;
////
////	  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
////	     you have to make sure that these data are rewritten before they are accessed during code
////	     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
////	     DCRST and ICRST bits in the FLASH_CR register. */
//	  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
//	  {
//		  return HAL_FLASH_GetError ();
//	  }
//
//	  /* Program the user Flash area word by word
//	    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/
//
//	   while (sofar<numberofwords)
//	   {
//	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, StartSectorAddress, Data[sofar]) == HAL_OK)
//	     {
//	    	 StartSectorAddress += 4;  // use StartPageAddress += 2 for half word and 8 for double word
//	    	 sofar++;
//	     }
//	     else
//	     {
//	       /* Error occurred while writing data in Flash memory*/
//	    	 return HAL_FLASH_GetError ();
//	     }
//	   }
//
//	  /* Lock the Flash to disable the flash control register access (recommended
//	     to protect the FLASH memory against possible unwanted operation) *********/
//	  HAL_FLASH_Lock();
//	  return 0;
//}

uint32_t Flash_Write_Data(uint32_t StartSectorAddress, uint32_t *Data, uint16_t numberofwords) {
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


/**
 * @brief  Reads data from flash memory into a buffer.
 * @param  StartSectorAddress: The starting address in flash memory to read data from.
 *                             Must be a valid flash memory address.
 * @param  RxBuf: Pointer to the buffer where the read data will be stored.
 *                The buffer must have enough space to store `numberofwords` 32-bit values.
 * @param  numberofwords: Number of 32-bit words to read from flash memory.
 * @retval None
 */
void Flash_Read_Data(uint32_t StartSectorAddress, uint32_t *RxBuf, uint16_t numberofwords)
{
    while (numberofwords--)
    {
        *RxBuf = *(__IO uint32_t *)StartSectorAddress; // Read 32-bit word from flash
        StartSectorAddress += 4;                      // Move to the next 32-bit word address
        RxBuf++;                                      // Move buffer pointer to the next word
    }
}

/**
 * @brief  Erases a specified Flash sector.
 * @param  StartSectorAddress: The starting address of the Flash sector to be erased.
 *                             Must be aligned with the Flash sector boundaries.
 * @retval uint32_t: Returns HAL_OK if the operation is successful, or an error code otherwise.
 *                   The error code is defined in the HAL library.
 */
uint32_t Flash_Erase_Sector(uint32_t StartSectorAddress)
{
    static FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SECTORError;

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Get the sector number based on the provided starting address */
    uint32_t StartSector = GetSector(StartSectorAddress);

    /* Fill EraseInit structure */
    EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;  // Assuming 3V voltage range
    EraseInitStruct.Sector        = StartSector;
    EraseInitStruct.NbSectors     = 1;  // Only erasing one sector

    /* Erase the sector */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
    {
        /* If an error occurred during erase, return the error code */
        uint32_t error = HAL_FLASH_GetError();
        HAL_FLASH_Lock();  // Ensure flash is locked before returning
        return error;
    }

    /* Lock the Flash to disable the flash control register access (recommended to protect the Flash memory against unwanted operation) */
    HAL_FLASH_Lock();

    return HAL_OK;  // Operation successful
}

//void Flash_SectorErase(uint32_t Address)
//{
//	uint32_t sectorNumber = GetSector(Address);
//    // 1. Wait until no flash memory operation is ongoing
//    while (FLASH->SR & FLASH_SR_BSY);
//
//    // 2. Unlock the flash memory if locked
//    if (FLASH->CR & FLASH_CR_LOCK)
//    {
//        FLASH->KEYR = 0x45670123; // Unlock key 1
//        FLASH->KEYR = 0xCDEF89AB; // Unlock key 2
//    }
//
//    // 3. Set the SER bit and select the sector to erase
//    FLASH->CR &= ~(FLASH_CR_SNB);           // Clear the sector number
//    FLASH->CR |= (sectorNumber << FLASH_CR_SNB_Pos); // Set the sector number
//    FLASH->CR |= FLASH_CR_SER;             // Enable sector erase
//
//    // 4. Start the erase operation
//    FLASH->CR |= FLASH_CR_STRT;
//
//    // 5. Wait until the BSY bit is cleared
//    while (FLASH->SR & FLASH_SR_BSY);
//
//    // 6. Clear the SER bit to disable sector erase
//    FLASH->CR &= ~FLASH_CR_SER;
//
//    // 7. Lock the flash memory to prevent accidental writes
//    FLASH->CR |= FLASH_CR_LOCK;
//}

