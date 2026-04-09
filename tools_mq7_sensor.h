#ifndef _TOOLS_MQ7_SENSOR_H_
#define _TOOLS_MQ7_SENSOR_H_

#include "stdint.h"
#include "math.h"

typedef float (*tools_mq7_getVol_cb)(void);

typedef struct _MQ7_Struct
{
	float ppm;
	
	uint16_t CAL_PPM;
	uint16_t RL;
	uint16_t R0;
	
	tools_mq7_getVol_cb MQ7_GET_VOL;
}mq7_t;

extern void tools_mq7_init(mq7_t *mq7, uint16_t cal_ppm, uint16_t rl, uint16_t r0, tools_mq7_getVol_cb getVol_cb);
extern float tools_mq7_get_ppm(mq7_t *mq7);
extern float tools_mq7_calibration(mq7_t *mq7);

#endif
