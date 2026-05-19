#ifndef __HARDWARE_TIMER_H
#define __HARDWARE_TIMER_H
#include "stm32f0xx.h"
#include "Hardware.h"



/*Function*****************************/
void Timer_initializes(void);
void Timer6_Initializes(void);
void Timer3_Initializes(void);
void timer_delay_us(unsigned short int Times);
void timer_delay_ms(unsigned short int Times);
void timer_delay_s(unsigned short int Times);

#endif 


