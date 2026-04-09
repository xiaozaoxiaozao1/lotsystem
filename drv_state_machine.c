#include "drv_state_machine.h"


void init_state(app_status * state)
{
	state->fan_state = fan_close;
	state->humi_state = humi_close;
	state->light_state = light_close;
	state->mode_state = mu_mode;
	state->warn_state = warning_close;
	state->oled_state = oled_show_main;
}




