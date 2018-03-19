#include "flash.h"


uint16_t STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节


#if STM32_FLASH_WREN	//如果使能了写   

/**
  * @brief  不检查的写入
  * @param  WriteAddr:起始地址
	*					pBuffer:数据指针
	*					NumToWrite:半字(16位)数 
  * @retval None
  */

void STMFLASH_Write_NoCheck(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;
	for(i=0;i<NumToWrite;)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
		WriteAddr+=2;//地址增加2.
	}  
} 
/**
  * @brief  检查的写入
  * @param  WriteAddr:起始地址
	*					pBuffer:数据指针
	*					NumToWrite:半字(16位)数 
  * @retval None
  */
int STMFLASH_Write_Check(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)   
{ 			 		 
	uint16_t i;
	for(i=0;i<NumToWrite;)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
		if(pBuffer[i]!=STMFLASH_ReadHalfWord(WriteAddr))
		{
			return 0;
		}
		else
		{
			i++;
			WriteAddr+=2;
		}
	} 
	return 1;	
}

/**
  * @brief  写入
  * @param  WriteAddr:起始地址
	*					pBuffer:数据指针
	*					NumToWrite:半字(16位)数 
  * @retval None
  */
int STMFLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite)	
{
	uint32_t secaddr;	   //扇区地址
	uint16_t secoff;	   //扇区内偏移地址(16位字计算)
	uint16_t secremain; //扇区内剩余地址(16位字计算)	   
 	uint16_t i;    
	uint32_t offaddr;   //去掉0X08000000后的地址
	
	if(WriteAddr<FLASH_BASE||(WriteAddr>=(FLASH_BASE+1024*STM32_FLASH_SIZE)))
		return 0;//非法地址
	
	INTX_DISABLE(); 
	FLASH_Unlock();						//解锁
	offaddr=WriteAddr-FLASH_BASE;		//实际偏移地址.
	secaddr=offaddr/STM_SECTOR_SIZE;			//扇区地址  
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
	
	if(NumToWrite<=secremain)
		secremain=NumToWrite;//不大于该扇区范围
	while(1) 
	{	
		STMFLASH_Read(secaddr*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			FLASH_ErasePage(secaddr*STM_SECTOR_SIZE+FLASH_BASE);//擦除这个扇区
			for(i=0;i<secremain;i++)//复制
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			if(STMFLASH_Write_Check(secaddr*STM_SECTOR_SIZE+FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2) ==0)//写入整个扇区  
			{
				FLASH_Lock();//上锁
				INTX_ENABLE();
				return 0;
			}
		}
		else 
			if(STMFLASH_Write_Check(WriteAddr,pBuffer,secremain) == 0)//写已经擦除了的,直接写入扇区剩余区间. 				
			{
				FLASH_Lock();//上锁
				INTX_ENABLE();
				return 0;
			}
		if(NumToWrite==secremain)
			break;//写入结束了
		else//写入未结束
		{
			secaddr++;				//扇区地址增1
			secoff=0;				//偏移位置为0 	 
		 	pBuffer+=secremain;  	//指针偏移
			WriteAddr+=secremain*2;	//写地址偏移	   
		  NumToWrite-=secremain;	//字节(16位)数递减
			if(NumToWrite>(STM_SECTOR_SIZE/2))
				secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
			else secremain=NumToWrite;//下一个扇区可以写完了
		}	 
	};	
	FLASH_Lock();//上锁
	INTX_ENABLE();
	return 1;
}
#endif

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
uint16_t STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}


//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	uint16_t i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(uint32_t WriteAddr,uint16_t WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}


void INTX_DISABLE(void) 
{ 
	__set_PRIMASK(1);
    
} 

void INTX_ENABLE(void) 
{ 
	__set_PRIMASK(0);   
}
