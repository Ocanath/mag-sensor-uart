#include "mag-encoder.h"
#include "m_uart.h"
#include "trig_fixed.h"

#define NETWORK_ID	1
#define NUM_TX_BYTES 4

typedef union
{
	int16_t v;
	uint8_t d[sizeof(int16_t)];
}int16_chk_t;

static volatile int16_chk_t theta;
static uint8_t uart_tx_buf[NUM_TX_BYTES] = {NETWORK_ID, 0, 0, 0};
volatile uint8_t gl_adc_cplt_flag = 0;
static volatile uint8_t uart_activity_flag = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	gl_adc_cplt_flag = 1;
}

void m_uart_rx_cplt_callback(uart_it_t * h)
{
	int8_t sum = 0;
	for(int i = 0; i < h->bytes_received; i++)
		sum += h->rx_buf[i];
	if(sum == 0)
	{
		uint8_t id = h->rx_buf[0];
		if(id == NETWORK_ID)
		{
			uart_tx_buf[1] = theta.d[0];
			uart_tx_buf[2] = theta.d[1];
			uart_tx_buf[3] = get_checksum(uart_tx_buf, 3);
			m_uart_tx_start(&m_huart1, uart_tx_buf, 4);
			uart_activity_flag = 1;
		}
	}
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
	for(int i = 0; i < 3; i++)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
		HAL_Delay(50);
	}

	while (1)
	{
		if(gl_adc_cplt_flag)
		{
			theta.v = (int16_t)(theta_abs_fixed());
			gl_adc_cplt_flag = 0;
		}
		if(uart_activity_flag)
		{
			HAL_ADC_Start_DMA(&hadc, (uint32_t * )dma_adc_raw, NUM_ADC);
			uart_activity_flag = 0;
		}
	}
}
