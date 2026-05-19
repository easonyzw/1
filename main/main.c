#include "Hardware.h"



int main(void)
{
  
	hardware_init();
	software_init();

  
  while(1)
  {  
    IO_state_update();
  }
}



