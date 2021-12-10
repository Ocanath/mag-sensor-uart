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

extern int32_t cos_mid;
extern int32_t sin_mid;

int32_t theta_abs_fixed();

#endif /* MAG_ENCODER_H_ */
