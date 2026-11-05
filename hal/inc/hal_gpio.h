#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#ifdef __cplusplus

extern "C" {  
#endif

#include "stm32f1xx_hal.h"

void MX_GPIO_Init(void);
void SystemClock_Config(void);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif // HAL_GPIO_H