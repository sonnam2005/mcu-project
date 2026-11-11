#include "HAL_ADC.h"

ADC_HandleTypeDef hadc1;

HAL_StatusTypeDef MX_ADC_Init(void)
{
    __HAL_RCC_ADC1_CLK_ENABLE();
 
    /* Xung ADC tối đa 14 MHz. Mặc định sau reset là PCLK2 / 2 = 36 MHz (quá giới hạn),
       nên chia 6: 72 MHz / 6 = 12 MHz. */
    RCC_PeriphCLKInitTypeDef pclk = {0};
    pclk.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    pclk.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&pclk) != HAL_OK) {
        return HAL_ERROR;
    }
 
    hadc1.Instance = ADC1;
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        return HAL_ERROR;
    }
 
    /* Hiệu chuẩn ADC một lần sau khi khởi tạo */
    return HAL_ADCEx_Calibration_Start(&hadc1);
 
}
 
uint16_t adc_read_channel(uint32_t channel, uint8_t n)
{
    if (n == 0) {
        n = 1;
    }
 
    ADC_ChannelConfTypeDef cfg = {0};
    cfg.Channel = channel;
    cfg.Rank = ADC_REGULAR_RANK_1;
    cfg.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &cfg) != HAL_OK) {
        return 0;
    }
 
    uint32_t sum = 0;
    for (uint8_t i = 0; i <= n; i++)            /* chạy n + 1 lần, bỏ mẫu đầu sau khi đổi kênh */
    {
        HAL_ADC_Start(&hadc1);
        if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
            uint32_t v = HAL_ADC_GetValue(&hadc1);
            if (i > 0) {
                sum += v;
            }
        }
        HAL_ADC_Stop(&hadc1);
    }
    return (uint16_t)(sum / n);
}

