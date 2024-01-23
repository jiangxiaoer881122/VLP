/*
 * Copyright (c) 2020 Libre Solar Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "adc.h"



/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
                         DT_SPEC_AND_COMMA)};
static int err;
// static uint32_t count = 0;
static uint16_t buf;
static struct adc_sequence sequence = {
    .buffer = &buf,
    /* buffer size in bytes, not number of samples */
    .buffer_size = sizeof(buf),
};
/**
 * @brief        adc初始化配置
 */
int adc_init(void)
{

    /* Configure channels individually prior to sampling. */
    for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
    {
        if (!adc_is_ready_dt(&adc_channels[i]))
        {
            printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
            return 0;
        }

        err = adc_channel_setup_dt(&adc_channels[i]);
        if (err < 0)
        {
            printk("Could not setup channel #%d (%d)\n", i, err);
            return 0;
        }
        (void)adc_sequence_init_dt(&adc_channels[i], &sequence);
    }
    return 1;
}
/**
 * @brief        adc读取数据处理
 */
void adc_read_data(void)
{
	//用于存储格式化字符串
	char str[10];
	//字符指针
	char *ptr;

        // printk("ADC reading[%u]:\n", count++);
        for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++)
        {
            int32_t val_mv;

            // printk("- %s, channel %d: ",
            //        adc_channels[i].dev->name,
            //        adc_channels[i].channel_id);
            // (void)adc_sequence_init_dt(&adc_channels[i], &sequence);
            err = adc_read(adc_channels[i].dev, &sequence);
            if (err < 0)
            {
                printk("Could not read (%d)\n", err);
                continue;
            }
            /*
             * If using differential mode, the 16 bit value
             * in the ADC sample buffer should be a signed 2's
             * complement value.
             */
            if (adc_channels[i].channel_cfg.differential)
            {
                val_mv = (int32_t)((int16_t)buf);
            }
            else
            {
                val_mv = (int32_t)buf;
            }
            // printk("ADC%\n" PRId32, val_mv);
            // printk("%d,", val_mv);

            //将整数格式化成指针
		    sprintf(str,"%d,",val_mv);
		    ptr =str;
    		print_uart(ptr);


            //下列代码是将ADC原始值变换为mV
            // err = adc_raw_to_millivolts_dt(&adc_channels[i],
            //                                &val_mv);
            // /* conversion to mV may not be supported, skip if not */
            // if (err < 0)
            // {
            //     printk(" (value in mV not available)\n");
            // }
            // else
            // {
            //     printk(" = %" PRId32 " mV\n", val_mv);
            // }
        }
}
