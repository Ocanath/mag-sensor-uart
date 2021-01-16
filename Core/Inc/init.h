/*
 * init.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Ocanath
 */

#ifndef INC_INIT_H_
#define INC_INIT_H_
#include "main.h"

#define I2C_SLAVE_ADDRESS (0x55 << 1)

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

I2C_HandleTypeDef hi2c1;

void SystemClock_Config(void);
 void MX_GPIO_Init(void);
 void MX_DMA_Init(void);
 void MX_ADC_Init(void);
 void MX_I2C1_Init(void);

#endif /* INC_INIT_H_ */
