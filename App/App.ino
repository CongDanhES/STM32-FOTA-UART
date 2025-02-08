#include "Arduino.h"
#include "STM32FreeRTOS.h"  // FreeRTOS library

#include "src/myLibs/define.h"

#define BUFFER_SIZE 20

uint32_t OTA_FLAG = 10;

// UART buffer
char receivedData[BUFFER_SIZE];
uint8_t dataIndex = 0;

// LED state
bool ledState = false;

// Task handles
TaskHandle_t Task_UART_Handle;
TaskHandle_t Task_LED_Handle;

// Function prototypes
void Task_UART(void *pvParameters);
void Task_LED(void *pvParameters);

void setup() {
    pinMode(LEDBLUE, OUTPUT);
    fota_port.setTx(PD5);
    fota_port.setRx(PD6);
    fota_port.begin(115200);

    memset(receivedData, 0, BUFFER_SIZE);

    // Create FreeRTOS tasks
    xTaskCreate(Task_UART, "UART_Task", 256, NULL, 2, &Task_UART_Handle);
    xTaskCreate(Task_LED, "LED_Task", 128, NULL, 1, &Task_LED_Handle);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();
}

// Task to toggle LED every 2 seconds
void Task_LED(void *pvParameters) {
    (void) pvParameters;
    while (1) {
        digitalWrite(LEDBLUE, !digitalRead(LEDBLUE));
        vTaskDelay(pdMS_TO_TICKS(200));  // Delay for 2 seconds
    }
}

// Task to receive UART data and toggle LED if "OTA_START" is received
void Task_UART(void *pvParameters) {
    (void) pvParameters;
    while (1) {
        if (fota_port.available()) {
            char receivedChar = fota_port.read();

            if (dataIndex < BUFFER_SIZE - 1) {
                receivedData[dataIndex++] = receivedChar;
            }

            if (receivedChar == '\n' || receivedChar == '\r') {
                receivedData[dataIndex] = '\0';

                if (strcmp(receivedData, "OTA_START\r\0") == 0) {
                    // Optional: Send acknowledgment back
                    fota_port.println("OTA_START");
                    flash.FlashWrite(FLAG_ADDRESS, &OTA_FLAG, 1);
                    delay(1000);
                    HAL_NVIC_SystemReset();

                    // Reset buffer
                    memset(receivedData, 0, BUFFER_SIZE);
                    dataIndex = 0;
                } else {
                    memset(receivedData, 0, BUFFER_SIZE);
                    dataIndex = 0;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));  // Small delay to prevent task starvation
    }
}

void loop() {
    // FreeRTOS is handling tasks, so loop remains empty
}
