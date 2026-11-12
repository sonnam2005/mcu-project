#include "ina219.h"
#include <stdlib.h>
#include <string.h>

static I2C_HandleTypeDef *s_hi2c;
#define INA219_I2C_TIMEOUT 100

// Các giá trị tính toán
static const float Current_LSB = 0.0001f;       /*100uA/LSB*/
static const float Power_LSB = 0.002f;          /*2mW/LSB*/

/*Các giá trị liên quan đến cấu hình*/
uint8_t config_rs[2] = {0x80, 0x00}; /*reset = 0x8000* gửi vào địa chỉ 0x00*/
uint8_t config_cal[2] = {0x10, 0x00}; /*cal gửi địa chỉ vào 0x05, R_shunt = 0.1 ohm */
uint8_t config_re[2] = {0x39, 0x9F};  /*cấu hình đọc max 26V và 3.2A*/

/*Cấu hình INA219*/
HAL_StatusTypeDef INA219_Init(I2C_HandleTypeDef *hi2c)
{
    s_hi2c = hi2c;
    if(HAL_I2C_Mem_Write(s_hi2c, INA219_ADDRESS, INA219_CONFIG, I2C_MEMADD_SIZE_8BIT, config_rs, 2, INA219_I2C_TIMEOUT ) != HAL_OK)
    {
        return HAL_ERROR;
    }
    HAL_Delay(100); /* chờ nguồn ổn định sau reset */

    if(HAL_I2C_Mem_Write(s_hi2c, INA219_ADDRESS, INA219_CALIBRATION, I2C_MEMADD_SIZE_8BIT, config_cal, 2, INA219_I2C_TIMEOUT ) != HAL_OK)
    {
        return HAL_ERROR;
    }
    if(HAL_I2C_Mem_Write(s_hi2c, INA219_ADDRESS, INA219_CONFIG, I2C_MEMADD_SIZE_8BIT, config_re, 2, INA219_I2C_TIMEOUT ) != HAL_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/*Đọc điện áp bus (V)*/
HAL_StatusTypeDef INA219_ReadBusVoltage(float *bus_voltage)
{
    uint8_t data[2];  /*Lưu MSB và LSB của điện áp bus*/
    if(HAL_I2C_Mem_Read(s_hi2c, INA219_ADDRESS, INA219_BUS_VOLTAGE, I2C_MEMADD_SIZE_8BIT, data, 2, INA219_I2C_TIMEOUT ) != HAL_OK)
    {
        return HAL_ERROR;
    }
    uint16_t raw_bus_voltage = (data[0] << 8) | data[1];
    raw_bus_voltage >>= 3;                   /*chuyển đổi để tính giá trị thật*/
    *bus_voltage = raw_bus_voltage * 0.004f; /*Mỗi bit tương ứng với 4mV - giá tri thật */
    return HAL_OK;
}

/*Đọc dòng điện (A)*/
HAL_StatusTypeDef INA219_ReadCurrent(float *current)
{
    uint8_t data[2];
    if(HAL_I2C_Mem_Read(s_hi2c, INA219_ADDRESS, INA219_CURRENT, I2C_MEMADD_SIZE_8BIT, data, 2, INA219_I2C_TIMEOUT ) != HAL_OK)
    {
        return HAL_ERROR;
    }
    uint16_t raw_current = (data[0] << 8) | data[1];
    *current = raw_current * Current_LSB; /*Mỗi bit tương ứng với 100uA - giá tri thật */
    return HAL_OK;
}

/*Đọc công suất (W)*/
HAL_StatusTypeDef INA219_ReadPower(float *power)
{
    uint8_t data[2];
    if(HAL_I2C_Mem_Read(s_hi2c, INA219_ADDRESS, INA219_POWER, I2C_MEMADD_SIZE_8BIT, data, 2, INA219_I2C_TIMEOUT ) != HAL_OK)
    {
        return HAL_ERROR;
    }
    uint16_t raw_power = (data[0] << 8) | data[1];
    *power = raw_power * Power_LSB; /*Mỗi bit tương ứng với 2mW - giá tri thật */
    return HAL_OK;
}

