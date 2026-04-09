#include "drv_app.h"

extern ADC_HandleTypeDef hadc1;
gy302_t global_gy302;
dht11_t global_dht11;
mq7_t global_mq7;
app_status global_state;
uint8_t mode_mode_pre;

//holdValued
typedef struct _HOLD_VALUE
{
	uint32_t hold_temp_max;
	uint32_t hold_humi_min;
	uint32_t hold_co_max;
}holdValue_t;

holdValue_t global_holdValue;

//oled
u8g2_t global_u8g2;
void show_main(u8g2_t *u8g2);


//MQ7
float mq7_get_vol(void);

//gy302
void gy302_scl_cfg_out(void);
void gy302_sda_cfg_out(void);
void gy302_sda_cfg_in(void);
void gy302_scl_write(uint8_t level);
void gy302_sda_write(uint8_t level);
uint8_t gy302_sda_read(void);

//dht11
void dht11_set_output(void);
void dht11_set_input(void);
void dht11_write_data(uint8_t level);
uint8_t dht11_read_data(void);


//esp8266
esp8266_t global_esp8266;
void mqtt_send(const char * strSend, uint32_t len);

//init
void drv_app_init(void)
{
	//state init
	printf("init state\n");
	init_state(&global_state);
	printf("init state complete!\n");
	
	//hold init
	global_holdValue.hold_co_max = 500;
	global_holdValue.hold_temp_max = 27;
	global_holdValue.hold_humi_min = 30;
	
	//gy302 init
	printf("gy302 init\n");
	tools_gy302_init(&global_gy302, gy302_scl_cfg_out, gy302_sda_cfg_out, gy302_sda_cfg_in, gy302_scl_write, gy302_sda_write, gy302_sda_read, HAL_Delay_us);
	printf("gy302 init complete!\n");
	
	//dht11 init
	printf("dht11 init\n");
	tools_dht11_init(&global_dht11, dht11_set_output, dht11_set_input, dht11_write_data, dht11_read_data, HAL_Delay_us);
	printf("dht11 init complete!\n");
	
	//mq7 init
	printf("mq7 init\n");
	uint8_t temp_mq7_r0;
	tools_mq7_init(&global_mq7, 10, 10, 0, mq7_get_vol);
	temp_mq7_r0 = (uint8_t)tools_mq7_calibration(&global_mq7);
	tools_mq7_init(&global_mq7, 10, 10, temp_mq7_r0, mq7_get_vol);
	printf("mq7 init complete!\n");
	
	//oled init
	printf("oled init\n");
	u8g2Init(&global_u8g2);
	printf("oled init complete!\r\n");
		
		
	enable_wifi_receive_interrupt();
	tools_esp8266_init(&global_esp8266, mqtt_send, tools_esp8266_handle_response, HAL_Delay);
	if(global_esp8266.currentState == ESP8266_ERROR_STATE)
	{
		printf("esp8266 init fail  errorCode:%d\r\n", global_esp8266.errCode);
	}
	else
	{
		printf("esp8266 init complete!\r\n");
	}
	
}

//esp8266
void mqtt_send(const char * strSend, uint32_t len)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)strSend, len, 1000);
}

//oled -------------------------------------------------------------------------------------------------------
void show_main(u8g2_t *u8g2)
{
	char lux_temp[50];
	char humi_temp[50];
	char temp_temp[50];
	char co_temp[50];
	
	u8g2_ClearBuffer(u8g2);
	u8g2_SetFontDirection(u8g2, 0);
	u8g2_SetFont(u8g2, u8g2_font_unifont_t_chinese1);
	
	
	sprintf(temp_temp,"温度: %d.%d °", global_dht11.tempInt, global_dht11.tempDec);
	u8g2_DrawUTF8(u8g2, 0, 15, temp_temp);
	
	sprintf(humi_temp,"湿度: %d.%d %%", global_dht11.humiInt, global_dht11.humiDec);
	u8g2_DrawUTF8(u8g2, 0, 31, humi_temp);
	
	sprintf(lux_temp,"光照: %.2f lux", global_gy302.light_intensity);
	u8g2_DrawUTF8(u8g2, 0, 47, lux_temp);

	sprintf(co_temp,"CO: %.2f ppm", global_mq7.ppm);
	u8g2_DrawUTF8(u8g2, 0, 63, co_temp);

	
	u8g2_SendBuffer(u8g2);
}

void show_mode(u8g2_t *u8g2)
{
	u8g2_ClearBuffer(u8g2);
	u8g2_SetFontDirection(u8g2, 0);
	u8g2_SetFont(u8g2, u8g2_font_unifont_t_chinese1);
	
	if(global_state.mode_state == auto_mode)
	{
		u8g2_DrawUTF8(u8g2, 30, 30, "自动模式");
	}
	else
	{
		u8g2_DrawUTF8(u8g2, 30, 30, "手动模式");
	}
	
	u8g2_SendBuffer(u8g2);
}





//beep ------------------------------------------------------------------------------------------------------------------
void open_beep(void)
{
	HAL_GPIO_WritePin(WARN_BEEP_GPIO_Port, WARN_BEEP_Pin, (GPIO_PinState)0);
}

void close_beep(void)
{
	HAL_GPIO_WritePin(WARN_BEEP_GPIO_Port, WARN_BEEP_Pin, (GPIO_PinState)1);
}

//led ----------------------------------------------------------------------------------------------------
void open_led(void)
{
	HAL_GPIO_WritePin(WARN_LIGHT_GPIO_Port, WARN_LIGHT_Pin, (GPIO_PinState)1);
}

void close_led(void)
{
	HAL_GPIO_WritePin(WARN_LIGHT_GPIO_Port, WARN_LIGHT_Pin, (GPIO_PinState)0);

}

//open fan ----------------------------------------------------------------------------------------------------
void open_fan(void)
{
	HAL_GPIO_WritePin(OPEN_FAN_GPIO_Port, OPEN_FAN_Pin, (GPIO_PinState)1);
}

void close_fan(void)
{
	HAL_GPIO_WritePin(OPEN_FAN_GPIO_Port, OPEN_FAN_Pin, (GPIO_PinState)0);
}

//open light ----------------------------------------------------------------------------------------------------
void open_light(void)
{
	HAL_GPIO_WritePin(OPEN_LIGHT_GPIO_Port, OPEN_LIGHT_Pin, (GPIO_PinState)1);
}

void close_light(void)
{
	HAL_GPIO_WritePin(OPEN_LIGHT_GPIO_Port, OPEN_LIGHT_Pin, (GPIO_PinState)0);
}

//open humi
void open_humi(void)
{
	HAL_GPIO_WritePin(OPEN_HUMI_GPIO_Port, OPEN_HUMI_Pin, (GPIO_PinState)1);
}

void close_humi(void)
{
	HAL_GPIO_WritePin(OPEN_HUMI_GPIO_Port, OPEN_HUMI_Pin, (GPIO_PinState)0);
}

//DHT11------------------------------------------------------------------------------------------------
void dht11_set_output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

void dht11_set_input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

void dht11_write_data(uint8_t level)
{
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, (GPIO_PinState)level);
}

uint8_t dht11_read_data(void)
{
	return HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin);
}

//MQ2---------------------------------------------------------------------------------------------------
float mq7_get_vol(void)
{
	uint16_t temp_adc_value;
	float temp_vol;
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_8;                                   /* 通道 */
	sConfig.Rank = ADC_REGULAR_RANK_1;                              
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;                  /* 采样时间 */
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)             
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	temp_adc_value = HAL_ADC_GetValue(&hadc1);
	
	temp_vol = (temp_adc_value * 3.3 * 2) /4096;
//	printf("temp_vol is : %.2f\r\n", temp_vol);
	return temp_vol;
}

//gy302 ----------------------------------------------------------------------------------------------------
void gy302_scl_cfg_out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GY302_SCL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GY302_SCL_GPIO_Port, &GPIO_InitStruct);
}

void gy302_sda_cfg_out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GY302_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GY302_SDA_GPIO_Port, &GPIO_InitStruct);
}

void gy302_sda_cfg_in(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GY302_SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GY302_SDA_GPIO_Port, &GPIO_InitStruct);
}

void gy302_scl_write(uint8_t level)
{
	HAL_GPIO_WritePin(GY302_SCL_GPIO_Port, GY302_SCL_Pin, (GPIO_PinState)level);
}

void gy302_sda_write(uint8_t level)
{
	HAL_GPIO_WritePin(GY302_SDA_GPIO_Port,GY302_SDA_Pin, (GPIO_PinState)level);
}

uint8_t gy302_sda_read(void)
{
	return HAL_GPIO_ReadPin(GY302_SDA_GPIO_Port, GY302_SDA_Pin);
}


//delay us -------------------------------------------------------------------------------------------------
void HAL_Delay_us(uint16_t us)
{
	uint16_t counter = 0;
	__HAL_TIM_SET_COUNTER(&htim3, counter); //
	HAL_TIM_Base_Start(&htim3); //
	while(__HAL_TIM_GET_COUNTER(&htim3) < us) //
	{
	}
	HAL_TIM_Base_Stop(&htim3); //
}

//oled process -----------------------------------------------------------------------------------------------------
void oled_process(void)
{
	if(global_state.oled_state == oled_show_main)
	{
		show_main(&global_u8g2);
	}
	else if(global_state.oled_state == oled_show_mode)
	{
		show_mode(&global_u8g2);
	}
}

void esp8266_send_process(void)
{
	uint8_t app_send_state = 1;
	
	
	if(app_send_state == 0)
	{
		printf("send to aliyun is success\r\n");
	}
	else
	{
		printf("send to aliyun is fail\r\n");
	}
	
	//post to app
	char data[256];
	sprintf(data, "temp:%d.%d,humi:%d.%d,light:%.2f,co:%.2f,lightState:%d,humiState:%d,fanState:%d", global_dht11.tempInt, global_dht11.tempDec, global_dht11.humiInt, global_dht11.humiDec, global_gy302.light_intensity, global_mq7.ppm, global_state.light_state, global_state.humi_state, global_state.fan_state);
	app_send_state = tools_esp8266_send_to_app(&global_esp8266, data, strlen(data));
	if(app_send_state == 0)
	{
		printf("send to app is success\r\n");
	}
	else
	{
		printf("send to app is fail\r\n");
	}
	
	printf("tempMax:%d,humiMin:%d,CoMax:%d,lightSwitch:%d,humiSwitch:%d,fanSwitch:%d,mode:%d\r\n",global_holdValue.hold_temp_max, global_holdValue.hold_humi_min, global_holdValue.hold_co_max, global_state.light_state, global_state.humi_state, global_state.fan_state, global_state.mode_state);
}

//work process ----------------------------------------------------------------------------------------------------
void work_process(void)
{
	if(global_state.mode_state == auto_mode)
	{
		if(global_dht11.tempInt > global_holdValue.hold_temp_max)
		{
			global_state.fan_state = fan_open;
		}
		
		if(global_dht11.humiInt < global_holdValue.hold_humi_min)
		{
			global_state.humi_state = humi_open;
		}
	}
	
	
	if(global_state.fan_state == fan_open)
	{
		open_fan();
	}
	else
	{
		close_fan();
	}
	
	if(global_state.humi_state == humi_open)
	{
		open_humi();
	}
	else
	{
		close_humi();
	}

	if(global_state.light_state == light_open)
	{
		open_light();
	}
	else
	{
		close_light();
	}
}

//warning process  ----------------------------------------------------------------------------------------------------
void warning_process(void)
{
	static uint8_t retry_count = 0;
	
	if(global_mq7.ppm > global_holdValue.hold_co_max)
	{
		global_state.warn_state = warning_open;
	}
	else
	{
		global_state.warn_state = warning_close;
	}
	
	if(global_state.warn_state == warning_open)
	{
	
		if(retry_count%40 == 0)
		{
			close_beep();
			close_led();
		}
		else
		{
			open_beep();
			open_led();
		}
		
		if((retry_count%100 == 0))
		{
			HAL_UART_Transmit(&huart2, "warning", strlen("warning"), 1000);
			retry_count = 0;
		}
		retry_count++;
	}
	else
	{
		close_beep();
		close_led();
		retry_count = 0;
	}
}


//holdVlaue set
void drv_app_response(const char *data)
{
	char *fixedpart = "tempMax";
	char *start = strstr(data, fixedpart);
	if(start == NULL)
	{
		return;
	}
	int temp = 0;
	char* token = strtok(start, ",");
	while (token != NULL) 
	{
		// Use sscanf to extract the key-value pairs
			if (sscanf(token, "tempMax:%d", &global_holdValue.hold_temp_max) == 1) 
			{	
			} 
			else if (sscanf(token, "humiMin:%d", &global_holdValue.hold_humi_min) == 1) 
			{
			} 
			else if (sscanf(token, "coMax:%d", &global_holdValue.hold_co_max) == 1) 
			{
			} 
			else if (sscanf(token, "lightSwState:%d", &temp) == 1) 
			{
				if(temp == 1)
				{
					global_state.light_state = light_open;
				}
				else if(temp == 0)
				{
					global_state.light_state = light_close;
				}
			} 
			else if (sscanf(token, "humiSwState:%d", &temp) == 1) 
			{
				if(temp == 1)
				{
					global_state.humi_state = humi_open;
				}
				else if(temp == 0)
				{
					global_state.humi_state = humi_close;
				}
			} 
			else if (sscanf(token, "fanSwState:%d", &temp) == 1) 
			{
				if(temp == 1)
				{
					global_state.fan_state = fan_open;
				}
				else if(temp == 0)
				{
					global_state.fan_state = fan_close;
				}
			} 
			else if (sscanf(token, "modeSwState:%d", &temp) == 1) 
			{
				if(mode_mode_pre != temp)
				{
					global_state.oled_state = oled_show_mode;
				}
				if(temp == 1)
				{
					global_state.mode_state = auto_mode;
					mode_mode_pre = 1;
				}
				else if(temp == 0)
				{
					global_state.mode_state = mu_mode;
					mode_mode_pre = 0;
				}
			}

			// Get the next token
			token = strtok(NULL, ",");
	}
}
