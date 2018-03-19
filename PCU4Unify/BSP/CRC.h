#ifndef _CRC_H
	#define	_CRC_H

#ifdef __cplusplus
 extern "C" {
#endif

	
#include  "stm32f10x.h"



unsigned int table_crc(unsigned char *ptr,unsigned char len);
   
unsigned short CRC_GetModbus16(unsigned char *pdata, int len);

unsigned short CRC_Get16(int Addr,int len);

#ifdef __cplusplus
}
#endif


 #endif		//END CRC_H
