/*
 * mag-encoder.c
 *
 *  Created on: Jun 20, 2018
 *      Author: Ocanath
 */
#include "mag-encoder.h"
#include "trig_fixed.h"

int32_t sin_mid = 1985;
int32_t cos_mid = 1985;


int32_t theta_abs_fixed()
{
	int32_t sinVal = ((int32_t)dma_adc_raw[ADC_SIN_CHAN])-sin_mid;
	int32_t cosVal = ((int32_t)dma_adc_raw[ADC_COS_CHAN])-cos_mid;
	return atan2_fixed(sinVal,cosVal);
}
