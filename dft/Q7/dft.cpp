#include<math.h>
#include "dft.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE],DTYPE real_op[SIZE],DTYPE imag_op[SIZE])
{
	//Write your code here
	int i, j;
	double w;
	DTYPE c, s;

    // Interchanged loops to improve parallelism
    for (j = 0; j < SIZE; j++) {
        for (i = 0; i < SIZE; i++) {
            w = (2.0 * 3.141592653589 / SIZE) * (DTYPE)i;

            c = cos(j * w);
            s = -sin(j * w);

            // Accumulate contributions for frequency bin i
            real_op[i] += real_sample[j] * c - imag_sample[j] * s;
            imag_op[i] += real_sample[j] * s + imag_sample[j] * c;
        }
    }
}
