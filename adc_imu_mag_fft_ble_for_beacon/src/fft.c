#include "fft.h"
//定义相关的频率的点位(在点位范围内上下加减2) 频率为213,437,589,777,943,985
int fre_num[6]={218-2,447-2,603-2,795-2,965-2,1008-2};
arm_cfft_instance_f32 scfft;
extern int pd[1000];
/* FFT信号数组，包含实部和虚部 */
float fft_input_buf[FFT_LENGTH * 2];
float fft_input_buf2[FFT_LENGTH * 2];
/* FFT输出结果数组，幅度输出 */
float fft_output_mag_buf[FFT_LENGTH];
/*FFT最大的六个值以及输出*/
int fft_out[6];
int fft_index[6];
//用于调试
// char str_a[100];
// //字符指针
// char *P_a;
// int  offset_a=0;
int fft(void)
{
	uint32_t maxIndex;
    float32_t maxValue;
    /* ===================fft部分=================== */
	/* 初始化scfft结构体，设定FFT相关参数*/
	arm_cfft_init_f32(&scfft, FFT_LENGTH);
	int i =0;
	/* 生成信号序列 */
	for ( i = 0; i < FFT_LENGTH; i++)
	{
        //实部进行PD数据的赋值
        if(i<1000)
        {
           fft_input_buf[2 * i] = pd[i];
        }else{
           fft_input_buf[2 * i] = 0;
        }
		/* 虚部 */
		fft_input_buf[2 * i + 1] = 0;
	}
	//输出i
	// printk("i:%d\n",i);
	/* FFT计算 */
	arm_cfft_f32(&scfft, fft_input_buf, 0, 1);
	 /* 运算结果求幅值 */
	arm_cmplx_mag_f32(fft_input_buf, fft_output_mag_buf, FFT_LENGTH);
	// printk("i:%d\n",i+1);
	// //以下是输出的FFT的值
    // for (int i = 0; i < FFT_LENGTH; i++)
	// {
	// 	printk("%d.%d, ", (uint16_t)(fft_output_mag_buf[i]), (uint16_t)((fft_output_mag_buf[i] - (uint16_t)(fft_output_mag_buf[i])) * 100.0));
	// 	if ((i + 1) % 32 == 0)
	// 	{
	// 		printk("\n");
	// 	}
	// }
	// printk("fft end\n");

	//for 循环 在对应频率 取得最大的六个数值
	for(i=0;i<6;i++)
	{
		//找寻最大值
    	arm_max_f32(&fft_output_mag_buf[fre_num[i]], 5, &maxValue, &maxIndex);
		//将最大值存入数组 记录数值
		fft_out[i]=(int)(maxValue*2/FFT_LENGTH);
		fft_index[i]=(int)maxIndex+fre_num[i];
	}
    // arm_max_f32(fft_output_mag_buf, FFT_LENGTH, &maxValue, &maxIndex);
    // printk("最大值为: %d\n", fft_out[0]);
    // printk("对应的索引序列为: %d\n",fft_index[0]);
	//串口检验FFT的大小
	// for(i=0;i<6;i++)
	// {
	// 	offset_a += sprintf(str_a + offset_a, "index %d:%d,", fft_index[i],fft_out[i]); 
	// }
	// P_a=str_a;
	// print_uart(P_a);
	// offset_a=0;
	return 0;
}