#ifndef __SOFT_LAYER_RS485_H
#define __SOFT_LAYER_RS485_H
#include "Hardware.h"

#define IO_Input_L rs485_send_04_buf[11]
#define IO_Input_H rs485_send_04_buf[12]

extern unsigned char rs485_send_02_buf[13];
extern unsigned char rs485_send_04_buf[15];
extern unsigned char rs485_send_05_buf[7];


void rs485_send_02(void);
void rs485_send_04(unsigned char p[2]);
void rs485_send_05(void); 
void rs485_receive_data_handler(void);



#endif



