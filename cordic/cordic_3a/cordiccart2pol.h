#ifndef CORDICCART2POL_H
#define CORDICCART2POL_H

#include "ap_fixed.h"

#define NO_ITER 16

#define AP_FIXED_BIT_WIDTH 16

typedef int coef_t;
typedef ap_fixed<AP_FIXED_BIT_WIDTH, 2> data_t; //float data_t;
typedef float acc_t;
typedef ap_fixed<AP_FIXED_BIT_WIDTH, 3> angle_t;

// void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta);
void cordiccart2pol(data_t x, data_t y, data_t * r,  angle_t * theta);

#endif
