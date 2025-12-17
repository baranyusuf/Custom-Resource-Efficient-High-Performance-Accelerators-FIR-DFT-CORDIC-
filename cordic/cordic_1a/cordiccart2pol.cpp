#include "cordiccart2pol.h"

// data_t Kvalues[NO_ITER] = {1,	0.500000000000000,	0.250000000000000,	0.125000000000000,	0.0625000000000000,	0.0312500000000000,	0.0156250000000000,	0.00781250000000000,	0.00390625000000000,	0.00195312500000000,	0.000976562500000000,	0.000488281250000000,	0.000244140625000000,	0.000122070312500000,	6.10351562500000e-05,	3.05175781250000e-05  /*stuff*/};

// data_t angles[NO_ITER] = {0.785398163397448,	0.463647609000806,	0.244978663126864,	0.124354994546761,	0.0624188099959574,	0.0312398334302683,	0.0156237286204768,	0.00781234106010111,	0.00390623013196697,	0.00195312251647882,	0.000976562189559320,	0.000488281211194898,	0.000244140620149362,	0.000122070311893670,	6.10351561742088e-05,	3.05175781155261e-05};

data_t Kvalues[] = {
    1.0, 0.5, 0.25, 0.125, 0.0625,
    0.03125, 0.015625, 0.0078125, 0.00390625, 0.001953125,
    0.0009765625, 0.00048828125, 0.000244140625, 0.0001220703125,
    6.103515625e-05, 3.0517578125e-05, 1.52587890625e-05,
    7.62939453125e-06, 3.814697265625e-06, 1.9073486328125e-06
};

data_t angles[] = {
    0.785398163397448, 0.463647609000806, 0.244978663126864, 0.124354994546761,
    0.0624188099959574, 0.0312398334302683, 0.0156237286204768, 0.00781234106010111,
    0.00390623013196697, 0.00195312251647882, 0.000976562189559320, 0.000488281211194898,
    0.000244140620149362, 0.000122070311893670, 6.10351561742088e-05, 3.05175781155261e-05,
    1.5258789061315762e-05, 7.62939453110197e-06, 3.814697265606496e-06, 1.907348632810187e-06
};

void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta)
{
	// Write your code here

	if (x == 0 && y == 0) {
        *r = 0;
        *theta = 0;
        return;
    }

    data_t current_x = x;
    data_t current_y = y;
    data_t current_theta = 0;
	// int NUM_ITERATION = 16;

    data_t base_angle = 0;
    if (current_x < 0) {
        if (current_y >= 0)
            base_angle = 3.141592;
        else
            base_angle = -3.141592;
        current_x = -current_x;
        current_y = -current_y;
    }

    for (int j = 0; j < NO_ITER; j++) {
        data_t x_shift = current_x * Kvalues[j];
        data_t y_shift = current_y * Kvalues[j];
    
        if (current_y > 0) {
			// Rotate clockwise to reduce y toward 0
			current_x = current_x + y_shift;
			current_y = current_y - x_shift;
			current_theta = current_theta + angles[j];
        }
        else {
            // Rotate counterclockwise
            current_x = current_x - y_shift;
            current_y = current_y + x_shift;
            current_theta = current_theta - angles[j];
        }
    }

    *r = current_x * 0.607252935;
    *theta = current_theta + base_angle;
}
