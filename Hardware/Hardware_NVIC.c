#include "Hardware_NVIC.h"

/******************************************************************
 * @brief  Interrupt priority initialization
 * @input  none
 * @return  none
******************************************************************/
void NVIC_initializes(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  /* usart 1 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;                 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            
	NVIC_Init(&NVIC_InitStructure);
	
  /* Timer 3 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                  
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;                   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                    
	NVIC_Init(&NVIC_InitStructure);

}
