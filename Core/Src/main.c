#include "mag-encoder.h"

typedef union
{
	float v;
	uint8_t d[sizeof(float)+1];
}floatsend_t;

/*
Generic hex checksum calculation.
TODO: use this in the psyonic API
*/
uint8_t get_checksum(uint8_t * arr, int size)
{

	int8_t checksum = 0;
	for (int i = 0; i < size; i++)
		checksum += (int8_t)arr[i];
	return -checksum;
}

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
	float prev_theta = -HALF_PI;

	uint32_t uart_disp_ts = 0;
	const uint32_t uart_update_period = 1;	//in ms

	while (1)
	{
		if(gl_adc_cplt_flag)
		{
			theta.v = unwrap(theta_abs_rad(),&prev_theta);
			theta.d[4] = get_checksum(theta.d, 4);
			gl_adc_cplt_flag = 0;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
		}

		if(HAL_GetTick() >= uart_disp_ts) //update frequency may be prone to jitters due to high calculation time of unwrap and atan2
		{
			HAL_ADC_Start_DMA(&hadc, (uint32_t * )dma_adc_raw, NUM_ADC);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
			HAL_UART_Transmit(&huart1, theta.d, 5, uart_update_period);
			uart_disp_ts = HAL_GetTick()+uart_update_period;	//1 = 1khz, 2 = 500Hz, 3 = 333Hz

		}
	}
}
