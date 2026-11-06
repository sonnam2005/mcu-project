
#ifndef HAL_PWM_H
#define HAL_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f1xx_hal.h"

/* Khai bao Timer 2 */
extern TIM_HandleTypeDef htim2;

/* Khoi tao PWM tai TIM2 Channel 1 - PA0 */
void MX_TIM2_Init(void);

/* Dat do rong xung PWM (micro giay) */
void PWM_SetServoPulse(uint16_t xung_us);

/* Dieu khien servo theo goc 0 - 180 do */
void PWM_SetServoAngle(uint8_t goc);

#ifdef __cplusplus
}
#endif

#endif /* HAL_PWM_H */
