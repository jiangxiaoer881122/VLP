/**
 * @file         ble.h
 * @details      用于ble数据传播
 * @author       jjl
 */
#ifndef _BLE_H
#define _BLE_H
/*
===========================
头文件包括
===========================
*/
#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
/*
===========================
宏定义包括
===========================
*/
#if defined(CONFIG_BT_CTLR_ADV_DATA_LEN_MAX)
#define BT_AD_DATA_LEN_MAX CONFIG_BT_CTLR_ADV_DATA_LEN_MAX
#else
#define BT_AD_DATA_LEN_MAX 31U
#endif

/* Size of AD data format length field in octets */
#define BT_AD_DATA_FORMAT_LEN_SIZE 1U

/* Size of AD data format type field in octets */
#define BT_AD_DATA_FORMAT_TYPE_SIZE 1U

/* Maximum value of AD data format length field (8-bit) */
#define BT_AD_DATA_FORMAT_LEN_MAX 255U

/* Device name length, size minus one null character */
#define BT_DEVICE_NAME_LEN (sizeof(CONFIG_BT_DEVICE_NAME) - 1U)

/* Device name length in AD data format, 2 bytes for length and type overhead */
#define BT_DEVICE_NAME_AD_DATA_LEN (BT_AD_DATA_FORMAT_LEN_SIZE + \
				    BT_AD_DATA_FORMAT_TYPE_SIZE + \
				    BT_DEVICE_NAME_LEN)

/* Maximum manufacturer data length, considering ad data format overhead and
 * the included device name in ad data format.
 */
#define BT_MFG_DATA_LEN_MAX       (MIN((BT_AD_DATA_FORMAT_LEN_MAX - \
					BT_AD_DATA_FORMAT_TYPE_SIZE), \
				       (BT_AD_DATA_LEN_MAX - \
					BT_AD_DATA_FORMAT_LEN_SIZE - \
					BT_AD_DATA_FORMAT_TYPE_SIZE)))
#define BT_MFG_DATA_LEN           (MIN(BT_MFG_DATA_LEN_MAX, \
				       (BT_AD_DATA_LEN_MAX - \
					BT_AD_DATA_FORMAT_LEN_SIZE - \
					BT_AD_DATA_FORMAT_TYPE_SIZE - \
					BT_DEVICE_NAME_AD_DATA_LEN)))
/*
===========================
函数申明包括
===========================
*/
/**
 * @brief        蓝牙开始广播
 */
int broadcaster_multiple(void);
//开始广播
void ble_data_update();
#endif