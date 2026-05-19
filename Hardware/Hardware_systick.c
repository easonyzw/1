#include "Hardware_systick.h"




void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   
}	

void delay_ms(unsigned short nms)
{	
	unsigned int temp;	
    
	SysTick->LOAD=(unsigned int)(nms*48/8*1000);
  SysTick->VAL=0x00;  
  SysTick->CTRL=0x01;  
  
  do
	{
		temp=SysTick->CTRL;
	}
	while(!((temp)>>4)&0x00000001);
	SysTick->CTRL=0x00;
	SysTick->VAL =0X00; 
	     	    
} 


void delay_us(unsigned int nus)
{		
	unsigned int temp;	
    
	SysTick->LOAD=(unsigned int)(nus*48/8);
  SysTick->VAL=0x00;  
  SysTick->CTRL=0x01;  
  
  do
	{
		temp=SysTick->CTRL;
	}
	while(!((temp)>>4)&0x00000001);
	SysTick->CTRL=0x00;
	SysTick->VAL =0X00;      
}

