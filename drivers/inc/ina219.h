#ifndef INA219_H
#define INA219_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>

// INA219 Địa chỉ I2C mặc định (7-bit)
#define INA219_ADDRESS           (0x40 << 1) // HAL yêu cầu dịch trái 1 bit -> 0x80

// INA219 Register Addresses
#define INA219_CONFIG         0x00      /*Config*/
#define INA219_SHUNT_VOLTAGE  0x01      /*Điện áp shunt */     
#define INA219_BUS_VOLTAGE    0x02      /*Điện áp bus */
#define INA219_POWER          0x03      /*Công suất */
#define INA219_CURRENT        0x04      /*Dòng điện */
#define INA219_CALIBRATION    0x05      /*Hiệu chuẩn dòng và công suất*/


HAL_StatusTypeDef INA219_Init(I2C_HandleTypeDef *hi2c);            /*Khởi tạo INA219*/
HAL_StatusTypeDef INA219_ReadBusVoltage(float *bus_voltage);   /*Đọc điện áp bus (V)*/
HAL_StatusTypeDef INA219_ReadCurrent(float *current);              /*Đọc dòng điện (A)*/ 
HAL_StatusTypeDef INA219_ReadPower(float *power);                  /*Đọc công suất (W)*/

#ifdef __cplusplus
}
#endif

#endif // INA219_H

/* đọc giá trị raw ra kết quả đúng nhưng không đọc được giá trị thật sau khi đổi*/