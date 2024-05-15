/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "imu_bag.h"



uint8_t data[15];
uint8_t icm42688_data[15];
uint8_t lis3dml_data[15];
int16_t acc_data[3] = {0};
int16_t gyro_data[3] = {0};
int16_t mag_data[3] = {0};
uint32_t icm42688_timestamp = 0;
/**
 * @brief Read a sequence of bytes from a sensor registers 重新封装IIC的读取
 * 
 */
static void sensor_register_read(uint8_t sensor_addr, uint8_t reg_addr, uint8_t *data, uint8_t len)
{
	EspI2cReadBytes(sensor_addr, reg_addr, data, len);
}
/**
 * @brief Write a byte to a sensor register 重新封装IIC的写入
 */
static void sensor_register_write_byte(uint8_t sensor_addr, uint8_t reg_addr, uint8_t data)
{
	EspI2cWriteByte(sensor_addr, reg_addr, data);
}
/**
 * @brief icm (imu) 初始化写入命令
 */
void icm_init()
{
	sensor_register_write_byte(ICM_I2C_ADDR, ICM_BANK_SEL, 0);
	k_msleep(10);
	/* 读WHO_AM_I寄存器 */
	sensor_register_read(ICM_I2C_ADDR, ICM_WHO_AM_I, data, 1);
	printk("icm42688 read id:%X\n", data[0]);
	if (data[0] == ICM_ID)
	{
		printk("icm42688 ready!\n");
	}
	else
	{
		printk("icm42688 not found!\n");
	}
	/* 配置寄存器 */
	sensor_register_write_byte(ICM_I2C_ADDR, ICM_DEVICE_CONFIG, 0x01); // 软重启
	k_msleep(10);													   // 等待10ms

	sensor_register_write_byte(ICM_I2C_ADDR, ICM_POWER_MAG, 0x1F); // 开启陀螺仪和加速度计
	k_msleep(100);

	/* 配置FS和ODR */
	// sensor_register_write_byte(ICM_I2C_ADDR, ICM_GYRO_CONFIG0, 0x67); // 250dps,200Hz
	sensor_register_write_byte(ICM_I2C_ADDR, ICM_GYRO_CONFIG0, 0x66); // 250dps,1000Hz
	k_msleep(50);
	// sensor_register_write_byte(ICM_I2C_ADDR, ICM_ACCEL_CONFIG0, 0x47); // 4g, 100Hz（这里应为200Hz）
	sensor_register_write_byte(ICM_I2C_ADDR, ICM_ACCEL_CONFIG0, 0x46); // 4g, 1000Hz
	k_msleep(50);
	sensor_register_write_byte(ICM_I2C_ADDR, ICM_GYRO_ACCEL_CONFIG0, 0x55); // 20Hz BW
	k_msleep(50);
	printk("icm42688 gyr and acc config finished!");

	/* 开启时间戳 */
	sensor_register_write_byte(ICM_I2C_ADDR, ICM_TMST_CONFIG, 0x33);
	k_msleep(50);
}
/**
 * @brief lis3dml (bag) 初始化写入命令
 */
void lis3dml_init()
{
	/* 读WHO_AM_I寄存器 */
	sensor_register_read(LIS3MDL_I2C_ADDR_G, LIS3MDL_WHO_AM_I, data, 1);
	printk("lis3dml read id:%X\n", data[0]);
	if (data[0] == LIS3MDL_ID)
	{
		printk("lis3dml ready!");
	}
	else
	{
		printk("lis3dml not found!");
	}
	/* 软重启 */
	sensor_register_write_byte(LIS3MDL_I2C_ADDR_G, LIS3MDL_CTRL_REG2, 0x04);
	/* 延时 */
	k_msleep(50);
	/* 配置BDU */
	sensor_register_write_byte(LIS3MDL_I2C_ADDR_G, LIS3MDL_CTRL_REG5, 0x40);
	/* 延时 */
	k_msleep(50);
	/* 配置 */
	// sensor_register_write_byte(LIS3MDL_I2C_ADDR_G, LIS3MDL_CTRL_REG1, 0x60 | 0x1C); // X、Y轴high performance，80Hz(这个不太对应该是ultra-high-perfermance)
	sensor_register_write_byte(LIS3MDL_I2C_ADDR_G, LIS3MDL_CTRL_REG1, 0x40 | 0x02); // X、Y轴high performance，300H performance，80Hz
	sensor_register_write_byte(LIS3MDL_I2C_ADDR_G, LIS3MDL_CTRL_REG2, 0x00);		// ±4 gauss
	sensor_register_write_byte(LIS3MDL_I2C_ADDR_G, LIS3MDL_CTRL_REG3, 0x00);		// 连续转换模式
	sensor_register_write_byte(LIS3MDL_I2C_ADDR_G, LIS3MDL_CTRL_REG4, 0x0C);		// Z轴high performance
}
/**
 * @brief imu与bag 初始化程序
 */
 void imu_bag_init(void)
{
	I2cInit();
	/* 初始化icm42688 */
	icm_init();
	/* 初始化lis3dml */
	lis3dml_init();
}
/**
 * @brief imu与bag 读取程序
 */
void imu_bag_read_data( void)
{
	//用于存储格式化字符串
	char str_42688[120];
	char str_3dml[120];
	// 字符指针
	char *ptr_42688;
	char *ptr_3dml;

		/* ICM-42688-P 读数据 */
		sensor_register_write_byte(ICM_I2C_ADDR, ICM_BANK_SEL, 0);
		sensor_register_write_byte(ICM_I2C_ADDR, ICM_SIGNAL_PATH_RESET, 0x04);
		sensor_register_read(ICM_I2C_ADDR, ICM_ACCEL_DATA_X1, icm42688_data, 12);
		sensor_register_write_byte(ICM_I2C_ADDR, ICM_BANK_SEL, 1);
		// k_usleep(10);
		sensor_register_read(ICM_I2C_ADDR, ICM_TMSTVAL0, icm42688_data + 12, 3);

		/* LIS3DML读数据 */
		sensor_register_read(LIS3MDL_I2C_ADDR_G, LIS3MDL_OUT_X_L, lis3dml_data, 6);

		/* ICM42688打印数据 */
		acc_data[0] = (icm42688_data[0] << 8) + icm42688_data[1];
		acc_data[1] = (icm42688_data[2] << 8) + icm42688_data[3];
		acc_data[2] = (icm42688_data[4] << 8) + icm42688_data[5];

		gyro_data[0] = (icm42688_data[6] << 8) + icm42688_data[7];
		gyro_data[1] = (icm42688_data[8] << 8) + icm42688_data[9];
		gyro_data[2] = (icm42688_data[10] << 8) + icm42688_data[11];

		icm42688_timestamp = (icm42688_data[14] << 16) + (icm42688_data[13] << 8) + icm42688_data[12];

		// /* LIS3DML打印数据 */
		mag_data[0] = lis3dml_data[0] + (lis3dml_data[1] << 8);
		mag_data[1] = lis3dml_data[2] + (lis3dml_data[3] << 8);
		mag_data[2] = lis3dml_data[4] + (lis3dml_data[5] << 8);

		// 将上面两行数据直接用串口打印
		// 将整数格式化成指针
		sprintf(str_42688,"icm42688, acc_x:%d, acc_y:%d, acc_z:%d, gyr_x:%d, gyr_y:%d, gyr_z:%d, tick:%ld \n",acc_data[0], acc_data[1], acc_data[2],
			   gyro_data[0], gyro_data[1], gyro_data[2],(long int)icm42688_timestamp);
		// sprintf(str_42688,"tick:%ld\n",(long int)icm42688_timestamp);
		ptr_42688 = str_42688;
		print_uart(ptr_42688);
		// memset(str_42688,0,sizeof(str_42688));

		sprintf(str_3dml,"lis3dml, mag_x:%d, mag_y:%d, mag_z:%d\n",
			   mag_data[0], mag_data[1], mag_data[2]);
		ptr_3dml = str_3dml;
		print_uart(ptr_3dml);
		// memset(str_3dml,0,sizeof(str_3dml));
}
