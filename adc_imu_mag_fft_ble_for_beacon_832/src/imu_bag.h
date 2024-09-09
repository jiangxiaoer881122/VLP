/**
 * @file         imu_bag.h
 * @details      用于定义imu与bag联合程序
 * @author       jjl
 */
#ifndef _IMU_BAG_H
#define _IMU_BAG_H

/*
===========================
头文件包括
===========================
*/
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <nrfx_gpiote.h>
#include "soft_iic.h"
#include "icm_42688.h"
#include "lis3dml.h"
#include "uart.h"
/*
===========================
宏定义
===========================
*/
/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000
/*
===========================
变量声明
===========================
*/
extern uint8_t icm42688_data[15];
extern uint8_t lis3dml_data[15];
extern int16_t acc_data[3];
extern int16_t gyro_data[3];
extern int16_t mag_data[3];
extern uint32_t icm42688_timestamp;

//声明消息队列
extern struct k_msgq uart_msgq2;
extern struct k_msgq uart_msgq3;
/*
===========================
函数声明
===========================
*/
/**
 * @brief icm (imu) 初始化写入命令
 */
void icm_init();
/**
 * @brief lis3dml (bag) 初始化写入命令
 */
void lis3dml_init();
/**
 * @brief read bytes to a sensor register 重新封装IIC的读取
 */
void sensor_register_read(uint8_t sensor_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);
/**
 * @brief Write a byte to a sensor register 重新封装IIC的写入
 */
void sensor_register_write_byte(uint8_t sensor_addr, uint8_t reg_addr, uint8_t data);
/**
 * @brief imu与bag 初始化程序
 */
void imu_bag_init(void);
/**
 * @brief imu与bag 读取程序
 */
void imu_bag_read_data(void);

#endif