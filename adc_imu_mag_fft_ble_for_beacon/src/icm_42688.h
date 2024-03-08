/**
 * @file         icm_42688.h
 * @details      用于定义icm42688寄存器地址
 * @author       zjs
 */
#ifndef _ICM_42688_H_
#define _ICM_42688_H_

/*
===========================
头文件包括
===========================
*/
#include <stdio.h>
#include <stdint.h>

#define ICM_I2C_ADDR UINT8_C(0x68)
#define ICM_ID 0x47

#define ICM_POWER_MAG 0x4E
#define ICM_DEVICE_CONFIG 0x11
#define ICM_DRIVE_CONFIG 0x13
#define ICM_WHO_AM_I UINT8_C(0x75)
#define ICM_TEMP_DATA1 0x1D
#define ICM_TEMP_DATA0 0x1E
#define ICM_GYRO_CONFIG0 0x4F
#define ICM_ACCEL_CONFIG0 0x50
#define ICM_GYRO_CONFIG1 0x51
#define ICM_GYRO_ACCEL_CONFIG0 0x52
#define ICM_ACCEL_CONFIG1 0x53
#define ICM_XA_ST_DATA 0x3B
#define ICM_YA_ST_DATA 0x3C
#define ICM_ZA_ST_DATA 0x3D
#define ICM_BANK_SEL 0x76

#define ICM_ACCEL_DATA_X1 0x1F
#define ICM_ACCEL_DATA_X0 0x20

#define ICM_ACCEL_DATA_Y1 0x21
#define ICM_ACCEL_DATA_Y0 0x22

#define ICM_ACCEL_DATA_Z1 0x23
#define ICM_ACCEL_DATA_Z0 0x24

#define ICM_GYRO_DATA_X1 0x25
#define ICM_GYRO_DATA_X0 0x26

#define ICM_GYRO_DATA_Y1 0x27
#define ICM_GYRO_DATA_Y0 0x28

#define ICM_GYRO_DATA_Z1 0x29
#define ICM_GYRO_DATA_Z0 0x2A

#define ICM_FIFO_CONFIG_INIT 0x16
#define ICM_FIFO_CONFIGURATION 0x5F
#define ICM_FIFO_DATA_REG 0x30

#define ICM_SIGNAL_PATH_RESET 0x4B

#define ICM_TMST_CONFIG 0x54
#define ICM_TMSTVAL0 0x62
// void configure_device();
// void icm_initialize();
// void read_values();

/*
===========================
函数声明
===========================
*/

#endif