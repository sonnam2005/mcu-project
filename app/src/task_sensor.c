#include "task_sensor.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "app_cfg.h"
#include "app_ipc.h"
#include "INA219.h"

/* Task Sensor: Đọc các giá trị của INA219 qua I2C - khóa mutex */
void Task_sensor(void *pv)
{
    TickType_t last = xTaskGetTickCount();

    for(;;)
    {
        float v = 0.0f, i = 0.0f, p = 0.0f;
        HAL_StatusTypeDef status = HAL_ERROR;

        /* Chiếm quyền I2C - xI2CMutex */
        if(xSemaphoreTake(xI2CMutex, pdMS_TO_TICKS(100)) == pdTRUE)
        {
            status = INA219_ReadBusVoltage(&v);
            status |= INA219_ReadCurrent(&i);
            status |= INA219_ReadPower(&p);
            xSemaphoreGive(xI2CMutex);
        }
        
        if(status == HAL_OK)
        {
            PowerData_t d;
            d.voltage = v;
            d.current = i;
            d.power   = p;
            d.timestamp = xTaskGetTickCount();
            xQueueOverwrite (xPowerQueue, &d);
        }
        vTaskDelayUntil(&last, pdMS_TO_TICKS(SENSOR_PERIOD_MS));
    }
}