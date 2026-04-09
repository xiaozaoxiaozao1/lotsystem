#include "tools_gy_302.h"


static void tools_i2c_start(gy302_t *gy302)
{
	//config sda direction out
	gy302->SDA_CFG_OUT();
	//sda scl output heigh
	gy302->SDA_WRITE(1);
	gy302->DELAY_US(4);
	gy302->SCL_WRITE(1);
	//sda output low
	gy302->DELAY_US(4);
	gy302->SDA_WRITE(0);
	//scl output low
	gy302->DELAY_US(10);
	gy302->SCL_WRITE(0);
	gy302->DELAY_US(4);
}

static void tools_i2c_stop(gy302_t *gy302)
{
	//config sda direction out
	gy302->SDA_CFG_OUT();
	//scl output low
	gy302->SCL_WRITE(0);
	gy302->DELAY_US(10);
	//sda output low
	gy302->SDA_WRITE(0);
	gy302->DELAY_US(10);
	//scl output heigh
	gy302->SCL_WRITE(1);
	gy302->DELAY_US(10);
	//sda output heigh
	gy302->SDA_WRITE(1);
	gy302->DELAY_US(10);
}

static void tools_i2c_send_ack(gy302_t *gy302)
{
	//config sda direction out
	gy302->SDA_CFG_OUT();
	//scl output low
	gy302->SCL_WRITE(0);
	//sda output low
	gy302->SDA_WRITE(0);
	gy302->DELAY_US(5);
	//scl output heigh
	gy302->SCL_WRITE(1);
	gy302->DELAY_US(5);
	//scl output low
	gy302->SCL_WRITE(0);
	gy302->DELAY_US(5);
}

static void tools_i2c_send_nack(gy302_t *gy302)
{
	//config sda direction out
	gy302->SDA_CFG_OUT();
	//scl outout low
	gy302->SCL_WRITE(0);
	//sda output heigh
	gy302->SDA_WRITE(1);
	gy302->DELAY_US(5);
	//scl output heigh
	gy302->SCL_WRITE(1);
	gy302->DELAY_US(5);
	//scl outouut low
	gy302->SCL_WRITE(0);
	gy302->DELAY_US(5);
}

static void tools_i2c_send_byte(gy302_t *gy302, uint8_t send_data)
{
	uint8_t bit_times;
	//config sda direction out
	gy302->SDA_CFG_OUT();
	//scl output low
	gy302->SCL_WRITE(0);
	gy302->DELAY_US(5);
	//send bit
	for(bit_times = 0; bit_times < 8; bit_times++)
	{
		if((send_data & 0x80) >> 7)
		{
			gy302->SDA_WRITE(1);
		}
		else
		{
			gy302->SDA_WRITE(0);
		}
		
		send_data <<= 1;
		gy302->DELAY_US(5);
		
		//scl output heigh
		gy302->SCL_WRITE(1);
		gy302->DELAY_US(5);
		//scl output low
		gy302->SCL_WRITE(0);
		gy302->DELAY_US(5);
	}
}

static uint8_t tools_i2c_read_byte(gy302_t *gy302, uint8_t ack)
{
	uint8_t bit_times;
	uint8_t i2c_buffer;
	//config sda direction in
	gy302->SDA_CFG_IN();
	//read
	for(bit_times = 0; bit_times < 8; bit_times++)
	{
		//scl output low
		gy302->SCL_WRITE(0);
		gy302->DELAY_US(5);
		
		//scl output heigh
		gy302->SCL_WRITE(1);
		i2c_buffer <<= 1;
		if(gy302->SDA_READ()) i2c_buffer++;
		gy302->DELAY_US(15);
	}
	if(!ack)
	{
		tools_i2c_send_nack(gy302);
	}
	else
	{
		tools_i2c_send_ack(gy302);
	}
	return i2c_buffer;
}

static uint8_t tools_i2c_read_ack(gy302_t *gy302)
{
	uint8_t ack = 0;
	//config sda direction out
	gy302->SDA_CFG_OUT();
	//sda output heigh
	gy302->SDA_WRITE(1);
	//config sda in
	gy302->SDA_CFG_IN();
	gy302->DELAY_US(5);
	//scl output heigh
	gy302->SCL_WRITE(1);
	gy302->DELAY_US(5);
	
	while(gy302->SDA_READ())
	{
		ack++;
		if(ack > 250)
		{
			return 1;
		}
	}
	gy302->SCL_WRITE(0);
	return 0;
}

void BH1750_WriteByte(gy302_t *gy302, uint8_t addr)
{
	tools_i2c_start(gy302);
	
	tools_i2c_send_byte(gy302, BH1750_Addr_GND_REG << 1|0);
	while(tools_i2c_read_ack(gy302) == 1){}
	
	tools_i2c_send_byte(gy302, addr);
	while(tools_i2c_read_ack(gy302) == 1){}
	
	tools_i2c_stop(gy302);
}

uint16_t BH1750_Read_Data(gy302_t *gy302)
{
	uint8_t H_Data;
	uint8_t L_Data;
	uint16_t RX_Data;
	
	tools_i2c_start(gy302);
	
	tools_i2c_send_byte(gy302, BH1750_Addr_GND_REG << 1|1);
	while(tools_i2c_read_ack(gy302) == 1){}
	
	H_Data = tools_i2c_read_byte(gy302, 1);
	L_Data = tools_i2c_read_byte(gy302, 0);
		
	tools_i2c_stop(gy302);
	
	RX_Data = ((uint16_t)H_Data<<8) + L_Data;
	return RX_Data;
}

/**	BH1750上电 **/
void BH1750_Power_ON(gy302_t *gy302)
{
	BH1750_WriteByte(gy302, BH1750_Power_ON_REG);
}

/**	BH1750断电 **/
void BH1750_Power_OFF(gy302_t *gy302)
{
	BH1750_WriteByte(gy302, BH1750_Power_OFF_REG);
}

/**	BH1750数据寄存器复位 **/
void BH1750_RESET(gy302_t *gy302)
{
	BH1750_WriteByte(gy302, BH1750_MODULE_RESET_REG);
}

void tools_gy302_init(gy302_t *gy302, tools_gy302_scl_cfg_out_cb SCL_CFG_OUT_CB,
																			tools_gy302_sda_cfg_out_cb SDA_CFG_OUT_CB,
																			tools_gy302_sda_cfg_in_cb SDA_CFG_IN_CB,
																			tools_gy302_scl_write SCL_WRITE_CB,
																			tools_gy302_sda_write SDA_WRITE_CB,
																			tools_gy302_sda_read SDA_READ_CB,
																			tools_gy302_delay_us DELAY_US_CB)
{
	
	gy302->SCL_CFG_OUT = SCL_CFG_OUT_CB;
	gy302->SDA_CFG_OUT = SDA_CFG_OUT_CB;
	gy302->SDA_CFG_IN  = SDA_CFG_IN_CB;
	gy302->SCL_WRITE	 = SCL_WRITE_CB;
	gy302->SDA_WRITE	 = SDA_WRITE_CB;
	gy302->SDA_READ		 = SDA_READ_CB;
	gy302->DELAY_US		 = DELAY_US_CB;
	
	gy302->SCL_CFG_OUT();
	gy302->SDA_CFG_OUT();
	gy302->SCL_WRITE(1);
	gy302->SDA_WRITE(1);
	
	gy302->DELAY_US(100);
	BH1750_Power_OFF(gy302);
	gy302->DELAY_US(100);
	BH1750_Power_ON(gy302);
	BH1750_RESET(gy302);
	BH1750_WriteByte(gy302, BH1750_CONTINUE_H_MODE2);
}

float tools_bh1750_getLightIntensity(gy302_t *gy302)
{
	gy302->light_intensity = (float)((float)BH1750_Read_Data(gy302) / 1.2f);
//	printf("value:%d\r\n", BH1750_Read_Data(gy302));
	return gy302->light_intensity;
}