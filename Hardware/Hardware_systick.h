#ifndef _HARDWARE_SYSTICK_H
#define _HARDWARE_SYSTICK_H

#include "stm32f0xx.h"
#include "Hardware.h"

void delay_init(void);
void delay_ms(unsigned short nms);
void delay_us(unsigned int nus);

#endif 


