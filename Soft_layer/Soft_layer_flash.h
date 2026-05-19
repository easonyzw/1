#ifndef __SOFT_LAYER_FLASH_H
#define __SOFT_LAYER_FLASH_H

#include "stm32f0xx_flash.h"

void FLASH_WriteByte(unsigned int addr , unsigned char *p , unsigned short int Byte_Num);
void FLASH_ReadByte(unsigned int addr , unsigned char *p , unsigned short int Byte_Num);

#endif


