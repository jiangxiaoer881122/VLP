#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include "nrfx_timer.h"
#include <nrfx_gpiote.h>
#include <arm_math.h>
#include <arm_const_structs.h>

/* FFT长度 */
#define FFT_LENGTH 2048
// /* LED灯 */
// #define LED_IO_NUM 6
/* FFT信号数组，包含实部和虚部 */
 float fft_input_buf[FFT_LENGTH * 2];
/* FFT输出结果数组，幅度输出 */
 float fft_output_mag_buf[FFT_LENGTH];
 arm_cfft_instance_f32 scfft;


int main(void)
{

	/* ===================LED指示灯=================== */
	// nrf_gpio_cfg_output(LED_IO_NUM);
	// nrf_gpio_pin_clear(LED_IO_NUM);
	/* ===================定时器部分=================== */

	/* ===================fft部分=================== */
	/* 初始化scfft结构体，设定FFT相关参数*/
	arm_cfft_init_f32(&scfft, FFT_LENGTH);
	int i;
	/* 生成信号序列 */
	for ( i = 0; i < FFT_LENGTH; i++)
	{
		/* 实部，生成频率为100Hz和150Hz合成信号 */
		fft_input_buf[2 * i] = 10 * arm_sin_f32(2 * PI * i * 100 / FFT_LENGTH) +
							   5 * arm_sin_f32(2 * PI * i * 150 / FFT_LENGTH);
		/* 虚部 */
		fft_input_buf[2 * i + 1] = 0;
	}
	//输出i
	printk("i:%d\n",i);
	/* FFT计算 */
	arm_cfft_f32(&scfft, fft_input_buf, 0, 1);
	 /* 运算结果求幅值 */
	arm_cmplx_mag_f32(fft_input_buf, fft_output_mag_buf, FFT_LENGTH);
	printk("i:%d\n",i+1);
	for (int i = 0; i < FFT_LENGTH; i++)
	{
		printk("%d.%d, ", (uint16_t)(fft_output_mag_buf[i]), (uint16_t)((fft_output_mag_buf[i] - (uint16_t)(fft_output_mag_buf[i])) * 100.0));
		k_msleep(10);
		if ((i + 1) % 32 == 0)
		{
			printk("\n");
		}
	}
	printk("fft end\n");

	while (1)
	{
		k_msleep(30);
		// nrf_gpio_pin_toggle(LED_IO_NUM);\
			/* FFT计算 */
		arm_cfft_f32(&scfft, fft_input_buf, 0, 1);
		/* 运算结果求幅值 */
		arm_cmplx_mag_f32(fft_input_buf, fft_output_mag_buf, FFT_LENGTH);
		k_msleep(500);
		/* 生成信号序列 */
		for (int i = 0; i < FFT_LENGTH; i++)
		{
			/* 实部，生成频率为100Hz和150Hz合成信号 */
			fft_input_buf[2 * i] = 10 * arm_sin_f32(2 * PI * i * 100 / FFT_LENGTH) +
								   5 * arm_sin_f32(2 * PI * i * 150 / FFT_LENGTH);
			/* 虚部 */
			fft_input_buf[2 * i + 1] = 0;
		}
		// printk("led toggle\n");
	}

	return 0;
}
