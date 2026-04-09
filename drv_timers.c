#include "drv_timers.h"

extern ADC_HandleTypeDef hadc1;

//basic count
uint32_t timer2_led_count = 0;
uint32_t timer2_oled_count = 0;
uint32_t timer2_esp8266_count = 0;
uint32_t timer2_work_count = 0;
uint32_t timer2_warning_count = 0;

//sensor count
uint32_t timer2_gy302_count = 0;
uint32_t timer2_dht11_count = 0;
uint32_t timer2_mq7_count = 0;

//basic flag
uint8_t timer2_led_enable_flag = 0;
uint8_t timer2_oled_enable_flag = 0;
uint8_t timer2_esp8266_enable_flag = 0;
uint8_t timer2_work_enable_flag = 0;
uint8_t timer2_warning_enable_flag = 0;

//sensor flag
uint8_t timer2_mq7_enable_flag = 0;
uint8_t timer2_gy302_enable_flag = 0;
uint8_t timer2_dht11_enable_flag = 0;


uint32_t timer2_oled_show_mode_count = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM2)
	{
		//basic
		if(timer2_led_count == TIMER2_LED_COUNT)
		{
			timer2_led_enable_flag = ENABLE;
			timer2_led_count = 0;
		}
		if(timer2_oled_count == TIMER2_OLED_COUNT)
		{
			timer2_oled_enable_flag = ENABLE;
			timer2_oled_count = 0;
		}
		if(timer2_esp8266_count == TIMER2_ESP8266_COUNT)
		{
			timer2_esp8266_enable_flag = ENABLE;
			timer2_esp8266_count = 0;
		}
		if(timer2_work_count == TIMER2_WORK_COUNT)
		{
			timer2_work_enable_flag = ENABLE;
			timer2_work_count = 0;
		}
		if(timer2_warning_count == TIMER2_WARNING_COUNT)
		{
			timer2_warning_enable_flag = ENABLE;
			timer2_warning_count = 0;
		}

		//sensor
		if(timer2_mq7_count == 1000)
		{
			timer2_mq7_enable_flag = ENABLE;
			timer2_mq7_count = 0;
		}
		if(timer2_gy302_count == 500)
		{
			timer2_gy302_enable_flag = ENABLE;
			timer2_gy302_count = 0;
		}
		if(timer2_dht11_count == 500)
		{
			timer2_dht11_enable_flag = ENABLE;
			timer2_dht11_count = 0;
		}
		
		if(global_state.oled_state == oled_show_mode)
		{
			timer2_oled_show_mode_count++;
			if(timer2_oled_show_mode_count > 2000)
			{
				global_state.oled_state = oled_show_main;
				timer2_oled_show_mode_count = 0;
			}
		}
	
		//basic
		timer2_led_count++;
		timer2_oled_count++;
		timer2_esp8266_count++;
		timer2_work_count++;
		timer2_warning_count++;
		
		
		//sensor
		timer2_gy302_count++;
		timer2_dht11_count++;
		timer2_mq7_count++;
	}
}
