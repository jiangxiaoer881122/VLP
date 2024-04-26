/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
int broadcaster_multiple(void);

int main(void)
{
	printk("Starting Multiple Broadcaster Demo\n");


	while (1)
	{
			(void)broadcaster_multiple();
			
			k_sleep(K_MSEC(200));
			//得先关闭蓝牙后在开启
			bt_disable();

			k_sleep(K_MSEC(800));
			
	}

	printk("Exiting %s thread.\n", __func__);
	
	return 0;
}
