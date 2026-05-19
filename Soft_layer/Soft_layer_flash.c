#include "Soft_layer_flash.h"

/******************************************************************
 * @brief  Write to local flash
 * @input  addr      : Write local flash start address
           *p        : Write local flash string
					 Byte_Num  : String length
 * @return  none
******************************************************************/
void FLASH_WriteByte(unsigned int addr , unsigned char *p , unsigned short int Byte_Num)
{
	 unsigned int HalfWord;
	
	 Byte_Num = Byte_Num/2;
	
	 FLASH_Unlock();
	
	 FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	
	 FLASH_ErasePage(addr);
	
	 while(Byte_Num --)
	 {
		 HalfWord=*(p++);
		 HalfWord|=*(p++)<<8;
		 FLASH_ProgramHalfWord(addr, HalfWord);
		 addr += 2;
	 }
	 
	 FLASH_Lock();
}

/******************************************************************
 * @brief  Read to local flash
 * @input  addr      : Read local flash start address
           *p        : Read local flash string
					 Byte_Num  : String length
 * @return  none
******************************************************************/
void FLASH_ReadByte(unsigned int addr , unsigned char *p , unsigned short int Byte_Num)
{
   while(Byte_Num--)
   {
      *(p++)=*((unsigned char*)addr++);
   }
}
