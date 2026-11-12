#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "app_cfg.h"
#include "app_ipc.h"
#include "hal_gpio.h"
#include "hal_i2c.h"
#include "hal_adc.h"
#include "hal_pwm.h"
#include "ina219.h"
#include "ssd1306.h"   
#include "task_display.h"
#include "task_tracking.h"
#include "task_sensor.h"

void Error_Handler(void);

/* Hook bắt lỗi */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask; (void)pcTaskName;
    __disable_irq();
    for (;;) { }
}

void vApplicationMallocFailedHook(void)
{
    __disable_irq();
    for (;;) { }
}

int main(void)
{
    /* Khởi tạo phần cứng  */
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    if (MX_ADC_Init()  != HAL_OK) { Error_Handler(); }
    if (MX_TIM2_Init() != HAL_OK) { Error_Handler(); }

    if (SSD1306_Init(&hi2c1) != HAL_OK) { Error_Handler(); }
    if (INA219_Init(&hi2c1)  != HAL_OK) { Error_Handler(); }

    /* Tạo mutex và queue */
    app_ipc_init();

    /* Tạo các task: dùng hằng trong app_cfg.h */
    if (xTaskCreate(Task_tracking, "Tracking", TASK_TRACKING_STACK, NULL, TASK_TRACKING_PRIO, NULL) != pdPASS) { Error_Handler(); }
    if (xTaskCreate(Task_sensor,   "Sensor",   TASK_SENSOR_STACK,   NULL, TASK_SENSOR_PRIO,   NULL) != pdPASS) { Error_Handler(); }
    if (xTaskCreate(Task_display,  "Display",  TASK_DISPLAY_STACK,  NULL, TASK_DISPLAY_PRIO,  NULL) != pdPASS) { Error_Handler(); }

    /* Bắt đầu scheduler */
    vTaskStartScheduler();

    for (;;)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) { }
}