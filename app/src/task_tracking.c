#include <stdio.h>
#include "FreeRTOS.h"
#include "task_tracking.h"
#include "app_cfg.h"
#include "app_ipc.h"
#include "hal_adc.h"
#include "hal_pwm.h"


void Task_tracking(void *pv)
{
    (void)pv;
    TickType_t thoi_diem_cu = xTaskGetTickCount();

    float goc_hien_tai = SERVO_START_DEG;
    servo_set_angle(goc_hien_tai);

    for (;;)
    {
        /* 1. Đọc giá trị 2 LDR từ PA4 (Channel 4) và PA5 (Channel 5), lọc trung bình 8 mẫu */
        uint16_t ldr_trai = adc_read_channel(ADC_CHANNEL_4, 8);
        uint16_t ldr_phai = adc_read_channel(ADC_CHANNEL_5, 8);

        /* 2. Tính độ lệch ánh sáng giữa 2 bên */
        int32_t do_lech = (int32_t)ldr_trai - (int32_t)ldr_phai;
        int32_t do_lech_tuyet_doi = (do_lech >= 0) ? do_lech : -do_lech;

        /* 3. Nếu lệch vượt ngưỡng vùng chết (TRACK_DEADBAND) thì mới quay Servo */
        if (do_lech_tuyet_doi > TRACK_DEADBAND)
        {
            float buoc_quay = (float)do_lech * TRACK_KP * (float)TRACK_DIR;

            /* Giới hạn bước quay tối đa mỗi lần (2 độ) để không bị giật */
            if (buoc_quay > TRACK_MAX_STEP)  buoc_quay = TRACK_MAX_STEP;
            if (buoc_quay < -TRACK_MAX_STEP) buoc_quay = -TRACK_MAX_STEP;

            goc_hien_tai += buoc_quay;

            /* Giới hạn góc quay trong vùng an toàn (20 -> 160 độ) */
            if (goc_hien_tai < SERVO_MIN_DEG) goc_hien_tai = SERVO_MIN_DEG;
            if (goc_hien_tai > SERVO_MAX_DEG) goc_hien_tai = SERVO_MAX_DEG;

            servo_set_angle(goc_hien_tai);
        }

        /* 4. Gửi dữ liệu ánh sáng và góc servo vào Queue cho Task_display hiển thị */
        LightData_t du_lieu_anh_sang = { ldr_trai, ldr_phai, xTaskGetTickCount() };
        ServoCmd_t  du_lieu_goc      = { goc_hien_tai };

        xQueueOverwrite(xLightQueue, &du_lieu_anh_sang);
        xQueueOverwrite(xServoQueue, &du_lieu_goc);

        /* 5. Ngủ chờ đúng chu kỳ 30ms */
        vTaskDelayUntil(&thoi_diem_cu, pdMS_TO_TICKS(TRACKING_PERIOD_MS));
    }
}