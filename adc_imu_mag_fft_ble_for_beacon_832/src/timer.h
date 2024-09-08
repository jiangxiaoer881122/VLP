/**
 * @file         timer.h
 * @details      用于timer 各类定义
 * @author       jjl
 */
#ifndef _TIMER_H
#define _TIMER_H

/*
===========================
头文件包括
===========================
*/
#include <nrfx_timer.h>
#include "imu_bag.h"
#include "adc.h"
#include <stdio.h>
#include <zephyr/kernel.h>
#include "nrfx_twi.h"
#include "nrfx_gpiote.h"
#include "iic_twi.h"
#include "icm_42688.h"
#include <zephyr/drivers/gpio.h>
#include <nrfx_gpiote.h>
#include "soft_iic.h"
#include "icm_42688.h"
#include "lis3dml.h"
#include "uart.h"
/*
===========================
宏定义
===========================
*/
/** @brief 选用哪个型号的定时器 */
#define TIMER_INST_IDX 2
#define TIMER2_INST_IDX 3
/** @brief 等待时间以us为单位*/
#define TIME_TO_WAIT_US 500UL
#define TIME2_TO_WAIT_US 500UL
/** @brief 等待时间以ms为单位*/
#define TIME_TO_WAIT_MS 50UL
// #define TIME2_TO_WAIT_MS 5UL
#define TIME2_TO_WAIT_MS 50UL
/*
===========================
变量声明
===========================
*/
typedef struct {
	uint8_t icm[15];
	uint8_t lis[15];
    int time;
} SensorData;


/*
===========================
函数声明
===========================
*/
/**
 * @brief 定时器事件处理函数
 *
 * @param[in] event_type 定时器的事件类型.
 * @param[in] p_context  传递的字节数
 */
void timer_handler(nrf_timer_event_t event_type, void * p_context);
void timer2_handler(nrf_timer_event_t event_type, void * p_context);
/**
 * @brief 定时器相关配置及开启
 *
 * @return Nothing.
 */
void timer1_init_enable(void);
void timer2_init_enable(void);
#endif