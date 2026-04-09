#ifndef _DRV_APP_H_
#define _DRV_APP_H_

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "inttypes.h"
#include "time.h"

#include "main.h"

#include "drv_state_machine.h"
#include "drv_timers.h"
#include "drv_stm32_esp8266_manager.h"


#include "tools_mq7_sensor.h"
#include "tools_gy_302.h"
#include "tools_dht11.h"

#include "tools_esp8266.h"

//全局阈值结构体
typedef struct _threshold_struct
{
	uint32_t holdTempMin;					//体温最低阈值
	uint32_t holdBloodMin;				//血氧最高阈值
	uint32_t holdHRMin;						//心率最低阈值
	uint32_t holdMq2Max;					//烟雾最高阈值
}threshold_struct;



extern esp8266_t global_esp8266;

extern gy302_t global_gy302;
extern dht11_t global_dht11;
extern mq7_t global_mq7;
extern app_status global_state;

extern void drv_app_init(void);
extern void oled_process(void);
extern void esp8266_send_process(void);
extern void work_process(void);
extern void warning_process(void);
extern void HAL_Delay_us(uint16_t us);

extern void open_beep(void);
extern void close_beep(void);
extern void open_led(void);
extern void close_led(void);
extern void open_fan(void);
extern void close_fan(void);
extern void open_light(void);
extern void close_light(void);



#endif
