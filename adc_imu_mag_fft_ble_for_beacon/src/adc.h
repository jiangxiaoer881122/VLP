/**
 * @file         adc.h
 * @details      用于定义adc采样API
 * @author       jjl
 */
#ifndef _ADC_H
#define _ADC_H
/*
===========================
头文件包括
===========================
*/
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
/*
===========================
宏定义包括
===========================
*/
#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
    !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
    ADC_DT_SPEC_GET_BY_IDX(node_id, idx),
/*
===========================
函数申明包括
===========================
*/
/**
 * @brief        adc初始化配置
 */
int adc_init(void);
/**
 * @brief        adc读取数据配置
 */
void adc_read_data(void);
#endif