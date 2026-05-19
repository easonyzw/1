#ifndef _CRC16_H_
#define _CRC16_H_

extern const unsigned short   crc16_tab[256];
extern const unsigned short   crcccitt_tab[256];


unsigned short crc16(unsigned char *data,unsigned int length);
unsigned short crc_ccitt(unsigned char *ptr,unsigned int len);

#endif

