#include "tools_dht11.h"

void tools_dht11_init(dht11_t *dht11, tools_dht11_cfg_out cfg_out, tools_dht11_cfg_in cfg_in, tools_dht11_write write_data, tools_dht11_read read_data, tools_dht11_delay_us delay_us)
{
	dht11->tempInt = 0;
	dht11->tempDec = 0;
	dht11->humiInt = 0;
	dht11->humiDec = 0;
	
	dht11->DAT_CFG_OUT = cfg_out;
	dht11->DAT_CFG_IN = cfg_in;
	dht11->DAT_WRITE = write_data;
	dht11->DAT_READ = read_data;
	dht11->DELAY_US = delay_us;
}


void tools_dht11_reset(dht11_t *dht11)
{
	//cfg gpio is output
	dht11->DAT_CFG_OUT();
	
//	//write gpio is heigh and delay 20us
	dht11->DAT_WRITE(1);
	dht11->DELAY_US(20);
	
	//write gpio is low and delay  at least 18ms
	dht11->DAT_WRITE(0);
	dht11->DELAY_US(1000*18);
	
	//wirte gpio is heigh and delay 20~40us
	dht11->DAT_WRITE(1);
	dht11->DELAY_US(20);
}


uint8_t tools_dht11_check(dht11_t *dht11)
{
	uint32_t retry = 0;
	
	//cfg gpio is input
	dht11->DAT_CFG_IN();
	
	//when data is low jump
	while(dht11->DAT_READ() && retry < 100)
	{
		retry++;
		dht11->DELAY_US(1);
	}
	if(retry >= 100)
	{
//		printf("1retry is >100 retry %d\r\n", retry);
		return 1;
	}
	else 
	{
		retry = 0;
	}
	
	//when data is heigh jump
	while(!dht11->DAT_READ() && retry < 100)
	{
		retry++;
		dht11->DELAY_US(1);
	}
	if(retry >= 100)
	{
//		printf("2retry is >100 retry %d\r\n", retry);
		return 1;
	}
	
	return 0;
}

uint8_t tools_dht11_read_bit(dht11_t *dht11)
{
	uint32_t retry = 0;
	
	//when data level is low jump
	while(dht11->DAT_READ() && retry < 100)
	{
		retry++;
		dht11->DELAY_US(1);
	}
	retry=0;
	//when data level is heigh jump
	while(!dht11->DAT_READ() && retry < 100)
	{
		retry++;
		dht11->DELAY_US(1);
	}
	//if after 40us, data level is also is 1, the bit data is 1 
	dht11->DELAY_US(33);
	if(dht11->DAT_READ()) 
		return 1;
	else 
		return 0;
}

uint8_t tools_dht11_read_byte(dht11_t *dht11)
{
	uint8_t i, data = 0;
	for(i = 0; i < 8; i++)
	{
		data <<= 1;
		data |= tools_dht11_read_bit(dht11);
	}
	return data;
}

dht11Status tools_dht11_read_data_once(dht11_t *dht11)
{
	uint8_t buf[5];
	uint8_t i;
	tools_dht11_reset(dht11);
	if(tools_dht11_check(dht11) == 0)
	{
		for(i = 0; i < 5; i++)
		{
			buf[i] = tools_dht11_read_byte(dht11);
		}
		if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
		{
			dht11->humiInt = buf[0];
			dht11->humiDec = buf[1];
			dht11->tempInt = buf[2];
			dht11->tempDec = buf[3];
			return success;
		}
		else
		{
			return sumError;
		}
	}
	else
	{
		return checkError;
	}
}

