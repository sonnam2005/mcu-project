#include "hal_pwm.h"
#include "app_cfg.h"

TIM_HandleTypeDef htim2;

HAL_StatusTypeDef MX_TIM2_Init(void)
{
    /* 1. Bật clock cho GPIOA và TIM2 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* 2. Cấu hình chân PA0 xuất xung PWM */
    GPIO_InitTypeDef cau_hinh_chan = {0};
    cau_hinh_chan.Pin   = GPIO_PIN_0;
    cau_hinh_chan.Mode  = GPIO_MODE_AF_PP;
    cau_hinh_chan.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &cau_hinh_chan);

    /* 3. Cấu hình bộ đếm TIM2: chia tần về 1MHz (1 tick = 1 us), chu kỳ 20000 us (20ms) */
    uint32_t tan_so_tim = HAL_RCC_GetPCLK1Freq();
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != 0) {
        tan_so_tim *= 2; /* Nhân đôi nếu APB1 prescaler khác 1 */
    }

    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = (tan_so_tim / 1000000U) - 1U;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = 20000U - 1U;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
        return HAL_ERROR;
    }

    /* 4. Cấu hình Kênh 1 (CH1) ở chế độ PWM Mode 1 */
    TIM_OC_InitTypeDef cau_hinh_pwm = {0};
    cau_hinh_pwm.OCMode     = TIM_OCMODE_PWM1;
    cau_hinh_pwm.Pulse      = 1500U; /* Khởi động ở 90 độ (1500 us) */
    cau_hinh_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;
    cau_hinh_pwm.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&htim2, &cau_hinh_pwm, TIM_CHANNEL_1) != HAL_OK) {
        return HAL_ERROR;
    }

    /* 5. Bắt đầu xuất xung PWM ra chân PA0 và đặt góc khởi đầu */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    servo_set_angle(SERVO_START_DEG);

    return HAL_OK;
}

void servo_set_angle(float goc_dat)
{
    /* Giới hạn góc an toàn (20 độ -> 160 độ) */
    if (goc_dat < SERVO_MIN_DEG) goc_dat = SERVO_MIN_DEG;
    if (goc_dat > SERVO_MAX_DEG) goc_dat = SERVO_MAX_DEG;

    /* Đổi từ độ (0..180) sang độ rộng xung (500..2500 us) */
    uint16_t xung_us = (uint16_t)(500.0f + (goc_dat / 180.0f) * 2000.0f);

    /* Ghi trực tiếp vào thanh ghi CCR1 để đổi góc */
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, xung_us);
}