#include<math.h>
#include "dft.h"
#include"coefficients32_2D.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE real_output[SIZE], DTYPE imag_output[SIZE])
{
//Write your code here
	const int FACTOR = 2;
	#pragma HLS ARRAY_PARTITION variable=real_sample block factor=FACTOR
	#pragma HLS ARRAY_PARTITION variable=imag_sample block factor=FACTOR
	#pragma HLS ARRAY_PARTITION variable=real_output block factor=FACTOR
	#pragma HLS ARRAY_PARTITION variable=imag_output block factor=FACTOR
	#pragma HLS ARRAY_PARTITION variable=cos_coeff_table block factor=FACTOR
	#pragma HLS ARRAY_PARTITION variable=sin_coeff_table block factor=FACTOR

	int i, j;
	DTYPE w;
	DTYPE c, s;

	// Calculate each frequency domain sample iteratively
	for (i = 0; i < SIZE; i += 1) {
		#pragma HLS PIPELINE II=1

		// Calculate the jth frequency sample sequentially
		for (j = 0; j < SIZE; j += 1) {
			#pragma HLS PIPELINE II=1

			c = cos_coeff_table[j][i];
			s = sin_coeff_table[j][i];
			// Multiply the current phasor with the appropriate input sample and keep
			// running sum
			real_output[i] += (real_sample[j] * c - imag_sample[j] * s);
			imag_output[i] += (real_sample[j] * s + imag_sample[j] * c);
		}
	}
}
