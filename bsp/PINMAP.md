# Bảng nối chân STM32

| Thiết bị      | Chân | Chức năng |
|---------------|------|-----------|
| OLED + INA219 | PB6  | I2C1_SCL  |
| OLED + INA219 | PB7  | I2C1_SDA  |
| Servo trục 1  | PA0  | TIM2_CH1  |
| Servo trục 2  | PA1  | TIM2_CH2  |
| LDR 1         | PA4  | ADC1_IN4  |
| LDR 2         | PA5  | ADC1_IN5  |

Ghi chú: servo 5V cấp nguồn riêng, nối chung GND với STM32.
