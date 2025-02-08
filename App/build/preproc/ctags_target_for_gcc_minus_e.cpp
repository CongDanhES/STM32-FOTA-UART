# 1 "C:\\TTD\\CONG DANH\\FOTA\\App\\App.ino"
# 2 "C:\\TTD\\CONG DANH\\FOTA\\App\\App.ino" 2
# 3 "C:\\TTD\\CONG DANH\\FOTA\\App\\App.ino" 2

# 5 "C:\\TTD\\CONG DANH\\FOTA\\App\\App.ino" 2



uint32_t OTA_FLAG = 10;

// UART buffer
char receivedData[20];
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
    pinMode(63, 0x1);
    fota_port.setTx(53);
    fota_port.setRx(54);
    fota_port.begin(115200);

    memset(receivedData, 0, 20);

    // Create FreeRTOS tasks
    xTaskCreate(Task_UART, "UART_Task", 256, 
# 34 "C:\\TTD\\CONG DANH\\FOTA\\App\\App.ino" 3 4
                                            __null
# 34 "C:\\TTD\\CONG DANH\\FOTA\\App\\App.ino"
                                                , 2, &Task_UART_Handle);
    xTaskCreate(Task_LED, "LED_Task", 128, 
# 35 "C:\\TTD\\CONG DANH\\FOTA\\App\\App.ino" 3 4
                                          __null
# 35 "C:\\TTD\\CONG DANH\\FOTA\\App\\App.ino"
                                              , 1, &Task_LED_Handle);

    // Start FreeRTOS scheduler
    vTaskStartScheduler();
}

// Task to toggle LED every 2 seconds
void Task_LED(void *pvParameters) {
    (void) pvParameters;
    while (1) {
        digitalWrite(63, !digitalRead(63));
        vTaskDelay(( ( TickType_t ) ( ( ( TickType_t ) ( 200 ) * ( TickType_t ) ((TickType_t)1000) ) / ( TickType_t ) 1000 ) )); // Delay for 2 seconds
    }
}

// Task to receive UART data and toggle LED if "OTA_START" is received
void Task_UART(void *pvParameters) {
    (void) pvParameters;
    while (1) {
        if (fota_port.available()) {
            char receivedChar = fota_port.read();

            if (dataIndex < 20 - 1) {
                receivedData[dataIndex++] = receivedChar;
            }

            if (receivedChar == '\n' || receivedChar == '\r') {
                receivedData[dataIndex] = '\0';

                if (strcmp(receivedData, "OTA_START\r\0") == 0) {
                    // Optional: Send acknowledgment back
                    fota_port.println("OTA_START");
                    flash.FlashWrite(0x08008000, &OTA_FLAG, 1);
                    delay(1000);
                    HAL_NVIC_SystemReset();

                    // Reset buffer
                    memset(receivedData, 0, 20);
                    dataIndex = 0;
                } else {
                    memset(receivedData, 0, 20);
                    dataIndex = 0;
                }
            }
        }
        vTaskDelay(( ( TickType_t ) ( ( ( TickType_t ) ( 10 ) * ( TickType_t ) ((TickType_t)1000) ) / ( TickType_t ) 1000 ) )); // Small delay to prevent task starvation
    }
}

void loop() {
    // FreeRTOS is handling tasks, so loop remains empty
}
