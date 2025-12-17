#include <math.h>
#include "dft.h"
#include "coefficients1024.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_op[SIZE], DTYPE imag_op[SIZE])
{
    #pragma HLS ARRAY_PARTITION variable=real_sample cyclic factor=16 dim=1
    #pragma HLS ARRAY_PARTITION variable=imag_sample cyclic factor=16 dim=1
    #pragma HLS ARRAY_PARTITION variable=cos_coefficients_table block factor=16 dim=1
    #pragma HLS ARRAY_PARTITION variable=sin_coefficients_table block factor=16 dim=1

    int i, j;

    for (i = 0; i < SIZE; i++) {
        DTYPE real_sum = 0;
        DTYPE imag_sum = 0;

        for (j = 0; j < SIZE; j++) {
            #pragma HLS PIPELINE II=1
            DTYPE c = cos_coefficients_table[(i * j) % SIZE];
            DTYPE s = sin_coefficients_table[(i * j) % SIZE];
            real_sum += real_sample[j] * c - imag_sample[j] * s;
            imag_sum += real_sample[j] * s + imag_sample[j] * c;
        }

        real_op[i] = real_sum;
        imag_op[i] = imag_sum;
    }
}
