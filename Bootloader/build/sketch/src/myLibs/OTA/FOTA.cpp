#line 1 "C:\\TTD\\CONG DANH\\FOTA\\Bootloader\\src\\myLibs\\OTA\\FOTA.cpp"
#include "FOTA.hpp"

FOTA::FOTA()
{
    // Constructor
}

FOTA::~FOTA()
{
    // Destructor
}

void FOTA::ProcessBinFile(uint8_t Firmware[][4],uint16_t length)
{
    // Process the binary file
    uint32_t numberofwords = length - 1;
    STM32Flash::FlashWrite(RX_WORD, &numberofwords, 1);

    // Write the firmware data to the flash memory
    uint32_t status = 0;
	STM32Flash::FlashRead(FLAG_ADDRESS,&status, 1);

    uint32_t write_address = SLOT1;

    for (int i= 0; i < numberofwords; i++) {
        uint32_t word_to_write =(Firmware[i][3] << 24) | (Firmware[i][2] << 16) | (Firmware[i][1] << 8)| Firmware[i][0];
        STM32Flash::FlashWrite(write_address, &word_to_write, 1);
        write_address += 4;
    }

    // update Flag for bootloader
    uint32_t flag = 0x00000001;
    STM32Flash::FlashWrite(FLAG_ADDRESS, &flag, 1);

    // reset system
    HAL_NVIC_SystemReset();
}


uint16_t FOTA::crc16_ccitt(uint8_t *data, uint16_t length) {
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

void FOTA::CopyToApplication(uint32_t Address)
{
    uint32_t target_Address = Address;
    uint32_t application_Address = APPLICATION;
    uint32_t copy_Buffer = 0;

    // get word received
    uint32_t word = 0;
    STM32Flash::FlashRead(RX_WORD, &word, 1);

    // Copy only the number of received words
    STM32Flash::FlashRead(target_Address, &copy_Buffer, 1);
    for (uint32_t i = 0; i < word; i++) {
        STM32Flash::FlashWrite(application_Address, &copy_Buffer, 1);
        target_Address += 4;
        application_Address += 4;
        STM32Flash::FlashRead(target_Address, &copy_Buffer, 1);
    }

    // Clear the flag to indicate bootloader is done
    uint32_t flag = 0x00000000;
    STM32Flash::FlashWrite(FLAG_ADDRESS, &flag, 1);
    STM32Flash::FlashWrite(RX_WORD, &flag, 1);
}

void FOTA::JumpToApplication(uint32_t Address)
{
    // set main stack pointer and update vector table
    __set_MSP(*(__IO uint32_t*)Address);
    SCB->VTOR = Address;

    // Jump to application
    typedef void (*pFunction)(void);
    pFunction JumpToApp = (pFunction)(*(__IO uint32_t*)(Address + 4));

    JumpToApp();
}