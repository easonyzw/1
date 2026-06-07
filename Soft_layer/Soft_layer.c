#include "Soft_layer.h"

#include "Hardware_EXTI.h"

/******************************************************************
 * @brief  software_init
 * @input  none
 * @return  none
******************************************************************/
void software_init(void)
{
	
	
	//	digital_input_initialization()
//就是初始化 16 路输入的滤波阈值。
//当前项目统一设置为 1。
	digital_input_initialization();
	station_number_initialization();
  
  EXTI8_close();
}

