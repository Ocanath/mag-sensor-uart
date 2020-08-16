#include "mag-encoder.h"

typedef union
{
	float v;
	uint8_t d[4];
}floatsend_t;

volatile uint8_t gl_adc_cplt_flag = 0;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	gl_adc_cplt_flag = 1;
}


int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC_Init();
	MX_USART1_UART_Init();

	HAL_ADC_Start_DMA(&hadc, (uint32_t * )dma_adc_raw, NUM_ADC);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15,0);

	floatsend_t theta;
	float prev_theta = 0.f;

	uint32_t uart_disp_ts = 0;
	const uint32_t uart_update_period = 2;	//in ms

	while (1)
	{
		if(gl_adc_cplt_flag)
		{
			theta.v = unwrap(theta_abs_rad(),&prev_theta);
			gl_adc_cplt_flag = 0;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
		}

		if(HAL_GetTick() >= uart_disp_ts) //update frequency may be prone to jitters due to high calculation time of unwrap and atan2
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
			HAL_UART_Transmit(&huart1, theta.d, 4, uart_update_period);
			uart_disp_ts = HAL_GetTick()+uart_update_period;	//1 = 1khz, 2 = 500Hz, 3 = 333Hz

		}
	}
}

