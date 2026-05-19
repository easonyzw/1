#include "Hardware.h"

/******************************************************************
 * @brief  hardware init
 * @input 
 * @return 
******************************************************************/
void hardware_init(void)
{
	
  Rcc_Init();
	NVIC_initializes();
  delay_init();
  Timer_initializes();
	GPIO_initializes();
  EXTI8_Config();
	RS485_initializes();
//  FLASH_OB_RDPConfig(OB_RDP_Level_2);
//  FLASH_ReadOutProtection(DISABLE);
}

