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
#include "nrfx_twi.h"
#include "nrfx_twim.h"
#include "nrfx_gpiote.h"
#include <zephyr/irq.h>
#include "ads1015.h"

#define TWI_INSTANCE_ID     0
#define TWI_SCL_PIN         NRF_GPIO_PIN_MAP(0, 27)
#define TWI_SDA_PIN			NRF_GPIO_PIN_MAP(0, 26)
//定义一个发送完成的变量
bool twi_done=true;
// 定义 TWI 实例结构体
nrfx_twi_t m_twi = NRFX_TWI_INSTANCE(TWI_INSTANCE_ID);
// void twi_handler(nrfx_twi_evt_t const * p_event, void * p_context) 
// {
// 	switch (p_event->type) {
// 		case NRFX_TWI_EVT_DONE:
// 			/* Transfer completed */
// 			twi_done=true;
// 			break;
// 		case NRFX_TWI_EVT_ADDRESS_NACK:
// 			/* NACK received after sending the address */
// 			printk("address nack");
// 			break;
// 		case NRFX_TWI_EVT_DATA_NACK:
// 			/* NACK received after sending a data byte */
// 			printk("data nack");
// 			break;
// 		default:
// 			break;
// 	}
// }
// 初始化 TWI
void twi_init(void)
{
	nrfx_err_t err_code;
    // 配置 TWI
    const nrfx_twi_config_t twi_config = {
        .scl = TWI_SCL_PIN,
        .sda = TWI_SDA_PIN,
        .frequency = NRF_TWI_FREQ_400K,
        .interrupt_priority = 4,
        .hold_bus_uninit = false,
    };
    err_code = nrfx_twi_init(&m_twi, &twi_config, NULL, NULL);
	// IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TWI_INST_GET(0)), IRQ_PRIO_LOWEST,
    //                    NRFX_TWI_INST_HANDLER_GET(0), 0);
	// irq_enable(NRFX_IRQ_NUMBER_GET(NRF_TWI_INST_GET(0)));
	// printk("IRQ_PRIO_LOWEST:%d\n",IRQ_PRIO_LOWEST);
	// printk("NRFX_IRQ_NUMBER_GET(NRF_TWI_INST_GET(0)):%d\n",	NRFX_IRQ_NUMBER_GET(NRF_TWI_INST_GET(0)));
	// printk("BIT:%d\n",	 (_IRQ_PRIO_OFFSET));
	// printk("BIT:%d\n",	 twi_config.interrupt_priority);
    // 使能 TWI
    nrfx_twi_enable(&m_twi);
}

//定义IIC的设备名称
// struct device *i2c_dev;
// #define I2C_DEV_NAME "I2C_0"
//用于进行flag的调用
int flag =0;
//存储一定的AD进而来进行在一定下进行输送
int pd[1000];
int pd2[1000];
//存储一定的imu进行来输送
int imu_count=0;
//用于imu的数据发送
// int count_imu =0;
// int imu_flag =0;
//这里是0.5秒的计时器
int count =0;
int b = 0;
int c = 1;
int imu_flag =0;

//以下是大的时间戳 0.5秒计时一次 0-65536
u_int16_t big_time = 0;
//MovingFlag 与beacondeviceType
uint8_t Mov_type = 0; 
//Moving flag 0为不动 1为动
uint8_t Moving_flag = 0;
//标志不同类型
uint8_t beacon_type = 0;
//标志接受端的电池容量
uint8_t voltage = 0;
//标志接收端的imu的时间戳
int small_time = 0; 

//以下是nrf库的adc采样
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
	single_channel.channel_config.gain = NRF_SAADC_GAIN1_4;
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
	//重新补充一下
	Bsp_HFCLK_Init_Extern();
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
	char str[200];
	// //字符指针
	char *P;
	int i=0,offset=0;
	//测试
	int ADC=0;
	// //硬件IIC zephry库的
	// i2c_dev = device_get_binding(I2C_DEV_NAME);
	// if(!i2c_dev)
	// {
	// 	printk("Failed\n");
	// 	return 0;
	// }else{
	// 	printk("SUCCESS\n");
	// }

	// int err = i2c_configure(i2c_dev, I2C_SPEED_SET(I2C_SPEED_FAST));
    // if (err) {
    //     printk("Failed to configure I2C device: %d\n", err);
    //     return 0;
    // }
    // printk("I2C device configured successfully\n");

	//iic nrf库
	twi_init();
	// 进行imu与bag的初始化
	// imu_bag_init();
	// 进行adc初始化
	// adc_init();
	// adc_init2();
	//进行串口初始化
	uart_init_slef();
	// bt_disable();
	// broadcaster_multiple();
	//进行定时器初始化 2k采样率 
 	timer1_init_enable(); 
	// //进行定时器初始化 20hz
	// timer2_init_enable();

	while (1)
	{
			if(flag)
			{
			//这代表0.5秒时间触发了
			// big_time++;
			//进行FFT处理
			// fft();
			//进行一个校准确保是10imu数据
			//然后复位
			//进行数据的更新
			// while(imu_flag==0);
			// imu_flag=0;
			// ble_data_update();
			//清零
			// flag=0;
			// }
		}else{
			//必须加否则会被优化
			c=1;
		}
	}

	return 0;
}





