#ifndef _SOFT_IIC_H_
#define _SOFT_IIC_H_

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <nrfx_gpiote.h>
//硬件IIC所需头文件
#include "iic_twi.h"

#define ACK_VAL 0x0
#define NACK_VAL 0x1
#define ACK_CHECK_EN 0x1
#define ACK_CHECK_DIS 0x0

// #define ESP32_GPIO_OUT_W1TS(gpio_num) (GPIO.out_w1ts = (0x01 << gpio_num)) // gpio_num:0 ~ 31    GPIO OUT HIGH
// #define ESP32_GPIO_OUT_W1TC(gpio_num) (GPIO.out_w1tc = (0x01 << gpio_num)) // gpio_num:0 ~ 31    GPIO OUT LOW

// #define ESP32_GPIO_OUT1_W1TS(gpio_num) (GPIO.out1_w1ts.data = (0x01 << (gpio_num - 32))) // gpio_num:32 ~ 39   GPIO OUT HIGH
// #define ESP32_GPIO_OUT1_W1TC(gpio_num) (GPIO.out1_w1tc.data = (0x01 << (gpio_num - 32))) // gpio_num:32 ~ 39   GPIO OUT LOW

#define ESP32_GPIO_IN(gpio_num) nrf_gpio_pin_read(gpio_num) // gpio_num:0 ~ 31    GPIO IN VALUE
// #define ESP32_GPIO_IN1(gpio_num) ((GPIO.in1.data >> gpio_num) & 0x1) // gpio_num:32 ~ 39   GPIO IN VALUE

#define GpioReadBit(gpio_num) ESP32_GPIO_IN(gpio_num)

#define GpioSetBit(gpio_num) nrf_gpio_pin_write(gpio_num, 1)

#define GpioResetBit(gpio_num) nrf_gpio_pin_write(gpio_num, 0)

#define I2C_SCL_PIN 27
#define I2C_SDA_PIN 26

// #define I2C_SCL_PIN 5  /*!< GPIO number used for I2C master clock */
// #define I2C_SDA_PIN 18 /*!< GPIO number used for I2C master data  */

#define I2C_SDA_HIGH GpioSetBit(I2C_SDA_PIN)
#define I2C_SDA_LOW GpioResetBit(I2C_SDA_PIN)

#define I2C_SCL_HIGH GpioSetBit(I2C_SCL_PIN)
#define I2C_SCL_LOW GpioResetBit(I2C_SCL_PIN)

#define I2C_SDA GpioReadBit(I2C_SDA_PIN)

//定义的IIC设备标签（硬件IIC所需）
extern struct device *i2c_dev;

// #define I2C_SDA				                gpio_get_level(I2C_SDA_PIN)

void I2cInit(void);

void I2cStart(void);

void I2cStop(void);

uint8_t I2cWaitAck(void);

void I2cAck(void);

void I2cNack(void);

void I2cWriteByte(uint8_t Data);

uint8_t I2cReadByte(uint8_t isAck);

void EspI2cWriteByte(uint8_t dev_addr,
                     uint8_t reg_addr,
                     uint8_t write_data);

void EspI2cReadByte(uint8_t dev_addr,
                    uint8_t reg_addr,
                    uint8_t *read_data);

void EspI2cWriteBytes(uint8_t dev_addr,
                      uint8_t reg_addr,
                      uint8_t *write_data,
                      uint8_t write_len);

void EspI2cReadBytes(uint8_t dev_addr,
                     uint8_t reg_addr,
                     uint8_t *read_data,
                     uint8_t read_len);

void I2cDetect(void);

#endif