/*
 * mag-encoder.h
 *
 *  Created on: Jun 20, 2018
 *      Author: Ocanath
 */

#ifndef MAG_ENCODER_H_
#define MAG_ENCODER_H_
#include "adc.h"

#define ADC_SIN_CHAN 	0
#define ADC_COS_CHAN 	1

#define ONE_BY_THREE_FACTORIAL 	0.16666666666
#define ONE_BY_FIVE_FACTORIAL 	0.00833333333
#define ONE_BY_SEVEN_FACTORIAL	0.00019841269
#define HALF_PI 				1.57079632679
#define PI						3.14159265359
#define THREE_BY_TWO_PI     	4.71238898038
#define TWO_PI              	6.28318530718
#define ONE_BY_TWO_PI 			0.1591549

const float rad_to_deg;

extern float theta_m_prev;

extern int16_t cos_mid;
extern int16_t sin_mid;

float theta_abs_rad();
float theta_abs_deg();

/*
 * fast trig functions
 */
float atan2_approx(float sinVal, float cosVal);
float sin_fast(float theta);
float cos_fast(float theta);
float fmod_2pi(float in);
float unwrap(float theta, float * prev_theta);


#endif /* MAG_ENCODER_H_ */
