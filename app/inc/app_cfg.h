#ifndef APP_CFG_H
#define APP_CFG_H

#include "FreeRTOS.h"
#include "task.h"

/* Do uu tien: TRACKING > SENSOR > DISPLAY */
#define TASK_TRACKING_PRIO   (tskIDLE_PRIORITY + 4)
#define TASK_SENSOR_PRIO     (tskIDLE_PRIORITY + 3)
#define TASK_DISPLAY_PRIO    (tskIDLE_PRIORITY + 2)

/* Stack (don vi: words) */
#define TASK_SENSOR_STACK    256
#define TASK_TRACKING_STACK  512
#define TASK_DISPLAY_STACK   512

#endif
