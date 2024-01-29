/**
 * @file         lis3dml.h
 * @details      用于定义lis3dml寄存器地址
 * @author       zjs
 */
#ifndef _LIS3DML_H_
#define _LIS3DML_H_
/*
===========================
头文件包括
===========================
*/
#include <stdio.h>
#include <stdint.h>

#define LIS3MDL_I2C_ADDR_V 0x1E /* SA1 -> VDD */
#define LIS3MDL_I2C_ADDR_G 0x1C /* SA1 -> GND */

#define LIS3MDL_WHO_AM_I 0x0F
#define LIS3MDL_ID 0x3D

#define LIS3MDL_CTRL_REG1 0x20
#define LIS3MDL_CTRL_REG2 0x21
#define LIS3MDL_CTRL_REG3 0x22
#define LIS3MDL_CTRL_REG4 0x23
#define LIS3MDL_CTRL_REG5 0x24

#define LIS3MDL_OUT_X_L 0x28
#define LIS3MDL_OUT_X_H 0x29
#define LIS3MDL_OUT_Y_L 0x2A
#define LIS3MDL_OUT_Y_H 0x2B
#define LIS3MDL_OUT_Z_L 0x2C
#define LIS3MDL_OUT_Z_H 0x2D
/*
===========================
函数声明
===========================
*/

#endif