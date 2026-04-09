#include "tools_oled.h"

extern void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c);
extern u8g2_t global_u8g2;
#define SEND_BUFFER_DISPLAY_MS(u8g2, ms)\
  do {\
    u8g2_SendBuffer(u8g2); \
    HAL_Delay(ms);\
  }while(0);
//U8g2的初始化，需要调用下面这个u8g2_Setup_ssd1306_128x64_noname_f函数，该函数的4个参数含义：
//u8g2：传入的U8g2结构体
//U8G2_R0：默认使用U8G2_R0即可（用于配置屏幕是否要旋转）
//u8x8_byte_sw_i2c：使用软件IIC驱动，该函数由U8g2源码提供
//u8x8_gpio_and_delay：就是上面我们写的配置函数
void u8g2Init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_gpio_and_delay); // 初始化u8g2 结构体
	u8g2_InitDisplay(u8g2); 
	u8g2_SetPowerSave(u8g2, 0);
	u8g2_ClearBuffer(u8g2);
}

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buffer[128];
    static uint8_t buf_idx;
    uint8_t *data;

    switch (msg)
    {
    case U8X8_MSG_BYTE_INIT:
    {
        /* add your custom code to init i2c subsystem */
//			printf("enter u8g2 U8X8_MSG_BYTE_INIT\n");
        MX_I2C1_Init(); //I2C初始化
    }
    break;

    case U8X8_MSG_BYTE_START_TRANSFER:
    {
        buf_idx = 0;
    }
    break;

    case U8X8_MSG_BYTE_SEND:
    {
        data = (uint8_t *)arg_ptr;

        while (arg_int > 0)
        {
            buffer[buf_idx++] = *data;
            data++;
            arg_int--;
        }
    }
    break;

    case U8X8_MSG_BYTE_END_TRANSFER:
    {
			HAL_StatusTypeDef i2c_state;
//        if (HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, buffer, buf_idx, 1000) != HAL_OK)
//				{
//					printf("i2c trans error\n");
//					return 0;
//				}
			i2c_state = HAL_I2C_Master_Transmit(&hi2c1, OLED_ADDRESS, buffer, buf_idx, 1000);
			if(i2c_state != HAL_OK)
			{
				printf("i2c_state is :%d" , i2c_state);
				HAL_I2C_MspDeInit(&hi2c1);
				MX_I2C1_Init();
				u8g2Init(&global_u8g2);
				return 0;
			}
    }
    break;

    case U8X8_MSG_BYTE_SET_DC:
        break;

    default:
        return 0;
    }

    return 1;
}


uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        for (uint16_t n = 0; n < 320; n++)
        {
            __NOP();
        }
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        HAL_Delay(1);
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        //Tims_delay_us(5);
				
        break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
        break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
        break;                    // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}

//uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
//{
//    switch (msg)
//    {
//    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
//        __NOP();
//        break;
//    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
//        for (uint16_t n = 0; n < 320; n++)
//        {
//            __NOP();
//        }
//        break;
//    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
//        HAL_Delay(1);
//        break;
//    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
//        //Tims_delay_us(5);
//				HAL_Delay_us(5);
//        break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
//    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
//				if(arg_int == 1)
//				{
//					HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, (GPIO_PinState)1);
//				}
//				else if(arg_int == 0)
//				{
//					HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, (GPIO_PinState)0);
//				}
//        break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
//    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
//			  if(arg_int == 1)
//				{
//					HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, (GPIO_PinState)1);
//				}
//				else if(arg_int == 0)
//				{
//					HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, (GPIO_PinState)0);
//				}
//        break;                    // arg_int=1: Input dir with pullup high for I2C data pin
//    case U8X8_MSG_GPIO_MENU_SELECT:
//        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
//        break;
//    case U8X8_MSG_GPIO_MENU_NEXT:
//        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
//        break;
//    case U8X8_MSG_GPIO_MENU_PREV:
//        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
//        break;
//    case U8X8_MSG_GPIO_MENU_HOME:
//        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
//        break;
//    default:
//        u8x8_SetGPIOResult(u8x8, 1); // default return value
//        break;
//    }
//    return 1;
//}

void testShowFont(u8g2_t *u8g2, char *temp_msb, char *humi_msb, char *temp_lsb, char *humi_lsb)
{
	u8g2_ClearBuffer(u8g2);

	u8g2_SetFont(u8g2,u8g2_font_unifont_t_chinese1);
	u8g2_DrawUTF8(u8g2,0,16,"温度：");
	u8g2_DrawUTF8(u8g2,40,16,temp_msb);
	u8g2_DrawUTF8(u8g2,58,16,".");
	u8g2_DrawUTF8(u8g2,63,16,temp_lsb);
	u8g2_DrawUTF8(u8g2,73,16,"°");

	u8g2_DrawUTF8(u8g2,0,40,"湿度：");
	u8g2_DrawUTF8(u8g2,40,40,humi_msb);
	u8g2_DrawUTF8(u8g2,58,40,".");
	u8g2_DrawUTF8(u8g2,63,40,humi_lsb);
	u8g2_DrawUTF8(u8g2,73,40,"%");

	u8g2_SendBuffer(u8g2);
}

//void draw(u8g2_t *u8g2)
//{
//	u8g2_ClearBuffer(u8g2);

//    u8g2_SetFontMode(u8g2, 1); /*字体模式选择*/
//    u8g2_SetFontDirection(u8g2, 0); /*字体方向选择*/
//    u8g2_SetFont(u8g2, u8g2_font_inb24_mf); /*字库选择*/
//    u8g2_DrawStr(u8g2, 0, 20, "U");

//    u8g2_SetFontDirection(u8g2, 1);
//    u8g2_SetFont(u8g2, u8g2_font_inb30_mn);
//    u8g2_DrawStr(u8g2, 21,8,"8");

//    u8g2_SetFontDirection(u8g2, 0);
//    u8g2_SetFont(u8g2, u8g2_font_inb24_mf);
//    u8g2_DrawStr(u8g2, 51,30,"g");
//    u8g2_DrawStr(u8g2, 67,30,"\xb2");

//    u8g2_DrawHLine(u8g2, 2, 35, 47);
//    u8g2_DrawHLine(u8g2, 3, 36, 47);
//    u8g2_DrawVLine(u8g2, 45, 32, 12);
//    u8g2_DrawVLine(u8g2, 46, 33, 12);

//    u8g2_SetFont(u8g2, u8g2_font_4x6_tr);
//    u8g2_DrawStr(u8g2, 1,54,"github.com/olikraus/u8g2");

//	u8g2_SendBuffer(u8g2);
//	HAL_Delay(1000);
//}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
//void MX_I2C1_Init(void)
//{

//  /* USER CODE BEGIN I2C1_Init 0 */

//  /* USER CODE END I2C1_Init 0 */

//  /* USER CODE BEGIN I2C1_Init 1 */

//  /* USER CODE END I2C1_Init 1 */
//  hi2c1.Instance = I2C1;
//  hi2c1.Init.ClockSpeed = 100000;
//  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
//  hi2c1.Init.OwnAddress1 = 0;
//  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c1.Init.OwnAddress2 = 0;
//  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN I2C1_Init 2 */

//  /* USER CODE END I2C1_Init 2 */

//}

//static void tools_i2c_start(oled_t *oled)
//{
//	//config sda direction out
//	oled->SDA_CFG_OUT();
//	//sda scl output heigh
//	oled->SDA_WRITE(1);
//	oled->DELAY_US(4);
//	oled->SCL_WRITE(1);
//	//sda output low
//	oled->DELAY_US(4);
//	oled->SDA_WRITE(0);
//	//scl output low
//	oled->DELAY_US(10);
//	oled->SCL_WRITE(0);
//	oled->DELAY_US(4);
//}

//static void tools_i2c_stop(oled_t *oled)
//{
//	//config sda direction out
//	oled->SDA_CFG_OUT();
//	//scl output low
//	oled->SCL_WRITE(0);
//	oled->DELAY_US(10);
//	//sda output low
//	oled->SDA_WRITE(0);
//	oled->DELAY_US(10);
//	//scl output heigh
//	oled->SCL_WRITE(1);
//	oled->DELAY_US(10);
//	//sda output heigh
//	oled->SDA_WRITE(1);
//	oled->DELAY_US(10);
//}

//static void tools_i2c_send_ack(oled_t *oled)
//{
//	//config sda direction out
//	oled->SDA_CFG_OUT();
//	//scl output low
//	oled->SCL_WRITE(0);
//	//sda output low
//	oled->SDA_WRITE(0);
//	oled->DELAY_US(5);
//	//scl output heigh
//	oled->SCL_WRITE(1);
//	oled->DELAY_US(5);
//	//scl output low
//	oled->SCL_WRITE(0);
//	oled->DELAY_US(5);
//}

//static void tools_i2c_send_nack(oled_t *oled)
//{
//	//config sda direction out
//	oled->SDA_CFG_OUT();
//	//scl outout low
//	oled->SCL_WRITE(0);
//	//sda output heigh
//	oled->SDA_WRITE(1);
//	oled->DELAY_US(5);
//	//scl output heigh
//	oled->SCL_WRITE(1);
//	oled->DELAY_US(5);
//	//scl outouut low
//	oled->SCL_WRITE(0);
//	oled->DELAY_US(5);
//}

//static void tools_i2c_send_byte(oled_t *oled, uint8_t send_data)
//{
//	uint8_t bit_times;
//	//config sda direction out
//	oled->SDA_CFG_OUT();
//	//scl output low
//	oled->SCL_WRITE(0);
//	oled->DELAY_US(5);
//	//send bit
//	for(bit_times = 0; bit_times < 8; bit_times++)
//	{
//		if((send_data & 0x80) >> 7)
//		{
//			oled->SDA_WRITE(1);
//		}
//		else
//		{
//			oled->SDA_WRITE(0);
//		}
//		
//		send_data <<= 1;
//		oled->DELAY_US(5);
//		
//		//scl output heigh
//		oled->SCL_WRITE(1);
//		oled->DELAY_US(5);
//		//scl output low
//		oled->SCL_WRITE(0);
//		oled->DELAY_US(5);
//	}
//}

//static uint8_t tools_i2c_read_byte(oled_t *oled, uint8_t ack)
//{
//	uint8_t bit_times;
//	uint8_t i2c_buffer;
//	//config sda direction in
//	oled->SDA_CFG_IN();
//	//read
//	for(bit_times = 0; bit_times < 8; bit_times++)
//	{
//		//scl output low
//		oled->SCL_WRITE(0);
//		oled->DELAY_US(5);
//		
//		//scl output heigh
//		oled->SCL_WRITE(1);
//		i2c_buffer <<= 1;
//		if(oled->SDA_READ()) i2c_buffer++;
//		oled->DELAY_US(15);
//	}
//	if(!ack)
//	{
//		tools_i2c_send_nack(oled);
//	}
//	else
//	{
//		tools_i2c_send_ack(oled);
//	}
//	return i2c_buffer;
//}

//static uint8_t tools_i2c_read_ack(oled_t *oled)
//{
//	uint8_t ack = 0;
//	//config sda direction out
//	oled->SDA_CFG_OUT();
//	//sda output heigh
//	oled->SDA_WRITE(1);
//	//config sda in
//	oled->SDA_CFG_IN();
//	oled->DELAY_US(5);
//	//scl output heigh
//	oled->SCL_WRITE(1);
//	oled->DELAY_US(5);
//	
//	while(oled->SDA_READ())
//	{
//		ack++;
//		if(ack > 250)
//		{
//			return 1;
//		}
//	}
//	oled->SCL_WRITE(0);
//	return 0;
//}


