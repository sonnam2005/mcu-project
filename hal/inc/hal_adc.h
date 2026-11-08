#ifndef HAL_ADC_H
#define HAL_ADC_H

#ifdef __cplusplus

extern "C" {  
#endif

#include "stm32f1xx_hal.h"

extern ADC_HandleTypeDef hadc1;

HAL_StatusTypeDef MX_ADC_Init(void);

uint16_t adc_read_channel(uint32_t channel, uint8_t n);

#ifdef __cplusplus
}
#endif

#endif // HAL_ADC_H