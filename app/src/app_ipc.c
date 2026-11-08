#include "app_ipc.h"
 
QueueHandle_t     xLightQueue;
QueueHandle_t     xPowerQueue;
QueueHandle_t     xServoQueue;
SemaphoreHandle_t xI2CMutex;
 
void app_ipc_init(void)
{
    xI2CMutex   = xSemaphoreCreateMutex();
    /* "hộp thư" 1 phần tử: ghi đè bằng xQueueOverwrite, đọc bằng xQueuePeek */
    xLightQueue = xQueueCreate(1, sizeof(LightData_t));
    xPowerQueue = xQueueCreate(1, sizeof(PowerData_t));
    xServoQueue = xQueueCreate(1, sizeof(ServoCmd_t));
    configASSERT(xI2CMutex && xLightQueue && xPowerQueue && xServoQueue);
}