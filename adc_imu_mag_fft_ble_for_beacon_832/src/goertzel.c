#include <math.h>
#include "goertzel.h"



float goertzel(int *data, int N, float fs, float f_target) {
    int k = (int)(0.5f + ((N * f_target) / fs));
    float omega = (2.0f * PI * k) / N;
    float coeff = 2.0f * cosf(omega);

    float s_prev = 0.0f;
    float s_prev2 = 0.0f;
    float s;
	// printk("da:%d\n",(int)data[0]);
	// printk("da:%d\n",(int)data[1]);
	// printk("da:%d\n",(int)data[2]);
	// printk("da:%d\n",(int)data[3]);
	// printk("da:%d\n",(int)data[4]);
	// printk("da:%d\n",(int)data[5]);
    for (int n = 0; n < N; n++) {
        s = data[n] + coeff * s_prev - s_prev2;
        s_prev2 = s_prev;
        s_prev = s;
    }

    return s_prev2 * s_prev2 + s_prev * s_prev - coeff * s_prev * s_prev2;
}
