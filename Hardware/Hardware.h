#ifndef _HARDWARE_H
#define _HARDWARE_H

#include "stm32f0xx.h"
#include "string.h"
#include "Hardware_RCC.h"
#include "Hardware_GPIO.h"
#include "Hardware_NVIC.h"
#include "Hardware_usart.h"
#include "Hardware_timer.h"
#include "Hardware_EXTI.h"
#include "Hardware_SYSTICK.h"


#include "Soft_layer.h"
#include "Soft_layer_bus.h"
#include "Soft_layer_data.h"
#include "Soft_layer_flash.h"
#include "soft_layer_rs485.h"
#include "Soft_layer_crc16.h"
#include "soft_layer_IO_input.h"




/*Bit band operation***************************************************************************/
typedef struct _16_Bits_Struct
{
     unsigned short int bit0  : 1;
     unsigned short int bit1  : 1;
     unsigned short int bit2  : 1;
     unsigned short int bit3  : 1;
     unsigned short int bit4  : 1;
     unsigned short int bit5  : 1;
     unsigned short int bit6  : 1;
     unsigned short int bit7  : 1;
     unsigned short int bit8  : 1;
     unsigned short int bit9  : 1;
     unsigned short int bit10 : 1;
     unsigned short int bit11 : 1;
     unsigned short int bit12 : 1;
     unsigned short int bit13 : 1;
     unsigned short int bit14 : 1;
     unsigned short int bit15 : 1;
} Bits_16_TypeDef;

#define PAout(n)   (((Bits_16_TypeDef *)(&(GPIOA->ODR)))->bit##n)  //OUTPUT 
#define PAin(n)    (((Bits_16_TypeDef *)(&(GPIOA->IDR)))->bit##n)  //INPUT
#define PBout(n)   (((Bits_16_TypeDef *)(&(GPIOB->ODR)))->bit##n)  //OUTPUT 
#define PBin(n)    (((Bits_16_TypeDef *)(&(GPIOB->IDR)))->bit##n)  //INPUT 
#define PCout(n)   (((Bits_16_TypeDef *)(&(GPIOC->ODR)))->bit##n)  //OUTPUT 
#define PCin(n)    (((Bits_16_TypeDef *)(&(GPIOC->IDR)))->bit##n)  //INPUT 
#define PDout(n)   (((Bits_16_TypeDef *)(&(GPIOD->ODR)))->bit##n)  //OUTPUT 
#define PDin(n)    (((Bits_16_TypeDef *)(&(GPIOD->IDR)))->bit##n)  //INPUT 
#define PEout(n)   (((Bits_16_TypeDef *)(&(GPIOE->ODR)))->bit##n)  //OUTPUT 
#define PEin(n)    (((Bits_16_TypeDef *)(&(GPIOE->IDR)))->bit##n)  //INPUT 
#define PFout(n)   (((Bits_16_TypeDef *)(&(GPIOF->ODR)))->bit##n)  //OUTPUT 
#define PFin(n)    (((Bits_16_TypeDef *)(&(GPIOF->IDR)))->bit##n)  //INPUT 
#define PGout(n)   (((Bits_16_TypeDef *)(&(GPIOG->ODR)))->bit##n)  //OUTPUT 
#define PGin(n)    (((Bits_16_TypeDef *)(&(GPIOG->IDR)))->bit##n)  //INPUT

/*Macro Definition***************************************************************************/


/* Function******************************************/
void hardware_init(void);
void receive_station_number(void);
void last_station_judgment(void);
void send_nex_station_number(void);

#endif 


