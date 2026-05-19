#ifndef  __SOFT_LAYER_IO_H
#define  __SOFT_LAYER_IO_H
#include "Hardware.h"


/*Function**************************/
void digital_input_initialization(void);
void IO_state_update(void);
void PORT_Address_return(void);
void module_show_status(void);
void LED_all_open(void);
void LED_all_close(void);
void module_show_status_running(void);


typedef struct
{ 
  unsigned short filter;
}sIO_parameter;

extern sIO_parameter IO_parameter[16];

#endif 




