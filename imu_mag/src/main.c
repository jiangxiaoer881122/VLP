/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "imu_bag.h"
#include "timer.h"
#include "adc.h"
#include "iic_twi.h"
#include "icm_42688.h"
#include "uart.h"
#include <stdio.h>
#include "zephyr/logging/log.h"
#include "nrfx_saadc.h"
#include <zephyr/kernel.h>

//定义IIC的设备名称
struct device *i2c_dev;
#define I2C_DEV_NAME "I2C_0"
//用于进行flag的调用
int flag =0;
//存储一定的AD进而来进行在一定下进行输送
int pd[100];
//用于imu的数据发送
int count =0;
int imu_flag =0;
int b = 0;
int c = 1;
//用于imu_bag计数
int a=0;
char a_a[10];
LOG_MODULE_REGISTER(adc, LOG_LEVEL_DBG);
#define CHANNEL_COUNT 1

static nrfx_saadc_channel_t single_channel = NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN2, 0);
static nrf_saadc_value_t buffer[CHANNEL_COUNT];
/*时钟校准*/
void Bsp_HFCLK_Init_Extern()
{
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;

    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
        // Do nothing.
    }
}

int adc_init2(void)
{
	Bsp_HFCLK_Init_Extern();
	nrfx_err_t err;
	//初始化 输入初始化的中断优秀级
	err = nrfx_saadc_init(NRFX_SAADC_DEFAULT_CONFIG_IRQ_PRIORITY);
	//配置通道的属性
	single_channel.channel_config.gain = NRF_SAADC_GAIN1_6;
	//显示通道的采样时间间隔
	LOG_INF("TIME=%d\r\n",single_channel.channel_config.acq_time);
	//nrfx_saadc_channel_config完成通道的配置
	err = nrfx_saadc_channel_config(&single_channel);
	//nrfx_saadc_channels_configured_get();记录了当前配置的通道配置情况
	uint32_t channels_mask = nrfx_saadc_channels_configured_get();
	LOG_INF("ADC channels mask: %d\n", channels_mask);
	//设置通道分辨率为12bit，不使用过采样，事件回调
	err = nrfx_saadc_simple_mode_set(channels_mask, NRF_SAADC_RESOLUTION_12BIT, NRF_SAADC_OVERSAMPLE_DISABLED, NULL);
	printk("this %d\n",err);
	//在开始 SAADC 转换之前，您需要调用此函数来设置一个缓冲区，SAADC 将把采样数据存储到这个缓冲区中。
	err = nrfx_saadc_buffer_set(buffer, CHANNEL_COUNT);
}

int adc_value_get(void)
{
	nrfx_err_t err;
	//nrfx_saadc_offset_calibrate(NULL);校准ADC的偏移量，这里不使用回调函数。
	err = nrfx_saadc_offset_calibrate(NULL);
	//nrfx_saadc_mode_trigger();开始采集数据。
	err = nrfx_saadc_mode_trigger();
	return buffer[0];
}
//转换成电压
int adc_to_voltage(int adc_value)
{
	return (adc_value * 3600) / 4096;
}

int main(void)
{
	// //用于存储格式化字符串
	char str[200];
	// //字符指针
	char *P;
	int i=0,offset=0;

	//硬件IIC
	i2c_dev = device_get_binding(I2C_DEV_NAME);
	if(!i2c_dev)
	{
		printk("Failed\n");
		return 0;
	}else{
		printk("SUCCESS\n");
	}
	int err = i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_FAST));
    if (err) {
        printk("Failed to configure I2C device: %d\n", err);
        return 0;
    }
    printk("I2C device configured successfully\n");
	// 进行imu与bag的初始化
	imu_bag_init();
	// 进行adc初始化
	adc_init2();
	//进行串口初始化
	uart_init_slef();
	//进行定时器初始化 2k采样率 
 	timer1_init_enable();
	while (1)
	{
			if(imu_flag)
			{
			imu_flag =0;
			imu_bag_read_data();
			// for(i=0;i<100;i++)
			// {
			// 	offset += sprintf(str + offset, "%d,", pd[i]); // 将整数转换为字符串并拼接到str中
			// }
			// P=str;
			// print_uart(P);
			// offset=0;
			// }
		}else{
			//必须加否则会被优化
			c=1;
		}
	}
	return 0;
}
