/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "imu_bag.h"
#include "timer.h"
#include "adc.h"
int main(void)
{
	//进行imu与bag的初始化
	imu_bag_init();
	//进行adc初始化
	adc_init();
	//进行定时器初始化 2k采样率 这里有问题会冲突
	timer1_init_enable();
	while (1)
	{
		// adc_read_data();
		// k_sleep(K_SECONDS(1));
	}

	return 0;
}
