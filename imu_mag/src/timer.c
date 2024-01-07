#include "timer.h"

/**
 * @brief 定时器事件处理函数
 *
 * @param[in] event_type 定时器的事件类型.
 * @param[in] p_context  传递的字节数
 */
void timer_handler(nrf_timer_event_t event_type, void * p_context)
{
    if(event_type == NRF_TIMER_EVENT_COMPARE0)
    {
        printk("hello timer2");
        // adc_read_data();
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
    config.bit_width = NRF_TIMER_BIT_WIDTH_32;
    config.p_context = "Some context";
    //初始化定时器
    status = nrfx_timer_init(&timer_inst, &config, timer_handler);
    NRFX_ASSERT(status == NRFX_SUCCESS);

#if defined(__ZEPHYR__)
    IRQ_DIRECT_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER_INST_IDX)), IRQ_PRIO_LOWEST,
                       NRFX_TIMER_INST_HANDLER_GET(TIMER_INST_IDX), 0);
#endif
    //清除定时器
    nrfx_timer_clear(&timer_inst);
    //直接通过转换来成计数器通道的配置数字，极大的简便了定时器的工作(这里可选择ms还是us)
    uint32_t desired_ticks = nrfx_timer_us_to_ticks(&timer_inst, TIME_TO_WAIT_US);
    // uint32_t desired_ticks = nrfx_timer_ms_to_ticks(&timer_inst, TIME_TO_WAIT_MS);


    //用于使能定时器比较通道，使能比较中断，设置触发比较寄存器CC[n],根据通道来停止任务（或者清零）
    nrfx_timer_extended_compare(&timer_inst, NRF_TIMER_CC_CHANNEL0, desired_ticks,
                                NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    //使能定时器
    nrfx_timer_enable(&timer_inst);
    printk("timer is ok\n");
}

