#include "Hardware_timer.h"

sTimer_counter  timer_counter;

/*****************************************************************
 * @brief   Timer three initiated   
 * @use     Used for station number reception timing
 * @input   none
 * @return  none
******************************************************************/
void Timer_initializes(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM6, ENABLE);	
  
	Timer3_Initializes();
}

/*****************************************************************
 * @brief   Timer three initiated   
 * @use     Used for station number reception timing
 * @input   none
 * @return  none
******************************************************************/
void Timer3_Initializes(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure1;
	
	TIM_TimeBaseStructure1.TIM_RepetitionCounter=0;
	TIM_TimeBaseStructure1.TIM_Period = 1000; 
	TIM_TimeBaseStructure1.TIM_Prescaler =48; 
	TIM_TimeBaseStructure1.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure1.TIM_CounterMode = TIM_CounterMode_Up;  
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure1); 
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );
	TIM_Cmd(TIM3,ENABLE);
}
/******************************************************************
 * @brief  timer3 transaction functions
 * @input  none
 * @return  none
******************************************************************/
void TIM3_IRQHandler(void)  
{  
  unsigned char i=0;
  
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
  {

    if(timer_counter.LED_ms<0xfff0)
    {
      timer_counter.LED_ms++;
    }    
    
    if(timer_counter.bus<0xfff0)
    {
      timer_counter.bus++;
    }
    
    for(i=0;i<16;i++)
    {
      if(timer_counter.IO_filter[i]<0xfff0)
      {
        timer_counter.IO_filter[i]++;
      }
    }

  }
  
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
}

/******************************************************************
 * @brief   Timer six initialize
 * @input   none
 * @return  none
******************************************************************/
void Timer6_Initializes(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Prescaler = 48;       
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;       
    TIM_TimeBaseStructure.TIM_Period = 1000;             
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;            
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM6, ENABLE);                                
}






