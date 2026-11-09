#ifndef HAL_PWM_H
#define HAL_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim2;

/* Khởi tạo PWM trên TIM2_CH1 (Chân PA0) tần số 50Hz (chu kỳ 20ms) */
HAL_StatusTypeDef MX_TIM2_Init(void);

/* Hàm đặt góc quay cho Servo (từ 20 đến 160 độ) */
void servo_set_angle(float goc_dat);

#ifdef __cplusplus
}
#endif

#endif // HAL_PWM_H