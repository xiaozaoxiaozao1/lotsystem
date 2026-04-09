#include "tools_esp8266.h"

#define ESP8266_INIT							"AT\r\n"
#define ESP8266_RST 							"AT+RST\r\n"
#define ESP8266_ATE_OFF						"ATE0\r\n"
#define ESP8266_AP_STA 						"AT+CWMODE=3\r\n"
#define ESP8266_START_MULTI_lINK 	"AT+CIPMUX=1\r\n"
#define ESP8266_ESTABLISH_SERVER  "AT+CIPSERVER=1\r\n"

static void tools_esp8266_is_step_error(esp8266_t *esp8266);
static uint8_t retry_count = 0;

extern void drv_app_response(const char *data);

void tools_esp8266_init(esp8266_t *esp8266, tools_esp8266_sent_at_cb sendFunc, tools_esp8266_handle_response_cb responseFunc, tools_esp8266_delay_ms_cb delayFunc)
{
	
	esp8266->currentState = ESP8266_INIT_STATE;
	esp8266->messageState = MESSAGE_IDLE;
	esp8266->appConnectedState = ESP8266_APP_DISCONNECTED;
	esp8266->errCode = ERROR_NONE;
	
	esp8266->sendFunc = sendFunc;
	esp8266->handleResponse = responseFunc;
	esp8266->delayMs = delayFunc;
	
	while((esp8266->currentState != ESP8266_ERROR_STATE) && (esp8266->currentState != ESP8266_READY_STATE))
	{
		char temp[256];
		switch(esp8266->currentState)
		{
			case ESP8266_INIT_STATE:
				esp8266->sendFunc(ESP8266_INIT, strlen(ESP8266_INIT));
				esp8266->delayMs(100);
				tools_esp8266_is_step_error(esp8266);
				break;
			
			case ESP8266_RESET_STATE:
				esp8266->sendFunc(ESP8266_RST, strlen(ESP8266_RST));
				esp8266->delayMs(100);
				tools_esp8266_is_step_error(esp8266);
				break;
			
			case ESP8266_ATE_OFF_STATE:
				esp8266->sendFunc(ESP8266_ATE_OFF, strlen(ESP8266_ATE_OFF));
				esp8266->delayMs(100);
				tools_esp8266_is_step_error(esp8266);
				break;
			
			case ESP8266_SET_MODE_STATE:
				esp8266->sendFunc(ESP8266_AP_STA, strlen(ESP8266_AP_STA));
				esp8266->delayMs(100);
				tools_esp8266_is_step_error(esp8266);
				break;
			
			case ESP8266_CONNECT_WIFI_STATE:
				sprintf(temp, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_NAME, WIFI_PASS);
				esp8266->sendFunc(temp, strlen(temp));
				esp8266->delayMs(100);
				tools_esp8266_is_step_error(esp8266);
				break;
			
			case ESP8266_SET_WIFI_STATE:
				sprintf(temp, "AT+CWSAP=\"%s\",\"%s\",5,4\r\n", AP_NAME, AP_PASS);
				esp8266->sendFunc(temp, strlen(temp));
				esp8266->delayMs(100);
				tools_esp8266_is_step_error(esp8266);
				break;
			
			case ESP8266_START_MULTI_LINK_STATE:
				esp8266->sendFunc(ESP8266_START_MULTI_lINK, strlen(ESP8266_START_MULTI_lINK));
				esp8266->delayMs(200);
				tools_esp8266_is_step_error(esp8266);
				break;
			
			case ESP8266_ESTABLISH_SERVER_STATE:
				esp8266->sendFunc(ESP8266_ESTABLISH_SERVER, strlen(ESP8266_ESTABLISH_SERVER));
				esp8266->delayMs(200);
				tools_esp8266_is_step_error(esp8266);
				break;
			
			default:
				break;
		}
	}
}

void tools_esp8266_handle_response(void* context, const char* response)
{
	esp8266_t *esp8266 = (esp8266_t*)context;
	char *temp = NULL;
	
	switch(esp8266->currentState)
	{
		case ESP8266_INIT_STATE:
			temp = strstr(response, "OK");
			if(temp != NULL)
			{
				esp8266->currentState = ESP8266_RESET_STATE;
				retry_count = 0;
			}
			break;
		
		case ESP8266_RESET_STATE:
			temp = strstr(response, "ready");
			if(temp != NULL)
			{
				esp8266->currentState = ESP8266_ATE_OFF_STATE;
				retry_count = 0;
			}
			break;
		
		case ESP8266_ATE_OFF_STATE:
			temp = strstr(response, "OK");
			if(temp != NULL)
			{
				esp8266->currentState = ESP8266_SET_MODE_STATE;
				retry_count = 0;
			}
			break;
		
		case ESP8266_SET_MODE_STATE:
			temp = strstr(response, "OK");
			if(temp != NULL)
			{
				esp8266->currentState = ESP8266_CONNECT_WIFI_STATE;
				retry_count = 0;
			}
			break;
		
		case ESP8266_CONNECT_WIFI_STATE:
			temp = strstr(response, "OK");
			if(temp != NULL)
			{
				esp8266->currentState = ESP8266_SET_WIFI_STATE;
				retry_count = 0;
			}
			break;
		
		case ESP8266_SET_WIFI_STATE:
			temp = strstr(response, "OK");
			if(temp != NULL)
			{
				esp8266->currentState = ESP8266_START_MULTI_LINK_STATE;
				retry_count = 0;
			}
			break;
		
		case ESP8266_START_MULTI_LINK_STATE:
			temp = strstr(response, "OK");
			if(temp != NULL)
			{
				esp8266->currentState = ESP8266_ESTABLISH_SERVER_STATE;
				retry_count = 0;
			}
			break;
		
		case ESP8266_ESTABLISH_SERVER_STATE:
			temp = strstr(response, "OK");
			if(temp != NULL)
			{
				esp8266->currentState = ESP8266_READY_STATE;
				retry_count = 0;
			}
			break;
		
		case ESP8266_READY_STATE:
			if(strstr(response, "OK") || (strstr(response, "ERROR")))
			{
				esp8266->messageState = MESSAGE_SEND_OK;
			}
			if(strstr(response, ">"))
			{
				esp8266->messageState = MESSAGE_PROMPT;
			}
			if(strstr(response, "+STA_CONNECTED"))
			{
				esp8266->appConnectedState = ESP8266_APP_CONNECTED;
			}
			if(strstr(response, "+STA_DISCONNECTED"))
			{
				esp8266->appConnectedState = ESP8266_APP_DISCONNECTED;
			}
			
			drv_app_response(response);
			break;
			
		default:
			break;
	}
}

static void tools_esp8266_is_step_error(esp8266_t *esp8266)
{
	switch(esp8266->currentState)
	{
		case ESP8266_INIT_STATE:
			if(retry_count >= 60)
			{
				esp8266->currentState = ESP8266_ERROR_STATE;
				esp8266->errCode = ERROR_INIT;
			}
			break;
		
		case ESP8266_RESET_STATE:
			if(retry_count >= 60)
			{
				esp8266->currentState = ESP8266_ERROR_STATE;
				esp8266->errCode = ERROR_RESET;
			}
			break;
		
		case ESP8266_ATE_OFF_STATE:
			if(retry_count >= 60)
			{
				esp8266->currentState = ESP8266_ERROR_STATE;
				esp8266->errCode = ERROR_ATE_OFF;
			}
			break;
		
		case ESP8266_SET_MODE_STATE:
			if(retry_count >= 60)
			{
				esp8266->currentState = ESP8266_ERROR_STATE;
				esp8266->errCode = ERROR_SET_MODE;
			}
			break;
		
		case ESP8266_CONNECT_WIFI_STATE:
			if(retry_count >= 60)
			{
				esp8266->currentState = ESP8266_ERROR_STATE;
				esp8266->errCode = ERROR_CONNECT_WIFI;
			}
			break;
		
		case ESP8266_SET_WIFI_STATE:
			if(retry_count >= 60)
			{
				esp8266->currentState = ESP8266_ERROR_STATE;
				esp8266->errCode = ERROR_SET_WIFI;
			}
			break;
		
		case ESP8266_START_MULTI_LINK_STATE:
			if(retry_count >= 60)
			{
				esp8266->currentState = ESP8266_ERROR_STATE;
				esp8266->errCode = ERROR_START_MULTI_LINK;
			}
			break;
		
		case ESP8266_ESTABLISH_SERVER_STATE:
			if(retry_count >= 60)
			{
				esp8266->currentState = ESP8266_ERROR_STATE;
				esp8266->errCode = ERROR_ESTABLISH_SERVER;
			}
			break;
		
		default:
			break;
	}
	retry_count++;
}

/*
 * @brief		esp8266 send value to app
 * @param		esp8266_t*		 value     value_len
 * @return	0:success   				1:fail
*/
uint8_t tools_esp8266_send_to_app(esp8266_t *esp8266, const char *value, uint32_t len)
{
	if(esp8266->appConnectedState == ESP8266_APP_CONNECTED)
	{
		uint32_t retry_count = 0;
		char temp_str[50];
		uint32_t temp_str_len;
		
		temp_str_len = sprintf(temp_str, "AT+CIPSEND=%d,%d\r\n", 0, len);
		esp8266->sendFunc(temp_str, temp_str_len);
		
		while(esp8266->messageState != MESSAGE_PROMPT)
		{
			if(retry_count > 500)
			{
				esp8266->messageState = MESSAGE_IDLE;
				return 1;
			}
			retry_count++;
			esp8266->delayMs(10);
		}
		
		retry_count = 0;
		esp8266->sendFunc(value, len);
		while(esp8266->messageState != MESSAGE_SEND_OK)
		{
			if(retry_count > 50)
			{
				esp8266->messageState = MESSAGE_IDLE;
				return 1;
			}
			retry_count++;
			esp8266->delayMs(10);
		}
		
		esp8266->messageState = MESSAGE_IDLE;
		return 0;
	}
	else
	{
		return 1;
	}
}
