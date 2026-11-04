#ifndef APP_IPC_H
#define APP_IPC_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/* Du lieu 2 cam bien anh sang LDR */
typedef struct {
    uint16_t ldr1;
    uint16_t ldr2;
    uint32_t timestamp;
} LightData_t;

/* Du lieu dien tu INA219 */
typedef struct {
    uint16_t voltage_mv;
    int16_t  current_ma;
    uint32_t timestamp;
} PowerData_t;

/* Goc 2 servo (do) */
typedef struct {
    uint8_t servo1_deg;
    uint8_t servo2_deg;
} ServoCmd_t;

extern QueueHandle_t     xLightQueue;
extern QueueHandle_t     xPowerQueue;
extern QueueHandle_t     xServoQueue;
extern SemaphoreHandle_t xI2CMutex;

void app_ipc_init(void);

#endif
