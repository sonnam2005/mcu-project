#ifndef SSD1306_H
#define SSD1306_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>

/* Địa chỉ 7-bit 0x3C, HAL yêu cầu dịch trái 1 bit -> 0x78 */
#define SSD1306_I2C_ADDR    (0x3C << 1)

#define SSD1306_WIDTH       128      
#define SSD1306_HEIGHT      64
#define SSD1306_PAGES       (SSD1306_HEIGHT / 8)

/*đặt 2 nếu ảnh bị lệch 2 pixel */
#define SSD1306_COLUMN_OFFSET   0

typedef enum {
    SSD1306_BLACK = 0,     
    SSD1306_WHITE = 1
} SSD1306_Color; 

HAL_StatusTypeDef SSD1306_Init(I2C_HandleTypeDef *hi2c);  /*Khởi tạo OLED*/
void SSD1306_Fill(SSD1306_Color color);
HAL_StatusTypeDef SSD1306_UpdateScreen(void);

void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_Color color);      /*Bật tắt 1 điểm ảnh*/

void SSD1306_SetCursor(uint8_t x, uint8_t y);    /*đặt vị trí bắt đầu cho ký tự tiếp theo*/

void SSD1306_WriteChar(char c, SSD1306_Color color);
void SSD1306_WriteString(const char *str, SSD1306_Color color);

/*Được gọi để điều khiển hiển thị*/
HAL_StatusTypeDef SSD1306_SetDisplay(uint8_t on);       /* 1 = bật, 0 = tắt */   
HAL_StatusTypeDef SSD1306_SetContrast(uint8_t contrast); /* độ sáng 0..255 */
HAL_StatusTypeDef SSD1306_Invert(uint8_t invert);        /* 1 = đảo màu */

#ifdef __cplusplus
}
#endif

#endif /* SSD1306_H */