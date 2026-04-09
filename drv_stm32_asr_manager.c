#include "drv_stm32_asr_manager.h"
extern UART_HandleTypeDef huart2;

void USART2_IdleCallback(uint8_t *pData, uint16_t len);

uint8_t uart2_rx_temp;
uint8_t uart2_rx_buf[1024];
uint16_t uart2_rx_buf_index = 0;

extern app_status global_state;

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);

	if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET)
	{
		HAL_UART_Receive(&huart2, &uart2_rx_temp, 1, 100);
		if(uart2_rx_buf_index < 1024)
		{
			uart2_rx_buf[uart2_rx_buf_index] = uart2_rx_temp;
			uart2_rx_buf_index++;
		}
		__HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_RXNE);
	}
	
	//receive a fram complete
	if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)
	{ 
			USART2_IdleCallback(uart2_rx_buf, uart2_rx_buf_index);
			memset(uart2_rx_buf, 0, uart2_rx_buf_index);
			uart2_rx_buf_index = 0;
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		
	}
}

void USART2_IdleCallback(uint8_t *pData, uint16_t len)
{	
	uint8_t open_state = 0;
	uint8_t device_id = 50;
	if(pData[0] == 0x55)
	{
		device_id = pData[1];
		open_state = pData[2];
		
		printf("%02x %02x %02x \r\n", pData[0], device_id, open_state);
		
		if(device_id == 1)
		{
			if(open_state == 1)
			{
				global_state.fan_state = fan_open;
			}
			else
			{
				global_state.fan_state = fan_close;
			}
		}

		if(device_id == 2)
		{
			if(open_state == 1)
			{
				global_state.light_state = light_open;
			}
			else
			{
				global_state.light_state = light_close;
			}
		}
		
		if(device_id == 3)
		{
			if(open_state == 1)
			{
				global_state.humi_state = humi_open;
			}
			else
			{
				global_state.humi_state = humi_close;
			}
		}
		
		if(device_id == 0xFF)
		{
			global_state.oled_state = oled_show_mode;
			if(open_state == 1)
			{
				global_state.mode_state = mu_mode;
			}
			else
			{
				global_state.mode_state = auto_mode;
			}
		}
	}
}