#ifndef _DRV_STM32_ESP8266_MANAGER_H_
#define _DRV_STM32_ESP8266_MANAGER_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"
#include "stm32f1xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "time.h"

#include "lwrb.h"

#include "tools_esp8266.h"

extern void stm32_esp8266_manager_init(void);
extern void receive_handler_process(void);
extern void enable_wifi_receive_interrupt(void);
#endif
