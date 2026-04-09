/**
  ******************************************************************************
  * @file    Templates/Inc/main.h 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    14-April-2017
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#define DEBUG_ENABLE  1
//#define DEBUG_ENABLE  0

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_nucleo.h"
#include "stm32f1xx.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
//middleware
#include "u8g2.h"
//tools
//#include "tools_matrix_key.h"
#include "tools_oled.h"
//app
#include "drv_app.h"
#include "drv_timers.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define ASR_TX_Pin GPIO_PIN_2
#define ASR_TX_GPIO_Port GPIOA
#define ASR_RX_Pin GPIO_PIN_3
#define ASR_RX_GPIO_Port GPIOA
#define MQ7_ADC_Pin GPIO_PIN_0
#define MQ7_ADC_GPIO_Port GPIOB
#define DEBUG_TX_Pin GPIO_PIN_10
#define DEBUG_TX_GPIO_Port GPIOB
#define DEBUG_RX_Pin GPIO_PIN_11
#define DEBUG_RX_GPIO_Port GPIOB
#define GY302_SDA_Pin GPIO_PIN_13
#define GY302_SDA_GPIO_Port GPIOB
#define GY302_SCL_Pin GPIO_PIN_14
#define GY302_SCL_GPIO_Port GPIOB
#define DHT11_Pin GPIO_PIN_15
#define DHT11_GPIO_Port GPIOB
#define WARN_BEEP_Pin GPIO_PIN_8
#define WARN_BEEP_GPIO_Port GPIOA
#define OPEN_LIGHT_Pin GPIO_PIN_9
#define OPEN_LIGHT_GPIO_Port GPIOA
#define OPEN_FAN_Pin GPIO_PIN_10
#define OPEN_FAN_GPIO_Port GPIOA
#define OPEN_HUMI_Pin GPIO_PIN_11
#define OPEN_HUMI_GPIO_Port GPIOA
#define WARN_LIGHT_Pin GPIO_PIN_12
#define WARN_LIGHT_GPIO_Port GPIOA
#define WIFI_TX_Pin GPIO_PIN_6
#define WIFI_TX_GPIO_Port GPIOB
#define WIFI_RX_Pin GPIO_PIN_7
#define WIFI_RX_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_8
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_9
#define OLED_SDA_GPIO_Port GPIOB




extern uint8_t uart2_rx_buffer[256];
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern RTC_HandleTypeDef hrtc;
extern void MX_I2C1_Init(void);
extern void HAL_Delay_us(uint16_t us);
extern void printArray(char *data, uint32_t len);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void Error_Handler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
