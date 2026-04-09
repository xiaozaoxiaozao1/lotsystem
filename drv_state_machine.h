#ifndef _DRV_STATE_MACHINE_H_
#define _DRV_STATE_MACHINE_H_

#include "stdint.h"

typedef enum _FAN_SWITCH_STATE
{
	fan_close = 0,
	fan_open
}fan_switch_state;

typedef enum _HUMI_SWITCH_STATE
{
	humi_close = 0,
	humi_open
}humi_switch_state;

typedef enum _LIGHT_SWITCH_STATE
{
	light_close = 0,
	light_open
}light_switch_state;

typedef enum _MODE_SWITCH_STATE
{
	mu_mode = 0,
	auto_mode
}mode_switch_state;

typedef enum _WARNING_STATE
{
	warning_close = 0,
	warning_open
}warning_state;

typedef enum _OLED_STATE
{
	oled_show_main = 0,
	oled_show_mode
}oled_show_state;

typedef struct _app_status_st
{
	fan_switch_state fan_state;
	humi_switch_state humi_state;
	light_switch_state light_state;
	mode_switch_state mode_state;
	warning_state warn_state;
	oled_show_state oled_state;
}app_status;

extern void init_state(app_status * state);

#endif
