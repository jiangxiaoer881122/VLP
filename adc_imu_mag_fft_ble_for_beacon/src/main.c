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
//定义IIC的设备名称
struct device *i2c_dev;
#define I2C_DEV_NAME "I2C_0"
//用于进行flag的调用
int flag =0;
//用于imu的数据发送
int count =0;
int imu_flag =0;
int b = 0;
int c = 1;


#include <stdio.h>
#include "zephyr/logging/log.h"
#include "nrfx_saadc.h"
#include <zephyr/kernel.h>


LOG_MODULE_REGISTER(adc, LOG_LEVEL_DBG);

#define CHANNEL_COUNT 1

static nrfx_saadc_channel_t single_channel = NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN2, 0);
static nrf_saadc_value_t buffer[CHANNEL_COUNT];

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
	// //用于计数
	// int count=0;
	// //用于存储格式化字符串
	// char str[10];
	// //字符指针
	// char *P;

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
		// 这里检测定时器代码
		if(flag)
		{
			flag=0;
			if(imu_flag)
			{
			imu_flag =0;
			imu_bag_read_data();
			}
		}else{
			//必须加否则会被优化
			c=1;
		}
	}

	return 0;
}





// #include "imu_bag.h"
// #include "timer.h"
// #include "adc.h"
// #include "iic_twi.h"
// #include "icm_42688.h"
// #include "uart.h"
// //定义IIC的设备名称
// struct device *i2c_dev;
// #define I2C_DEV_NAME "I2C_0"
// //用于进行flag的调用
// int flag =0;
// //用于imu的数据发送
// int count =0;
// int imu_flag =0;
// int b = 0;
// int c = 1;
//以多线程的方式来进行数据的传输
// #define STACK_SIZE 1024
// #define PRIORITY 5
//定义线程的运行内存
// K_THREAD_STACK_DEFINE(thread_stack_1,STACK_SIZE);
// K_THREAD_STACK_DEFINE(thread_stack_2,STACK_SIZE);
//定义线程的结构对象用来创造
// struct k_thread thread_data_1;
// struct k_thread thread_data_2;
// struct device *i2c_dev;
//定义线程的处理函数
//分别是adc 采样和imu数据
//adc_read_data();
//imu_bag_read_data();
// void thread_entry_1(void *p1, void *p2, void *p3)
// {
//     // 在这里编写线程1的逻辑
// 	while(1)
// 	{
// 		adc_read_data();
// 		k_sleep(K_USEC(500));
// 	}

// }
// void thread_entry_2(void *p1, void *p2, void *p3)
// {
//     // 在这里编写线程2的逻辑
// 	while(1)
// 	{
// 		imu_bag_read_data();
// 		k_sleep(K_MSEC(500));
// 	}

// }

// int main(void)
// {
// 	// //用于计数
// 	// int count=0;
// 	// //用于存储格式化字符串
// 	// char str[10];
// 	// //字符指针
// 	// char *ptr;

// 	//硬件IIC
// 	i2c_dev = device_get_binding(I2C_DEV_NAME);
// 	if(!i2c_dev)
// 	{
// 		printk("Failed\n");
// 		return 0;
// 	}else{
// 		printk("SUCCESS\n");
// 	}

// 	int err = i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_FAST));
//     if (err) {
//         printk("Failed to configure I2C device: %d\n", err);
//         return 0;
//     }
//     printk("I2C device configured successfully\n");

// 	// 进行imu与bag的初始化
// 	imu_bag_init();
// 	// 进行adc初始化
// 	adc_init();
// 	//进行串口初始化
// 	uart_init_slef();
// 	//进行定时器初始化 2k采样率 
//  	timer1_init_enable();
// 	// 进行线程的创立
// 	//这开启两个线程
// 	// k_thread_create(&thread_data_1, thread_stack_1,
// 	// 				K_THREAD_STACK_SIZEOF(thread_stack_1),
// 	// 				thread_entry_1, NULL, NULL, NULL,
// 	// 				PRIORITY, 0, K_NO_WAIT);
// 	// k_thread_create(&thread_data_2, thread_stack_2,
// 	// 				K_THREAD_STACK_SIZEOF(thread_stack_2),
// 	// 				thread_entry_2, NULL, NULL, NULL,
// 	// 				PRIORITY, 0, K_NO_WAIT);
// 	while (1)
// 	{
// 		// printk("System clock frequency :%d",sys_clock_hw_cycles_per_sec);	
// 		//将整数格式化成指针
// 		// sprintf(str,"%d\n",count);
// 		// ptr =str;
// 		// print_uart(ptr);
// 		// k_sleep(K_MSEC(1));
// 		// count++;

// 		// 这里检测定时器代码
// 		if(flag)
// 		{
// 			flag=0;
// 			adc_read_data();
// 			if(imu_flag)
// 			{
// 			imu_flag =0;
// 			imu_bag_read_data();
// 			}
// 		}else{
// 			//必须加否则会被优化
// 			c=1;
// 		}
// 	}

// 	return 0;
// }