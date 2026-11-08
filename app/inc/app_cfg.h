#ifndef APP_CFG_H
#define APP_CFG_H
 
#include "FreeRTOS.h"
#include "task.h"
 
/* Độ ưu tiên: TRACKING > SENSOR > DISPLAY (configMAX_PRIORITIES phải > 4) */
#define TASK_TRACKING_PRIO   (tskIDLE_PRIORITY + 4)
#define TASK_SENSOR_PRIO     (tskIDLE_PRIORITY + 3)
#define TASK_DISPLAY_PRIO    (tskIDLE_PRIORITY + 2)
 
/* Stack (đơn vị: words, 1 word = 4 byte) */
#define TASK_TRACKING_STACK  256
#define TASK_SENSOR_STACK    256
#define TASK_DISPLAY_STACK   512   /* snprintf + %f cần nhiều stack */
 
/* chu kì task (ms) */
#define TRACKING_PERIOD_MS   30
#define SENSOR_PERIOD_MS     250
#define DISPLAY_PERIOD_MS    250
 
/* tham số điều khiển bám nắng (cần chỉnh lại) */
#define TRACK_DEADBAND       80
#define TRACK_KP             0.01f
#define TRACK_MAX_STEP       2.0f
#define TRACK_DIR            (+1)     /* -1 : quay ngược */
#define SERVO_MIN_DEG        20.0f
#define SERVO_MAX_DEG        160.0f
#define SERVO_START_DEG      90.0f
 
#endif
