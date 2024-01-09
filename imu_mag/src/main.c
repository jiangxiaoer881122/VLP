/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "imu_bag.h"
#include "timer.h"
#include "adc.h"

//以多线程的方式来进行数据的传输
#define STACK_SIZE 1024
#define PRIORITY 5
//定义线程的运行内存
K_THREAD_STACK_DEFINE(thread_stack_1,STACK_SIZE);
K_THREAD_STACK_DEFINE(thread_stack_2,STACK_SIZE);
//定义线程的结构对象用来创造
struct k_thread thread_data_1;
struct k_thread thread_data_2;

//定义线程的处理函数
//分别是adc 采样和imu数据
//adc_read_data();
//imu_bag_read_data();
void thread_entry_1(void *p1, void *p2, void *p3)
{
    // 在这里编写线程1的逻辑
	while(1)
	{
		adc_read_data();
		k_sleep(K_USEC(50));
	}

}
void thread_entry_2(void *p1, void *p2, void *p3)
{
    // 在这里编写线程2的逻辑
	while(1)
	{
		imu_bag_read_data();
		k_sleep(K_MSEC(50));
	}

}
int main(void)
{
	// 进行imu与bag的初始化
	imu_bag_init();
	// 进行adc初始化
	adc_init();
	// 进行定时器初始化 2k采样率 这里有问题会冲突
	//  timer1_init_enable();
	// 进行线程的创立
	k_thread_create(&thread_data_1, thread_stack_1,
					K_THREAD_STACK_SIZEOF(thread_stack_1),
					thread_entry_1, NULL, NULL, NULL,
					PRIORITY, 0, K_NO_WAIT);
	k_thread_create(&thread_data_2, thread_stack_2,
					K_THREAD_STACK_SIZEOF(thread_stack_2),
					thread_entry_2, NULL, NULL, NULL,
					PRIORITY, 0, K_NO_WAIT);
	// while (1)
	// {
	// 	// adc_read_data();
	// 	// k_sleep(K_SECONDS(1));
	// }

	return 0;
}
