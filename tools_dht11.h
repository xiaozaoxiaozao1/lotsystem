#ifndef _TOOLS_DHT11_H_
#define _TOOLS_DHT11_H_
#include "stdio.h"
#include "stdint.h"


typedef void (*tools_dht11_cfg_out)(void);			//set gpio is output mode
typedef void (*tools_dht11_cfg_in)(void);				//set gpio is in mode
typedef void (*tools_dht11_write)(uint8_t);			//gpio out level
typedef uint8_t (*tools_dht11_read)(void);			//gpio read
typedef void (*tools_dht11_delay_us)(uint16_t);	//delay_us 

typedef enum _dht11_read_status
{
	success = 1,
	checkError,
	sumError
}dht11Status;

typedef struct _dht11_struct
{
	uint8_t tempInt;									//The integer part of temperature
	uint8_t tempDec;									//The decimal part of temperature
	uint8_t humiInt;									//The integer part of humidity
	uint8_t humiDec;									//The decimal part of humidity
	
	tools_dht11_cfg_out	 		DAT_CFG_OUT;	//callback about data cfg is output
	tools_dht11_cfg_in  		DAT_CFG_IN;		//callback about data cfg is input
	tools_dht11_write   		DAT_WRITE;		//callback about data write a level     heigh or low
	tools_dht11_read    		DAT_READ;			//callback about data read 
	tools_dht11_delay_us		DELAY_US;
}dht11_t;

extern void tools_dht11_init(dht11_t *dht11, tools_dht11_cfg_out cfg_out, tools_dht11_cfg_in cfg_in, tools_dht11_write write_data, tools_dht11_read read_data, tools_dht11_delay_us delay_us);
extern dht11Status tools_dht11_read_data_once(dht11_t *dht11);


#endif
