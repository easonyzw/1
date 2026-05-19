#ifndef _Hardware_GPIO_H
#define _Hardware_GPIO_H

#include "Hardware.h"
#include "stm32f0xx.h"

/***********************************   Macro Definition   ********************************/  
/* LED GPIO Pin */
#define LED0_OFF  GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define LED0_ON   GPIO_ResetBits(GPIOC,GPIO_Pin_2)

#define LED1_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_0)
#define LED1_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_0)

#define LED2_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define LED2_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_2)

#define LED3_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define LED3_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_4)

#define LED4_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define LED4_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_6)

#define LED5_OFF  GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define LED5_ON   GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define LED6_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define LED6_ON   GPIO_ResetBits(GPIOB,GPIO_Pin_1)

#define LED7_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define LED7_ON   GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define LED8_OFF  GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define LED8_ON   GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define LED9_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define LED9_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_1)

#define LED10_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define LED10_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_3)

#define LED11_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define LED11_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define LED12_OFF  GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define LED12_ON   GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define LED13_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define LED13_ON   GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define LED14_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_2)
#define LED14_ON   GPIO_ResetBits(GPIOB,GPIO_Pin_2)

#define LED15_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define LED15_ON   GPIO_ResetBits(GPIOB,GPIO_Pin_11)

#define CH15  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define CH14  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define CH13  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define CH12  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)
#define CH11  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)
#define CH10  GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)
#define CH9  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)
#define CH8  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)

#define CH7  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define CH6  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define CH5  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)
#define CH4  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define CH3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)
#define CH2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
#define CH1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define CH0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)

/* BUS GPIO Pin*/
#define BUS_input   PBin(8)
#define BUS_output  PBout(9)

/***********************************    Function    **************************************/
/* GPIO initializes */
void GPIO_initializes(void);
/* LED */
void GPIO_LED_Configuration(void);
/* Input pin */
void GPIO_input_Configuration(void);
/* BUS */
void GPIO_BUS_Configuration(void);
/* USART */
void GPIO_usart1_Configuration(void);



#endif

