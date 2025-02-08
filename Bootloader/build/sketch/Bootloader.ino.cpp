#include <Arduino.h>
#line 1 "C:\\TTD\\CONG DANH\\FOTA\\Bootloader\\Bootloader.ino"
#include "src/myLibs/Utils/define.hpp"

// define task handle
TaskHandle_t Task_FOTA_Handle;

// task function parameters
void Task_FOTA(void *pvParameters);

#line 9 "C:\\TTD\\CONG DANH\\FOTA\\Bootloader\\Bootloader.ino"
void setup();
#line 45 "C:\\TTD\\CONG DANH\\FOTA\\Bootloader\\Bootloader.ino"
void loop();
#line 9 "C:\\TTD\\CONG DANH\\FOTA\\Bootloader\\Bootloader.ino"
void setup(){

    // FOTA port begin 
    fota_PORT.setTx(PD5);
    fota_PORT.setRx(PD6);
    fota_PORT.begin(115200);   

    // FreeRTOS task creation
    xTaskCreate(Task_FOTA, "Task_FOTA", 256, NULL, 1, &Task_FOTA_Handle);

    // Check if OTA flag set
    uint32_t rx= 0;
    flash.FlashRead(FLAG_ADDRESS,&rx, 1);

    if(rx == 1){
        // if flag set, copy the firmware to the application area and jump to the application
        fota.CopyToApplication(SLOT1);
        fota_PORT.println("JUMP TO APPLICATION");
        delay(1000);
        fota.JumpToApplication(APPLICATION);
    }
    else if(rx == 10){
        fota_PORT.println("OTA ENTER");
    }
    else{
        uint32_t app= 0;
        flash.FlashRead(APPLICATION,&rx, 1);
        if(rx != 0xFFFFFFFF){
            fota.JumpToApplication(APPLICATION);
        }
    }

    // Start the scheduler
    vTaskStartScheduler();
}

void loop(){

}

uint8_t OTA_mode = OTA_INIT;
uint16_t uart_count = 0;
uint8_t uart_rx[30];
uint8_t rx_data;

uint8_t Firmware_rx[FOTA_SIZE][4];
uint16_t cur_index = 0;
uint16_t cur_line = 0;

uint8_t crc_buffer[2];
uint8_t crc_index = 0;

bool EofFlag = 0;

void Task_FOTA(void *pvParameters){
    while(1){
        if (fota_PORT.available() > 0)
        {
            rx_data = fota_PORT.read();
            // OTA Init mode: receive "OTA_START" command 
            if(OTA_mode == OTA_INIT){
                if(rx_data == '\r' || rx_data == '\n'){
                    uart_rx[uart_count] = '\0';
                    if(strcmp((char* )uart_rx, "OTA_START\0") == 0){
                        fota_PORT.println("OTA_START");
                        OTA_mode = OTA_START;
                    }
                    else{
                        OTA_mode = OTA_INIT;
                    }
                    uart_count = 0;
                    memset(uart_rx, 0 ,sizeof(uart_rx));
                }else{
                    uart_rx[uart_count]= rx_data;
                    uart_count++;
                }
            }
            // OTA Start mode: receive firmware data
            else if (OTA_mode == OTA_START) {
                Firmware_rx[cur_line][cur_index++] = rx_data;
                if (cur_index == 4 &&
                    Firmware_rx[cur_line][0] == '#' &&
                    Firmware_rx[cur_line][1] == '#' &&
                    Firmware_rx[cur_line][2] == '#' &&
                    Firmware_rx[cur_line][3] == '#') {

                    EofFlag = 1;
                    OTA_mode = OTA_CRC;
                }
                if (cur_index >= 4) {
                    cur_index = 0;
                    cur_line++;
                    if(cur_line % 100 == 0){
                        OTA_mode = OTA_CRC;
                    }
                }
    	    }
            // OTA CRC mode: receive CRC data
            else if (OTA_mode == OTA_CRC) {

                crc_buffer[crc_index++] = rx_data;
                if (crc_index == 2) {
                    uint16_t received_crc = (crc_buffer[0] << 8) | crc_buffer[1];
                    uint16_t crc;

                    if (EofFlag == 0) {
                        // Calculate CRC for the current firmware part
                        crc = fota.crc16_ccitt((uint8_t*)Firmware_rx[cur_line - (CRC_PART_SIZE / 4)], CRC_PART_SIZE);
                    } else {
                        // Calculate CRC for the remaining lines
                        uint16_t remaining_lines = (cur_line - 1) % 100;
                        crc = fota.crc16_ccitt((uint8_t*)Firmware_rx[cur_line - 1 - remaining_lines], remaining_lines * 4);
                    }

                    // Compare received CRC with calculated CRC
                    if (received_crc == crc) {
                        fota_PORT.println("CRC_OK");
                        if (EofFlag == 0) {
                            OTA_mode = OTA_START; // Move to the next OTA mode
                        } else {
                            fota_PORT.println("CRC_END");
                            OTA_mode = OTA_END; // End OTA process
                            fota.ProcessBinFile(Firmware_rx,cur_line); // Process the binary file
                        }
                    } else {
                        fota_PORT.println("CRC_ERROR");
                        HAL_NVIC_SystemReset();
                    }

                    // Reset CRC buffer and index
                    crc_index = 0;
                    memset(crc_buffer, 0, sizeof(crc_buffer));
    	        }
    	    }         
        }
    }
}
