#include "Soft_layer.h"

#include "Hardware_EXTI.h"

/******************************************************************
 * @brief  software_init
 * @input  none
 * @return  none
******************************************************************/
void software_init(void)
{
	digital_input_initialization();

	station_number_initialization();
  
  EXTI8_close();
}

