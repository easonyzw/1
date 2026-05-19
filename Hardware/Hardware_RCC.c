#include "Hardware_RCC.h"
/******************************************************************
 * @brief  Rcc_Init
 * @input  none
 * @return  none
******************************************************************/
void Rcc_Init(void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  ErrorStatus       HSEStartUpStatus;

  RCC_ClockSecuritySystemCmd(ENABLE);
                  
  RCC_HSEConfig(RCC_HSE_ON); 
  HSEStartUpStatus = RCC_WaitForHSEStartUp();  

  if(HSEStartUpStatus == SUCCESS)  
  {
    FLASH_PrefetchBufferCmd(ENABLE);          
    RCC_HCLKConfig(RCC_SYSCLK_Div1);                                    
    RCC_PCLKConfig(RCC_HCLK_Div1);
    RCC_PLLConfig(RCC_PLLSource_HSE,RCC_PLLMul_6);   
    RCC_PLLCmd(ENABLE);
                    
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    RCC_GetClocksFreq(&RCC_Clocks);     
    while(RCC_GetSYSCLKSource() != 0x08)                  
    {
    }
  } 
}



