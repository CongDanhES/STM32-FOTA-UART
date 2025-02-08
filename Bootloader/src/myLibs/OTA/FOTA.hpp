#ifndef FOTA_HPP
#define FOTA_HPP

#include "src/myLibs/Flash/Flash.hpp"

#define APPLICATION 	    0x08020000
#define SLOT1 				0x08040000
#define SLOT2 				0x08060000
#define FLAG_ADDRESS 		0x08008000
#define RX_WORD 			0x0800C000

class FOTA
{
public:
    FOTA();
    ~FOTA();
    void ProcessBinFile(uint8_t Firmware[][4], uint16_t length);
    void CopyToApplication(uint32_t Address);
    void JumpToApplication(uint32_t Address);
    uint16_t crc16_ccitt(uint8_t *data, uint16_t length);
};


#endif