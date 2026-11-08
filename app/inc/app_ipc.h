#ifndef APP_IPC_H
#define APP_IPC_H
 
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
 
/* 2 cảm biến ánh sáng LDR (0...4065) */
typedef struct {
    uint16_t ldr1;          /* PA4 */
    uint16_t ldr2;          /* PA5 */
    uint32_t timestamp;     /* tick lúc đó */
} LightData_t;
 
/* Du lieu INA219 (don vi nho nhat de khoi phai truyen float) */
typedef struct {
    float voltage;
    float  current;
    float  power;
    uint32_t timestamp;
} PowerData_t;
 
/* Góc 1 servo (do) */
typedef struct {
    float angle_deg;
} ServoCmd_t;
 
extern QueueHandle_t     xLightQueue;
extern QueueHandle_t     xPowerQueue;
extern QueueHandle_t     xServoQueue;
extern SemaphoreHandle_t xI2CMutex;
 
/* Tao mutex + queue; gọi trước vTaskStartScheduler() */
void app_ipc_init(void);
 
#endif