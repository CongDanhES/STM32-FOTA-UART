#ifndef _DEFINE_H_
#define _DEFINE_H_

#include "config.h"

#define LEDBLUE         PD15
#define LEDRED          PD14
#define LEDORANGE       PD13
#define LEDYELLOW       PD12

#define BUFFER_SIZE     30

#define FLAG_ADDRESS    0x08008000

HardwareSerial fota_port(FOTA_PORT);

#endif