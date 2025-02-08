#line 1 "C:\\TTD\\CONG DANH\\FOTA\\Bootloader\\src\\myLibs\\Utils\\define.hpp"
#ifndef DEFINE_HPP
#define DEFINE_HPP

#include "src/myLibs/Utils/config.hpp"
#include "STM32FreeRTOS.h"

// UART FOTA port 
HardwareSerial fota_PORT(FOTA_PORT);

// FOTA 
#include "src/myLibs/OTA/FOTA.hpp"

#define FOTA_SIZE 10000
#define CRC_PART_SIZE 400

enum FOTA_MODE{
	OTA_INIT,
	OTA_START,
	OTA_END,
	OTA_PROCESS,
	OTA_ERROR,
	OTA_CRC
};

FOTA fota;

// Flash
#include "src/myLibs/Flash/Flash.hpp"
STM32Flash flash;


#endif