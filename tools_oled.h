/*
 * oled.h
 *
 *  Created on: May 19, 2023
 *      Author: muyunqing
 */

#ifndef __OLED_H
#define __OLED_H

#include "stdint.h"
#include "main.h"

#include "u8g2.h"
#include "drv_app.h"

#define OLED_ADDRESS  0x78  // oled
#define OLED_CMD   		0x00  //
#define OLED_DATA  		0x40  //


#define USE_SOFTWARE 1

//需实现回调函数
typedef void (*tools_oled_scl_cfg_out_cb)(void);
typedef void (*tools_oled_sda_cfg_out_cb)(void);
typedef void (*tools_oled_sda_cfg_in_cb)(void);
typedef void (*tools_oled_scl_write)(uint8_t);
typedef void (*tools_oled_sda_write)(uint8_t);
typedef uint8_t (*tools_oled_sda_read)(void);
typedef void (*tools_oled_delay_us)(uint16_t);

typedef struct _oled_strutc
{
	tools_oled_scl_cfg_out_cb 	SCL_CFG_OUT;
	tools_oled_sda_cfg_out_cb 	SDA_CFG_OUT;
	tools_oled_sda_cfg_in_cb 	SDA_CFG_IN;
	tools_oled_scl_write				SCL_WRITE;
	tools_oled_sda_write				SDA_WRITE;
	tools_oled_sda_read				SDA_READ;
	tools_oled_delay_us				DELAY_US;
}oled_t;


//extern void draw(u8g2_t *u8g2);
extern void testShowFont(u8g2_t *u8g2, char *temp_msb, char *humi_msb, char *temp_lsb, char *humi_lsb);
extern uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
extern void u8g2Init(u8g2_t *u8g2);

#endif
