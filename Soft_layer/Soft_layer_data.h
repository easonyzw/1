#ifndef __SOFT_LAYER_DATA_H
#define __SOFT_LAYER_DATA_H

#include "Hardware.h"

/*********Submodule type****************/
#define IO_NP8        0X02
#define IO_I4NP4      0X03
#define AD_VI4        0X04
#define AD_VQ4        0X05
#define AD_VI2Q2      0X06
#define R4(RS485)     0X08
#define R2(RS232)     0X09
#define DA_CQ4        0X0A
#define IO_I16        0X1D
#define IO_NP16       0X0C
#define IO_RE16       0X0D
#define IO_RE8        0X0E
#define IO_I4RE4      0X0F
#define IO_I8         0X1A
/***********Device type*****************/

#define Analog_input_V   0x01
#define Analog_input_C  0x02

#define IO_input       0x03
#define IO_output      0x04

#define Analog_output_V   0x05
#define Analog_output_C  0x06

#define Business       0xFE

/************Trigger event type**********/
#define Unchanged_without_updating     0x00
#define Unchanged_and_updated          0x01
#define bool_change                    0x02
#define Analog_change                  0x03
#define Communication_CRC_error        0x04
#define Noframe_received_communication 0x05
#define Analog_overflow                0x06

#define USART1_SEND_BUFFER_SIZE   30
#define USART1_RECEIVE_BUFFER_SIZE 30

typedef struct
{
	unsigned char  station_address;
  unsigned char  flag_got_pulse;
	unsigned char  receiveBuffer[USART1_RECEIVE_BUFFER_SIZE];
  unsigned char  sendBuffer[USART1_SEND_BUFFER_SIZE];
	unsigned short receive_length;
  
       
}sModbus_management;

extern sModbus_management  modbus_management;

typedef struct
{ 
	unsigned short int   bus;
	unsigned short int   IO_filter[16];
	unsigned       int   LED_ms;
}sTimer_counter;

extern sTimer_counter timer_counter;





#endif



