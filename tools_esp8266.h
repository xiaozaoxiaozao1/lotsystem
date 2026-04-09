#ifndef _TOOLS_ESP8266_H_
#define _TOOLS_ESP8266_H_

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "inttypes.h"

#define WIFI_NAME ""
#define WIFI_PASS ""

#define AP_NAME "STM32System"
#define AP_PASS "123456789"


//esp8266 connect status
typedef enum _ESP8266_STATUS
{
	  ESP8266_INIT_STATE = 0,                 		// Initialize ESP8266
    ESP8266_RESET_STATE = 1,                   	// Reset ESP8266
    ESP8266_ATE_OFF_STATE = 2,                 	// Turn off echo
    ESP8266_SET_MODE_STATE = 3,                	// Set mode to AP+STA
    ESP8266_CONNECT_WIFI_STATE = 4,            	// Connect to WiFi
		ESP8266_SET_WIFI_STATE = 5,                	// Set WiFi
    ESP8266_START_MULTI_LINK_STATE = 6,        	// Start multiple links
    ESP8266_ESTABLISH_SERVER_STATE = 7,        // Establish server

    ESP8266_READY_STATE = 8,                   // ESP8266 initialization complete
    ESP8266_ERROR_STATE = 9                    // ESP8266 error
}esp8266_state;

//esp8266 connected app state
typedef enum _ESP8266_APP_CONNECTED_STATUS
{
	ESP8266_APP_DISCONNECTED = 0,
	ESP8266_APP_CONNECTED
}esp8266_app_connected_state;

//esp8266 send message status
typedef enum _ESP8266_MESSAGE_STATUS
{
	MESSAGE_IDLE = 0,
	MESSAGE_PROMPT = 1,
	MESSAGE_SEND_OK = 2
}esp8266_message_state;

//esp8266 error code
typedef enum _ESP8266_ERROR_CODE
{
		ERROR_NONE = 0,              				// No error
		ERROR_INIT = 1,                    	// Initialize ESP8266
		ERROR_RESET = 2,                   	// Reset ESP8266
    ERROR_ATE_OFF = 3,                 	// Turn off echo
    ERROR_SET_MODE = 4,                	// Set mode to AP+STA
    ERROR_CONNECT_WIFI = 5,            	// Connect to WiFi
		ERROR_SET_WIFI = 6,                	// Set WiFi
    ERROR_START_MULTI_LINK = 7,        // Start multiple links
    ERROR_ESTABLISH_SERVER = 8,        // Establish server
	
		ERROR_UNKNOWN = 99           				// Unknown error
}esp8266_error_code;

//call back
typedef void (*tools_esp8266_sent_at_cb)(const char*, uint32_t);
typedef void (*tools_esp8266_handle_response_cb)(void *, const char*);
typedef void (*tools_esp8266_delay_ms_cb)(uint32_t);

//esp8266 st
typedef struct _ESP8266_STRUCT
{
	esp8266_state 											currentState;
	esp8266_message_state								messageState;
	esp8266_app_connected_state					appConnectedState;
	esp8266_error_code 									errCode;
	tools_esp8266_sent_at_cb 						sendFunc;
	tools_esp8266_handle_response_cb		handleResponse;
	tools_esp8266_delay_ms_cb						delayMs;
}esp8266_t;

extern void tools_esp8266_init(esp8266_t *esp8266, tools_esp8266_sent_at_cb sendFunc, tools_esp8266_handle_response_cb responseFunc, tools_esp8266_delay_ms_cb delayFunc);
extern void tools_esp8266_handle_response(void* context, const char* response);

//send to app
extern uint8_t tools_esp8266_send_to_app(esp8266_t *esp8266, const char *value, uint32_t len);

#endif
