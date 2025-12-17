#include<math.h>
#include "dft.h"
#include"coefficients1024.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE],DTYPE real_op[SIZE],DTYPE imag_op[SIZE])
{
	//Write your code here
	int i, j;
	double w;
	DTYPE c, s;
	// Calculate each frequency domain sample iteratively
	for (i = 0; i < SIZE; i += 1) {
		// Calculate the jth frequency sample sequentially
		for (j = 0; j < SIZE; j += 1) {
			// Utilize HLS tool to calculate sine and cosine values
			c = cos_coefficients_table[(i * j) % SIZE];
			s = sin_coefficients_table[(i * j) % SIZE];
			// Multiply the current phasor with the appropriate input sample and keep
			// running sum
			real_op[i] += (real_sample[j] * c - imag_sample[j] * s);
			imag_op[i] += (real_sample[j] * s + imag_sample[j] * c);
		}
	}
}
