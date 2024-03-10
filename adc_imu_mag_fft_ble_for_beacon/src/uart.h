/**
 * @file         uart.h
 * @details      用于定义uart收发API
 * @author       jjl
 */
#ifndef _UART_H
#define _UART_H
/*
===========================
头文件包括
===========================
*/
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

/*
===========================
宏定义包括
===========================
*/
/* change this to any other UART peripheral if desired */
//检查设备节点
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)

#define MSG_SIZE 32
/*
===========================
函数申明包括
===========================
*/
/*
 * Read characters from UART until line end is detected. Afterwards push the
 * data to the message queue.
 * 读取串口数据并写入队列
 */ 
void serial_cb(const struct device *dev, void *user_data);
/*
 * Print a null-terminated string character by character to the UART interface
    串口以字符发送
 */
void print_uart(char *buf);
//串口初始化且注册中断处理函数
int uart_init_slef(void);
#endif
