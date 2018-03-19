#ifndef __STMFLASH_H__
#define __STMFLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f10x.h"

//debug  
#define STM32_FLASH_SIZE 256	 		//所选STM32的FLASH容量大小(单位为K)

#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)

#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif	


#define FLASH_BASE            ((uint32_t)0x08000000) /*!< FLASH base address in the alias region */



uint16_t STMFLASH_ReadHalfWord(uint32_t faddr);		  //读出半字  

int STMFLASH_Write(uint32_t WriteAddr,u16 *pBuffer,uint16_t NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   		//从指定地址开始读出指定长度的数据
int STMFLASH_Write_Check(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   ;



//测试写入
void Test_Write(uint32_t WriteAddr,uint16_t WriteData);					
void INTX_DISABLE(void) ;
void INTX_ENABLE(void) ;

#ifdef __cplusplus
}
#endif

#endif



