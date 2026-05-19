#ifndef _HARDWARE_USART_H
#define _HARDWARE_USART_H

#include "stm32f0xx.h"
#include "Hardware.h"


void RS485_initializes(void);
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 CNDTR);
void usart1_send_data(unsigned char* data , unsigned char len);
void DMA_Config_RX(DMA_Channel_TypeDef* DMA_CHx,unsigned int cpar,unsigned int cmar,unsigned short int cndtr);
void DMA_Config_TX(DMA_Channel_TypeDef* DMA_CHx,unsigned int cpar,unsigned int cmar,unsigned short int cndtr);

#endif 


