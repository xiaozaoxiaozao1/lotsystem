#ifndef _TOOLS_GY_302_H_
#define _TOOLS_GY_302_H_

#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"

/*	器件地址(7位-用左移1位-最后1位为读写位--1为读-0为写) */
#define	BH1750_Addr_GND_REG		0x23	//ADDR引脚接低电平
#define	BH1750_Addr_VCC_REG		0x5C	//ADDR引脚接高电平

				/******	指令集宏定义*****/
#define				BH1750_Power_OFF_REG			0x00	//断电指令
#define				BH1750_Power_ON_REG				0x01	//通电,等待测量命令
#define				BH1750_MODULE_RESET_REG		0x07	//重置数据寄存器,关机模式下，不能接受重置命令

/*************************************************	
	不同模式下分辨率不同（也即精度不同）
	高分辨率模式2：分辨率是0.5lx
	高分辨率模式：分辨率1lx
	低分辨率模式：分辨率4lx
	不同模式只是精度不一样，对于计算没有区别
***************************************************/
	/**	工作模式指令集	**/
#define				BH1750_CONTINUE_H_MODE			0x10	//连续H分辨率模式
#define				BH1750_CONTINUE_H_MODE2			0x11	//连续H分辨率模式2
#define				BH1750_CONTINUE_L_MODE			0x13	//连续L分辨率模式
#define				BH1750_ONE_TIME_H_MODE			0x20	//一次H分辨率模式
#define				BH1750_ONE_TIME_H_MODE2			0x21	//一次H分辨率模式2
#define				BH1750_ONE_TIME_L_MODE			0x23	//一次性L分辨率模式

//需实现回调函数
typedef void (*tools_gy302_scl_cfg_out_cb)(void);
typedef void (*tools_gy302_sda_cfg_out_cb)(void);
typedef void (*tools_gy302_sda_cfg_in_cb)(void);
typedef void (*tools_gy302_scl_write)(uint8_t);
typedef void (*tools_gy302_sda_write)(uint8_t);
typedef uint8_t (*tools_gy302_sda_read)(void);
typedef void (*tools_gy302_delay_us)(uint16_t);


typedef struct _gy302_strutc
{
	float light_intensity;
	tools_gy302_scl_cfg_out_cb 	SCL_CFG_OUT;
	tools_gy302_sda_cfg_out_cb 	SDA_CFG_OUT;
	tools_gy302_sda_cfg_in_cb 	SDA_CFG_IN;
	tools_gy302_scl_write				SCL_WRITE;
	tools_gy302_sda_write				SDA_WRITE;
	tools_gy302_sda_read				SDA_READ;
	tools_gy302_delay_us				DELAY_US;
}gy302_t;


extern void tools_gy302_init(gy302_t *gy302, tools_gy302_scl_cfg_out_cb SCL_CFG_OUT_CB,
																			tools_gy302_sda_cfg_out_cb SDA_CFG_OUT_CB,
																			tools_gy302_sda_cfg_in_cb SDA_CFG_IN_CB,
																			tools_gy302_scl_write SCL_WRITE_CB,
																			tools_gy302_sda_write SDA_WRITE_CB,
																			tools_gy302_sda_read SDA_READ_CB,
																			tools_gy302_delay_us DELAY_US_CB);

extern float tools_bh1750_getLightIntensity(gy302_t *gy302);
#endif
