
#include "hal_pwm.h"

/* Gioi han xung dieu khien servo */
#define XUNG_MIN_US     1000U
#define XUNG_MAX_US     2000U
#define XUNG_GIUA_US    1500U

#define GOC_TOI_DA      180U

/* Khai bao Timer 2 */
TIM_HandleTypeDef htim2;


/*
 * Khoi tao PWM:
 * - Chan PA0
 * - TIM2 Channel 1
 * - Tan so 50 Hz
 * - Chu ky 20 ms
 */
void MX_TIM2_Init(void)
{
    TIM_OC_InitTypeDef cauhinh_pwm = {0};

    /* Cap clock cho TIM2 */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* Cau hinh bo dem Timer 2 */
    htim2.Instance = TIM2;

    htim2.Init.Prescaler = 71;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 19999;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    /* Khoi tao PWM */
    HAL_TIM_PWM_Init(&htim2);

    /* Cau hinh kenh PWM Channel 1 */
    cauhinh_pwm.OCMode = TIM_OCMODE_PWM1;
    cauhinh_pwm.Pulse = XUNG_GIUA_US;
    cauhinh_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;
    cauhinh_pwm.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(
        &htim2,
        &cauhinh_pwm,
        TIM_CHANNEL_1
    );

    /* Bat dau phat PWM */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}


/*
 * Thay doi do rong xung PWM
 * xung_us: do rong xung tinh bang micro giay
 */
void PWM_SetServoPulse(uint16_t xung_us)
{
    /* Gioi han xung nho nhat */
    if (xung_us < XUNG_MIN_US)
    {
        xung_us = XUNG_MIN_US;
    }

    /* Gioi han xung lon nhat */
    if (xung_us > XUNG_MAX_US)
    {
        xung_us = XUNG_MAX_US;
    }

    /* Cap nhat do rong xung */
    __HAL_TIM_SET_COMPARE(
        &htim2,
        TIM_CHANNEL_1,
        xung_us
    );
}


/*
 * Dieu khien servo theo goc
 * goc: tu 0 den 180 do
 */
void PWM_SetServoAngle(uint8_t goc)
{
    uint16_t xung_us;

    /* Khong cho goc vuot qua 180 do */
    if (goc > GOC_TOI_DA)
    {
        goc = GOC_TOI_DA;
    }

    /* Chuyen doi goc thanh do rong xung */
    xung_us = XUNG_MIN_US
            + ((uint32_t)goc * (XUNG_MAX_US - XUNG_MIN_US))
            / GOC_TOI_DA;

    /* Cap nhat PWM cho servo */
    PWM_SetServoPulse(xung_us);
}
