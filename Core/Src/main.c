#include "mag-encoder.h"
#include "m_uart.h"
#include "trig_fixed.h"

#define NETWORK_ID	0
#define NUM_TX_BYTES 4	//addr and checksum
#define NUM_RX_BYTES 4	//addr, 2 datas, checksum
#define NUM_SLAVES 1

typedef union
{
	int16_t v;
	uint8_t d[sizeof(int16_t)];
}int16_fmt_t;

static volatile int16_fmt_t theta;
volatile uint8_t gl_adc_cplt_flag = 0;
static volatile uint8_t uart_activity_flag = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	gl_adc_cplt_flag = 1;
}

typedef struct slave_node_t
{
	int16_fmt_t theta;
	uint8_t id;
}slave_node_t;

slave_node_t nodes[NUM_SLAVES] = {
		{
				.theta = {0},
				.id = 1
		}
};

void m_uart_rx_cplt_callback(uart_it_t * h)
{
	int8_t sum = 0;
	for(int i = 0; i < h->bytes_received; i++)
		sum += h->rx_buf[i];
	if(sum == 0)
	{
		uint8_t id = h->rx_buf[0];
		//kludge. can do some state management with the ID sent out by the master and use O(n) as a fallback. For now, just see if it works
		for(int i = 0; i < NUM_SLAVES; i++)
		{
			if(id == nodes[i].id)
			{
				nodes[i].theta.d[0] = h->rx_buf[1];
				nodes[i].theta.d[1] = h->rx_buf[2];
				break;
			}
		}
		uart_activity_flag = 1;
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

	uint32_t tx_ts = 0;
	while (1)
	{
		uint32_t tick = HAL_GetTick();
		if(tick > tx_ts)
		{
			tx_ts = tick+10;
			for(int i = 0; i < NUM_SLAVES; i++)
			{
				uint8_t buf[NUM_TX_BYTES];
				buf[0] = nodes[i].id;
				buf[1] = nodes[i].theta.d[0];
				buf[2] = nodes[i].theta.d[1];
				buf[3] = get_checksum(buf,3);
				m_uart_tx_start(&m_huart1, buf, NUM_TX_BYTES);
			}
		}

//		if(gl_adc_cplt_flag)
//		{
//			theta.v = (int16_t)(theta_abs_fixed());
//			gl_adc_cplt_flag = 0;
//		}
//		if(uart_activity_flag)
//		{
//			HAL_ADC_Start_DMA(&hadc, (uint32_t * )dma_adc_raw, NUM_ADC);
//			uart_activity_flag = 0;
//		}
	}
}
