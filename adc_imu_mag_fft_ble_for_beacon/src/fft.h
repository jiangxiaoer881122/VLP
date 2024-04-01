/**
 * @file         fft.h
 * @details      用于定义FFT
 * @author       jjl
 */
#ifndef _FFT_H
#define _FFT_H
/*
===========================
头文件包括
===========================
*/
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include "nrfx_timer.h"
#include <nrfx_gpiote.h>
#include <arm_math.h>
#include <arm_const_structs.h>
#include "uart.h"
// #include <support_functions.h>
/*
===========================
宏定义包括
===========================
*/

/* FFT长度 */
#define FFT_LENGTH 2048


/*
===========================
函数申明包括
===========================
*/
/**
 * @brief        fft运作
 */
int fft(void);
#endif