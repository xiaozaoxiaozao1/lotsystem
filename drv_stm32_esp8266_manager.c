#include "drv_stm32_esp8266_manager.h"

extern void printArray(char *data, uint32_t len);

extern UART_HandleTypeDef huart1;
extern void USART1_IdleCallback(uint8_t *pData, uint16_t len);
void enable_wifi_receive_interrupt(void);

uint8_t uart_rx_temp;
uint8_t uart_rx_buf[1024];
uint16_t uart_rx_buf_index = 0;



extern esp8266_t global_esp8266;
//对数据进行组包操作,组包格式为TLV。 如果不需要组包也可以在这里直接进行业务的处理。
void USART1_IdleCallback(uint8_t *pData, uint16_t len)
{	
	tools_esp8266_handle_response(&global_esp8266, pData);
	//printf("\r %s\r", pData);
}


//使能串口接受中断
void enable_wifi_receive_interrupt(void)
{
	  HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
	
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
		__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

//ESP8266串口中断接受函数
void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);

	if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET)
	{
		HAL_UART_Receive(&huart1, &uart_rx_temp, 1, 100);
		if(uart_rx_buf_index < 1024)
		{
			uart_rx_buf[uart_rx_buf_index] = uart_rx_temp;
			uart_rx_buf_index++;
		}
		__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
	}
	
	//receive a fram complete
	if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET)
	{ 
			USART1_IdleCallback(uart_rx_buf, uart_rx_buf_index);
			memset(uart_rx_buf, 0, uart_rx_buf_index);
			uart_rx_buf_index = 0;
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		
	}
}
