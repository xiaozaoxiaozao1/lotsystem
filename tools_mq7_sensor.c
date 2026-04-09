#include "tools_mq7_sensor.h"

void tools_mq7_init(mq7_t *mq7, uint16_t cal_ppm, uint16_t rl, uint16_t r0, tools_mq7_getVol_cb getVol_cb)
{
	mq7->CAL_PPM = cal_ppm;
	mq7->RL = rl;
	mq7->R0 = r0;
	
	mq7->MQ7_GET_VOL = getVol_cb;
}

float tools_mq7_get_ppm(mq7_t *mq7)
{
	float temp_vol = mq7->MQ7_GET_VOL();
	float RS = (3.3f - temp_vol) / temp_vol * mq7->RL;
	float temp_ppm = 98.322f * pow(RS/mq7->R0, -1.458f);
	mq7->ppm = temp_ppm;
	return temp_ppm;
}

float tools_mq7_calibration(mq7_t *mq7)
{
	float temp_vol = mq7->MQ7_GET_VOL();
	float RS = 0;
	float R0 = 0;
	RS = (3.3f - temp_vol) / temp_vol * mq7->RL;
	R0 = RS / pow(mq7->CAL_PPM / 98.322, 1 / -1.458f);
	return R0;
}
