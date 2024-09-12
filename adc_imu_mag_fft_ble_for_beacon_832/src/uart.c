/*
用于串口发送
 */

#include "uart.h"





/* queue to store up to 10 messages (aligned to 4-byte boundary) */
//初始化存储队列
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 100, 4);

const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

/* receive buffer used in UART ISR callback */
char rx_buf[MSG_SIZE];
int rx_buf_pos;

/*
 * Read characters from UART until line end is detected. Afterwards push the
 * data to the message queue.
 */
void serial_cb(const struct device *dev, void *user_data)
{
	uint8_t c;

	if (!uart_irq_update(uart_dev)) {
		return;
	}

	if (!uart_irq_rx_ready(uart_dev)) {
		return;
	}

	/* read until FIFO empty */
	while (uart_fifo_read(uart_dev, &c, 1) == 1) {
		if ((c == '\n' || c == '\r'|| c == ',') && rx_buf_pos > 0) {
			/* terminate string */
			rx_buf[rx_buf_pos] = ',';
			rx_buf[rx_buf_pos+1] = '\0';

			/* if queue is full, message is silently dropped */
			k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);
			/* reset the buffer (it was copied to the msgq) */
			rx_buf_pos = 0;
		} else if (rx_buf_pos < (sizeof(rx_buf) - 1)) {
			rx_buf[rx_buf_pos++] = c;
		}
		/* else: characters beyond buffer size are dropped */
	}
}

/*
 * Print a null-terminated string character by character to the UART interface
 */
void print_uart(char *buf)
{
	int msg_len = strlen(buf);

	for (int i = 0; i < msg_len; i++) {
		uart_poll_out(uart_dev, buf[i]);
	}
}
//串口初始化且注册中断处理函数
int uart_init_slef(void)
{
	if (!device_is_ready(uart_dev)) {
		printk("UART device not found!");
		return 0;
	}else{
		printk("hello uart\n");
	}
    	/* configure interrupt and callback to receive data */
	//进行中断处理函数的回调
	int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

	if (ret < 0) {
		if (ret == -ENOTSUP) {
			printk("Interrupt-driven UART API support not enabled\n");
		} else if (ret == -ENOSYS) {
			printk("UART device does not support interrupt-driven API\n");
		} else {
			printk("Error setting UART callback: %d\n", ret);
		}
		return 0;
	}else{
		printk("hello ret\n");
	}
	uart_irq_rx_enable(uart_dev);
	printk("this is ok\n");
	printk("this is anther\n");	
    return 0;
}
// int main(void)
// {
// 	//用于计数
// 	int  count =0;
// 	char str[10];//用于存储格式化字符串
// 	char tx_buf[MSG_SIZE]; //字符指针
// 	char *ptr;
// 	while(1)
// 	{
// 	//将整数格式化成字符串
// 	sprintf(str,"%d\n",count);
// 	//将字符串的首地址赋给字符指针
// 	ptr=str;
// 	print_uart(ptr);
// 	k_sleep(K_MSEC(1));
// 	count++;
// 	// print_uart("this is my first try\n");
// 	// k_sleep(K_MSEC(1000));
// 	}
// 	/* indefinitely wait for input from the user */
// 	while (k_msgq_get(&uart_msgq, &tx_buf, K_FOREVER) == 0) {
// 		print_uart("Echo: ");
// 		print_uart(tx_buf);
// 		print_uart("\r\n");
// 	}
// 	return 0;
// }
