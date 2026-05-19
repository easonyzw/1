#include "Hardware_GPIO.h"


/******************************************************************
 * @brief   GPIO initializes
 * @input   none
 * @return  none
******************************************************************/
void GPIO_initializes(void)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC | 
	                      RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOF |  RCC_AHBPeriph_DMA1, ENABLE);

  /* LED */
	GPIO_LED_Configuration();
	/* Input pin */
	GPIO_input_Configuration();
  /* BUS port Configuiation*/
  GPIO_BUS_Configuration();

}

/******************************************************************
 * @brief   GPIO_LED_Configuration
 * @input   none
 * @return  none
******************************************************************/
void GPIO_LED_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;                            
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                      
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                 
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);



  GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
  GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_11);
	GPIO_SetBits(GPIOC,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
}
/******************************************************************
 * @brief  IO Port initialization
 * @input  none
 * @return  none
******************************************************************/
void GPIO_input_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /*Input port initialization*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOF, &GPIO_InitStructure);


}
/******************************************************************
 * @brief   BUS output and input
 * @input   none
 * @return  none
******************************************************************/
void GPIO_BUS_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                            
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                      
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                     
  GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_DOWN ;                 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
}




