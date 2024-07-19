#include "timer.h"
#include "adc.h"
#include "mymacros.h"
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
#include "ads1015.h"
#define  MY_PD_IRQ 5
#define  MY_IMU_IRQ 5
/**
 * @brief 定时器事件处理函数 2khz的采样同时兼顾0.5秒的选择
 *
 * @param[in] event_type 定时器的事件类型.
 * @param[in] p_context  传递的字节数
 */
// char str[10];
// char *P;
//10组imu的数据
SensorData imu_10[10] = {0};
//10组imu的数据做隔离
SensorData imu_10_2[10] = {0};
extern int flag,count,imu_flag,pd[1000],imu_count,pd2[1000];
//进行imu的数据控制
extern uint8_t icm42688_data[15],lis3dml_data[15];
extern int icm42688_time;
extern u_int16_t big_time;
//这里只是调试用途
char PA[20];
char PB[100];
int a=365;
void timer_handler(nrf_timer_event_t event_type, void * p_context)
{

    if(event_type == NRF_TIMER_EVENT_COMPARE0)
    {
        // count++;
        // pd[count-1] = adc_value_get();
        // //这里是用于打印
        // if(PD_UART_Display)
        // {
        //     sprintf(PA, "%d,",pd[count-1]); 
        //     print_uart(PA);
        // }
        // // 这里是0.05秒的计时器 与20hz的计时器
        // if(count%1000==0)
        // {
        //     count =0;
        //     flag =1;
        //     big_time++;
        //     //需要进行复制从而进行隔离
        //     memcpy(pd2, pd, sizeof(pd));
        // }
    Ads1015init(0);
	a=Ads1015read();
    //进行一次判断
    if(a>2047)
    {
        a=a-4096;
    }else{
        a=a;
    }
	char pa[10];
	sprintf(pa,"%d,",a);
	print_uart(pa);
        // flag=1;

    }
}
/**
 * @brief 第二个定时器事件处理函数 imu的采样
 *
 * @param[in] event_type 定时器的事件类型.
 * @param[in] p_context  传递的字节数
 */
void timer2_handler(nrf_timer_event_t event_type, void * p_context)
{

    if(event_type == NRF_TIMER_EVENT_COMPARE1)
    {
        //这里是0.5秒的计时器 与20hz的计时器
        imu_bag_read_data();
        //进行数据拷贝
        memcpy(imu_10[imu_count].icm, icm42688_data, sizeof(icm42688_data));
        memcpy(imu_10[imu_count].lis, lis3dml_data, sizeof(lis3dml_data));
        //用于测试是否数据传输过来
        // for(int j=0;j<1;j++)
        // {
        // sprintf(PB, "icm[%d]%d,imu_10.icm[%d]%d\n",j,icm42688_data[j+1],j,imu_10[imu_count].icm[j+1]); 
        // print_uart(PB);
        // }
        imu_10[imu_count].time = icm42688_time;
        //检测时间戳问题
        // sprintf(PB, "temp:%d,imu_10 count:%d\n",imu_10[imu_count].time,imu_count); 
        // print_uart(PB);
        imu_count++;
        if(imu_count==10)
        {
        imu_count=0;
        memcpy(imu_10_2, imu_10, sizeof(imu_10)); 
        //检查时间戳问题
        // sprintf(PB, "temp:%d,\n",imu_10_2[0].time); 
        // print_uart(PB);
        //检查时间戳问题
        // for(int j=0;j<10;j++)
        // {
        // sprintf(PB, "time1:%d,time2:%d\n",imu_10_2[j].time,imu_10[j].time); 
        // print_uart(PB);
        // }
        //自定义时间戳
        //进行置位imu_flag
        imu_flag = 1;        
        }
    }
}

/**
 * @brief 定时器相关配置及启动
 *
 * @return Nothing.
 */
void timer1_init_enable(void)
{
    nrfx_err_t status;
    (void)status;
    nrfx_timer_t timer_inst = NRFX_TIMER_INSTANCE(TIMER_INST_IDX);
    //用于获取定时器的基频值
    uint32_t base_frequency = NRF_TIMER_BASE_FREQUENCY_GET(timer_inst.p_reg);
    //配置定时器的基本属性
    nrfx_timer_config_t config = NRFX_TIMER_DEFAULT_CONFIG(base_frequency);
    //打印定时器的频率值
    printk("this is time %d\n",base_frequency);
    config.bit_width = NRF_TIMER_BIT_WIDTH_32;
    config.p_context = "Some context";
    //修改优先级为7
    config.interrupt_priority =6;
    printk("config.frequnecy is %d",config.frequency);
    printk("config.mode is %d",config.mode);
    //初始化定时器
    status = nrfx_timer_init(&timer_inst, &config, timer_handler);
    NRFX_ASSERT(status == NRFX_SUCCESS);

#if defined(__ZEPHYR__)
    IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER_INST_IDX)), MY_PD_IRQ,
                       NRFX_TIMER_INST_HANDLER_GET(TIMER_INST_IDX), 0);
    // IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER_INST_IDX)), IRQ_PRIO_LOWEST,
    //                    NRFX_TIMER_INST_HANDLER_GET(TIMER_INST_IDX), 0);                       
    //用于调试打印
    printk("irq:id%d",NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER_INST_IDX)));
#endif
    //清除定时器
    nrfx_timer_clear(&timer_inst);
    //直接通过转换来成计数器通道的配置数字，极大的简便了定时器的工作(这里可选择ms还是us)
    uint32_t desired_ticks = nrfx_timer_us_to_ticks(&timer_inst, TIME_TO_WAIT_US);
    // uint32_t desired_ticks = nrfx_timer_ms_to_ticks(&timer_inst, TIME_TO_WAIT_MS);
    printk("this is desired %d",desired_ticks);

    //用于使能定时器比较通道，使能比较中断，设置触发比较寄存器CC[n],根据通道来停止任务（或者清零）
    nrfx_timer_extended_compare(&timer_inst, NRF_TIMER_CC_CHANNEL0, desired_ticks,
                                NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    //使能定时器
    nrfx_timer_enable(&timer_inst);
    printk("timer is ok\n");
}

/**
 * @brief 第二个定时器相关配置及启动
 *
 * @return Nothing.
 */
void timer2_init_enable(void)
{
    nrfx_err_t status2;
    (void)status2;
    nrfx_timer_t timer_inst2 = NRFX_TIMER_INSTANCE(TIMER2_INST_IDX);
    //用于获取定时器的基频值
    uint32_t base_frequency2 = NRF_TIMER_BASE_FREQUENCY_GET(timer_inst2.p_reg);
    //配置定时器的基本属性
    nrfx_timer_config_t config2 = NRFX_TIMER_DEFAULT_CONFIG(base_frequency2);
    //打印定时器的频率值
    printk("this is time2 %d\n",base_frequency2);
    config2.bit_width = NRF_TIMER_BIT_WIDTH_32;
    config2.p_context = "Some context";
    printk("config.frequnecy is %d",config2.frequency);
    printk("config.mode is %d",config2.mode);
    //初始化定时器
    status2 = nrfx_timer_init(&timer_inst2, &config2, timer2_handler);
    printk("aaa:status %d",status2);
    NRFX_ASSERT(status2 == NRFX_SUCCESS);

#if defined(__ZEPHYR__)
    // IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER2_INST_IDX)), MY_IMU_IRQ,
    //                    NRFX_TIMER_INST_HANDLER_GET(TIMER2_INST_IDX), 0);
    IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER2_INST_IDX)), IRQ_PRIO_LOWEST,
                       NRFX_TIMER_INST_HANDLER_GET(TIMER2_INST_IDX), 0);                       
    printk("IRQ_PRIO_LOWEST:%d\n",IRQ_PRIO_LOWEST);
	printk("timer2:%d\n",	NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER2_INST_IDX)));
	printk("BIT:%d\n",	 NRFX_TIMER_INST_HANDLER_GET(TIMER2_INST_IDX));
#endif
    config2.interrupt_priority =6;
    printk("config2.frequnecy is %d",config2.interrupt_priority);
    //清除定时器
    nrfx_timer_clear(&timer_inst2);
    //直接通过转换来成计数器通道的配置数字，极大的简便了定时器的工作(这里可选择ms还是us)
    uint32_t desired_ticks2 = nrfx_timer_ms_to_ticks(&timer_inst2, TIME2_TO_WAIT_MS);
    printk("this is desired %d",desired_ticks2);
    //用于使能定时器比较通道，使能比较中断，设置触发比较寄存器CC[n],根据通道来停止任务（或者清零）
    // nrfx_timer_extended_compare(&timer_inst2, NRF_TIMER_CC_CHANNEL0, desired_ticks2,
    //                             NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    nrfx_timer_extended_compare(&timer_inst2, NRF_TIMER_CC_CHANNEL1, desired_ticks2,
                                    NRF_TIMER_SHORT_COMPARE1_CLEAR_MASK, true);

    //使能定时器
    nrfx_timer_enable(&timer_inst2);
    printk("timer2 is ok\n");
}

