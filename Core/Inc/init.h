/*
 * init.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Ocanath
 */

#ifndef INC_INIT_H_
#define INC_INIT_H_
#include "main.h"


extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;

extern UART_HandleTypeDef huart1;

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC_Init(void);
void MX_USART1_UART_Init(void);


#endif /* INC_INIT_H_ */
