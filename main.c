#include "stm32f1xx_hal.h"
#include "hal_pwm.h"

/* Hàm delay thô phục vụ test độc lập (chưa bật FreeRTOS Scheduler) */
static void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) {
        __NOP();
    }
}

int main(void)
{
    /* 1. Khởi tạo hệ thống HAL cơ bản */
    HAL_Init();
    
    /* (Lưu ý: Nếu nhóm đã có hàm SystemClock_Config() thì giữ nguyên, 
     * nếu chưa, hệ thống sẽ chạy mặc định ở xung nhịp nội HSI) */

    /* 2. Khởi tạo PWM TIM2 cho Servo tại chân PA0 */
    if (MX_TIM2_Init() != HAL_OK) {
        /* Bị lỗi phần cứng hoặc cấu hình */
        Error_Handler();
    }

    /* Vòng lặp test quét góc Servo liên tục */
    while (1)
    {
        servo_set_angle(20.0f);   pos: góc tối thiểu
        delay_ms(1000);           /* Đợi 1 giây */

        servo_set_angle(90.0f);   /* Quay về giữa (90 độ) */
        delay_ms(1000);

        servo_set_angle(160.0f);  /* Quay sang góc tối đa */
        delay_ms(1000);
    }
}