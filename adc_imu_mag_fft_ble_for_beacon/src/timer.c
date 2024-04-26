#include "timer.h"
#include "adc.h"
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
/**
 * @brief 定时器事件处理函数 2khz的采样同时兼顾0.5秒的选择
 *
 * @param[in] event_type 定时器的事件类型.
 * @param[in] p_context  传递的字节数
 */
// char str[10];
// char *P;
extern int flag,count,imu_flag,pd[1000];
void timer_handler(nrf_timer_event_t event_type, void * p_context)
{

    if(event_type == NRF_TIMER_EVENT_COMPARE0)
    {
        count++;
        pd[count-1] = adc_value_get();
        //这里是0.05秒的计时器 与20hz的计时器
        if(count%100==0)
        {
            count =0;
            flag =1;
        }
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

    if(event_type == NRF_TIMER_EVENT_COMPARE0)
    {
        //这里是0.5秒的计时器 与20hz的计时器
        imu_bag_read_data();
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
    //修改优先级为6
    config.interrupt_priority =6;
    printk("config.frequnecy is %d",config.frequency);
    printk("config.mode is %d",config.mode);
    //初始化定时器
    status = nrfx_timer_init(&timer_inst, &config, timer_handler);
    NRFX_ASSERT(status == NRFX_SUCCESS);

#if defined(__ZEPHYR__)
    IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER_INST_IDX)), IRQ_PRIO_LOWEST,
                       NRFX_TIMER_INST_HANDLER_GET(TIMER_INST_IDX), 0);
    printk("A\n");
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
    IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER2_INST_IDX)), IRQ_PRIO_LOWEST,
                       NRFX_TIMER_INST_HANDLER_GET(TIMER2_INST_IDX), 0);
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
    nrfx_timer_extended_compare(&timer_inst2, NRF_TIMER_CC_CHANNEL0, desired_ticks2,
                                    NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    //使能定时器
    nrfx_timer_enable(&timer_inst2);
    printk("timer2 is ok\n");
}

