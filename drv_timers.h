#ifndef _DRV_TIMER_H_
#define _DRV_TIMER_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"
#include "stm32f1xx.h"
#include "main.h"
#include "stdio.h"

#include "multi_button.h"
#include "drv_app.h"
//---
#define TIMER2_LED_COUNT 1000
#define TIMER2_OLED_COUNT 500
#define TIMER2_ESP8266_COUNT 5000
#define TIMER2_WORK_COUNT 100
#define TIMER2_WARNING_COUNT 100


#define ENABLE 1
#define DISENABLE 0

//basic
extern uint8_t timer2_led_enable_flag;
extern uint8_t timer2_oled_enable_flag;
extern uint8_t timer2_esp8266_enable_flag;
extern uint8_t timer2_work_enable_flag;
extern uint8_t timer2_warning_enable_flag;

//sensor
extern uint8_t timer2_gy302_enable_flag;
extern uint8_t timer2_dht11_enable_flag;
extern uint8_t timer2_mq7_enable_flag;

#endif
