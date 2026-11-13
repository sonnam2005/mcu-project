#include "task_display.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "app_cfg.h"
#include "app_ipc.h"
#include "ssd1306.h"


/* Task Display: Hiển thị các giá trị đọc được lên Oled */
void Task_display(void *pv)
{
    PowerData_t p = {0};
    LightData_t l = {0};

    /* Biến lưu chuỗi giá trị*/
    char buf_v[32];
    char buf_i[32];
    char buf_p[32];
    char buf_ldr1[32];
    char buf_ldr2[32];


    TickType_t last = xTaskGetTickCount();
    for(;;)
    {
        /* Chưa có giá trị mới thì lưu giá trị cũ */
        xQueuePeek(xPowerQueue, &p, 0);
        xQueuePeek(xLightQueue, &l, 0);

        /* Đổi giá trị thành chuỗi */
        snprintf(buf_v, sizeof(buf_v), "Voltage: %.3f V", p.voltage);
        snprintf(buf_i, sizeof(buf_i), "Current: %.3f A", p.current);
        snprintf(buf_p, sizeof(buf_p), "Power: %.3f W", p.power);
        snprintf(buf_ldr1, sizeof(buf_ldr1), "LDR1: %u", l.ldr1);
        snprintf(buf_ldr2, sizeof(buf_ldr2), "LDR2: %u", l.ldr2);

        /* Hiển thị lên Oled */
        SSD1306_Fill(SSD1306_BLACK);

        SSD1306_SetCursor(0, 0);
        SSD1306_WriteString("SOURCE MONITORING", SSD1306_WHITE);

        SSD1306_SetCursor(0, 10);
        SSD1306_WriteString(buf_v, SSD1306_WHITE);

        SSD1306_SetCursor(0, 20);
        SSD1306_WriteString(buf_i, SSD1306_WHITE);

        SSD1306_SetCursor(0, 30);
        SSD1306_WriteString(buf_p, SSD1306_WHITE);

        SSD1306_SetCursor(0, 40);
        SSD1306_WriteString(buf_ldr1, SSD1306_WHITE);

        SSD1306_SetCursor(0, 50);
        SSD1306_WriteString(buf_ldr2, SSD1306_WHITE);

        /* Chiếm quyền I2C - xI2CMutex */
        if(xSemaphoreTake(xI2CMutex, pdMS_TO_TICKS(100)) == pdTRUE)
        {
            SSD1306_UpdateScreen();
            xSemaphoreGive(xI2CMutex);
        }

        vTaskDelayUntil(&last, pdMS_TO_TICKS(DISPLAY_PERIOD_MS));
    }

}