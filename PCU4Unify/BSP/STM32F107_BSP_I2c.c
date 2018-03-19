
#include "STM32F107_BSP_io.h"


/*
* 函数名：BSP_GPIO_Init
* 描述	：初始化
* 输入	：void
* 输出	: void
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_i2c.h"
#include "STM32F107_BSP_I2c.h"
#include "STM32F107_BSP_timer.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_Speed              200000
#define I2C1_SLAVE_ADDRESS7    0xA0
#define I2C_PageSize           128

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void GPIO_Config(void);
static void I2C_Config(void);
static void EEWP_DISABLE(void);
static void EEWP_DISABLE(void);
static void I2C_EepromWaitEepromStandbyState(u8 chipsel);

//static u16 HexToBCD(u16 Input);
//static u16 BCDToHex(u16 Input);
static void vStartRTC(void);
static u8 BSP_I2C_RTC_Read (u8 address);
static void BSP_I2C_RTC_Write (u8 address, u8 data);


 _CALENDAR  Calendar;

/*******************************************************************************
* Function Name  : void Delay_us(UINT32 delay_count)
* Description    : delay--ms
* Input          : UINT32 delay_count 6000=1mS @48MHz
* Output         : None
* Return         : None
*******************************************************************************/

void Delay_10us(u16 delay_count)
{
	volatile	u16 i,j;
	for (i=0; i< delay_count; i++)
	{
		for (j=0; j<100; j++);
	}
}

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure the used I/O ports pin
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);


  
  /* Configure I2C2 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //WP
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  EEWP_DISABLE();			//写保护
}

/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : I2C Configuration
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Config(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C2, &I2C_InitStructure);

  /* I2C Peripheral Enable */
  I2C_Cmd(I2C2, ENABLE);
    
  //printf("\n\r I2C_Configuration----\n\r");
    
}

/*******************************************************************************
* Function Name  : I2C_EE_Init
* Description    : Initializes peripherals used by the I2C EEPROM driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_I2C_EepromInit()
{
  // GPIO configuration 
  GPIO_Config();

  // I2C configuration 
  I2C_Config();

  /* depending on the EEPROM Address selected in the i2c_ee.h file */

}

/**-------------------------------------------------------
  * 函数名 void EEWP_EN(void)
  * 功能   EEPROM写保护打开
  * 参数   无
  * 返回值 无
***------------------------------------------------------*/
void EEWP_EN(void)
{  
   GPIOE->BRR |= 1<<15; 
}

/**-------------------------------------------------------
  * 函数名 void EEWP_DISABLE(void)
  * 功能   EEPROM写保护打开
  * 参数   无
  * 返回值 无
***------------------------------------------------------*/
void EEWP_DISABLE(void)
{
  GPIOE->BSRR |= 1<<15;  
}



/*******************************************************************************
* Function Name  : I2C_EE_BufferWrite
* Description    : Writes buffer of data to the I2C EEPROM.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_I2C_EepromBufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite,u8 chipsel)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;


  
  Addr = WriteAddr % I2C_PageSize;  
  count = I2C_PageSize - Addr;		
  NumOfPage =  NumByteToWrite / I2C_PageSize;	
  NumOfSingle = NumByteToWrite % I2C_PageSize;	
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      BSP_I2C_EepromPageWrite(pBuffer, WriteAddr, NumOfSingle,chipsel);
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)  //一次写128个数据
      {
        BSP_I2C_EepromPageWrite(pBuffer, WriteAddr, I2C_PageSize,chipsel); 
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)	//除128个以外的
      {
        BSP_I2C_EepromPageWrite(pBuffer, WriteAddr, NumOfSingle,chipsel);
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      BSP_I2C_EepromPageWrite(pBuffer, WriteAddr, NumOfSingle,chipsel);
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        BSP_I2C_EepromPageWrite(pBuffer, WriteAddr, count,chipsel);
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        BSP_I2C_EepromPageWrite(pBuffer, WriteAddr, I2C_PageSize,chipsel);
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        BSP_I2C_EepromPageWrite(pBuffer, WriteAddr, NumOfSingle,chipsel); 
      }
    }
  }  
  

}

/*******************************************************************************
* Function Name  : I2C_EE_ByteWrite
* Description    : Writes one byte to the I2C EEPROM.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_I2C_EepromByteWrite(u8* pBuffer, u16 WriteAddr,u8 chipsel)
{
	u16 EEPROM_ADDRESS,TimeCnt;

	if(chipsel == 0)
		{
		EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
		}
	else 
		{
		EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
		}

  /* Send STRAT condition */
  I2C_GenerateSTART(I2C2, ENABLE);

  /* Test on EV5 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C2, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++; 
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C2, (u8)(WriteAddr>>8));
  
  /* Test on EV8 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;

    I2C_SendData(I2C2, (u8)(WriteAddr));
  
  /* Test on EV8 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;
  

  /* Send the byte to be written */
  I2C_SendData(I2C2, *pBuffer); 
   
  /* Test on EV8 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;

  EEWP_EN();
  Delay_10us(1);
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C2, ENABLE);
  
  Delay_10us(1);
  EEWP_DISABLE();
 // I2C_EepromWaitEepromStandbyState(0);
}

/*******************************************************************************
* Function Name  : I2C_EE_PageWrite
* Description    : Writes more than one byte to the EEPROM with a single WRITE
*                  cycle. The number of byte can't exceed the EEPROM page size.
* Input          : - pBuffer : pointer to the buffer containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_I2C_EepromPageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite,u8 chipsel)
{
	u16 EEPROM_ADDRESS,TimeCnt;

	if(chipsel == 0)
		{
		EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;		//is the address right?
		}
	else 
		{
		EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
		}
   TimeCnt = 0;
   while((I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++; // Added by Najoua 27/08/2008
    
  /* 发送启动条件 */
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;
  
  /* Send EEPROM address for write */
 
  I2C_Send7bitAddress(I2C2, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;

  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(I2C2, (u8)(WriteAddr>>8));  

  /* Test on EV8 and clear it */
  TimeCnt = 0;
  while((! I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;
  
    I2C_SendData(I2C2, (u8)(WriteAddr));  

  /* Test on EV8 and clear it */
  TimeCnt = 0;
  while((! I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C2, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
	TimeCnt = 0;
    while ((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;
  }
  EEWP_EN();
  Delay_10us(1);

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C2, ENABLE);
  Delay_10us(1);
  EEWP_DISABLE();
//  I2C_EepromWaitEepromStandbyState(0);
}

/*******************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description    : Reads a block of data from the EEPROM.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the EEPROM.
*                  - ReadAddr : EEPROM's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
void BSP_I2C_EepromBufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead,u8 chipsel)
{  
	u16 EEPROM_ADDRESS,TimeCnt;
	
	if(chipsel == 0)
		{
		EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
		}
	else 
		{
		EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
		}

  //*((u8 *)0x4001080c) |=0x80; 
    TimeCnt = 0;
    while((I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++; // Added by Najoua 27/08/2008
    
    
  /* Send START condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  /* Test on EV5 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C2, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C2, ENABLE);

  /* Send the EEPROM's internal address to write to */

  I2C_SendData(I2C2, (u8)(ReadAddr>>8));  

  /* Test on EV8 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;
  
    /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C2, (u8)(ReadAddr));  

  /* Test on EV8 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C2, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C2, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C2, ENABLE);
    }
#if 1	
//远龙解决方案
	TimeCnt = 0;
	
	/* Wait for the byte to be received */
	while((I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE) == RESET)&& (TimeCnt < IICWAITTimeOut)) TimeCnt++;

	if(TimeCnt >= IICWAITTimeOut){*pBuffer = 0xFF;return;}
	
	/* Read a byte from the EEPROM */
	*pBuffer = I2C_ReceiveData(I2C2);

	/* Point to the next location where the byte read will be saved */
	pBuffer++; 
		
	/* Decrement the read bytes counter */
	NumByteToRead--;  

	//end of yuan long's solution
#endif

#if 0
  /* Test on EV7 and clear it */
	 if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))  
	 {		
	   /* Read a byte from the EEPROM */
	   *pBuffer = I2C_ReceiveData(I2C2);
  
	   /* Point to the next location where the byte read will be saved */
	   pBuffer++; 
	   
	   /* Decrement the read bytes counter */
	   NumByteToRead--;  
	   
	 } 
#endif

  }
  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C2, ENABLE);

}

/*******************************************************************************
* Function Name  : I2C_EepromWaitEepromStandbyState
* Description    : Wait for EEPROM Standby state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EepromWaitEepromStandbyState(u8 chipsel)      
{
  vu16 SR1_Tmp = 0;
  u16 EEPROM_ADDRESS,TimeCnt;
  if(chipsel == 0)
          {
          EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
          }
  else 
          {
          EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
          }
  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C2, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C2, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C2, EEPROM_ADDRESS, I2C_Direction_Transmitter);
	TimeCnt++;
  }while((!(I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0x0002))&& (TimeCnt < IICWAITTimeOut));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C2, I2C_FLAG_AF);
    /* STOP condition */    
    I2C_GenerateSTOP(I2C2, ENABLE); // Added by Najoua 27/08/2008
}

/*===========================================================================*
 *  函数名称：void BSP_I2C_RTC_Write (u8 address, u8 data)
 *  功能描述：写8563寄存器函数
 *  用法说明：
 *  输入参数：address--寄存器地址  data--寄存器数据
 *  输出参数：
 *  设计人员：
 *  创建日期：
 *==========================================================================*/

void BSP_I2C_RTC_Write (u8 address, u8 data)
{
	 u16 TimeCnt;
	 TimeCnt = 0;
	 while((I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))&& (TimeCnt < IICWAITTimeOut))TimeCnt++; 

	  /* Send STRAT condition */
	  I2C_GenerateSTART(I2C2, ENABLE);
	
	  /* Test on EV5 and clear it */
	  TimeCnt = 0;
	  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;  
	
	  /* Send EEPROM address for write */
	  I2C_Send7bitAddress(I2C2, PCF8563_ADDRESS, I2C_Direction_Transmitter);
	  
	  /* Test on EV6 and clear it */
	  TimeCnt = 0;
	  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;
		  
	  /* Send the EEPROM's internal address to write to */
	  I2C_SendData(I2C2, address);
	  
	  /* Test on EV8 and clear it */
	  TimeCnt = 0;
	  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;
	
	  /* Send the byte to be written */
	  I2C_SendData(I2C2, data); 
	   
	  /* Test on EV8 and clear it */
	  TimeCnt = 0;
	  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;
	  
	  /* Send STOP condition */
	  I2C_GenerateSTOP(I2C2, ENABLE);


}


/*===========================================================================*
 *  函数名称：u8 BSP_I2C_RTC_Read (u8 address)
 *  功能描述：读取8563寄存器内容函数
 *  用法说明：
 *  输入参数：address--寄存器地址
 *  输出参数：
 *  设计人员：
 *  创建日期：
 *==========================================================================*/

u8 BSP_I2C_RTC_Read (u8 address)
{
	u8 ReceData;
	u16 TimeCnt = 0;
     //*((u8 *)0x4001080c) |=0x80; 
    while((I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))&& (TimeCnt < IICWAITTimeOut))TimeCnt++; // Added by Najoua 27/08/2008
    
    
  /* Send START condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  /* Test on EV5 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C2, PCF8563_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C2, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C2, address);  

  /* Test on EV8 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C2, PCF8563_ADDRESS, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;

  TimeCnt = 0;
  while((!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))&& (TimeCnt < IICWAITTimeOut))TimeCnt++;

  ReceData = I2C_ReceiveData(I2C2);

  I2C_AcknowledgeConfig(I2C2, DISABLE);

  /* Send STOP Condition */
  I2C_GenerateSTOP(I2C2, ENABLE);

  return ReceData;
 

}



/*===========================================================================*
 *  函数名称：void vStartRTC(void)
 *  功能描述：启动RTC，设置RTC输出1024Hz信号
 *  用法说明：
 *  输入参数：
 *  输出参数：
 *  设计人员：
 *  创建日期：
 *==========================================================================*/
void vStartRTC(void)
{
	BSP_I2C_RTC_Write(CLKOUTCTLREG, 0x81);						// 输出1024Hz信号
	BSP_I2C_RTC_Write(CTLSTATUS1REG, 0);						// 启动时钟
}

/*===========================================================================*
 *  函数名称：Uint16 BCDToHex(Uint16 Input)
 *  功能描述：将BCD码转换为16进制数
 *  用法说明：
 *  输入参数：
 *  输出参数：
 *  设计人员：
 *  创建日期：
 *==========================================================================*/
u16 BCDToHex(u16  Input)
{
	return (Input & 0x0F) + (Input >> 4) * 10;
}

/*===========================================================================*
 *  函数名称：u16 HexToBCD(u16 Input)
 *  功能描述：将16进制数转换为BCD码
 *  用法说明：
 *  输入参数：
 *  输出参数：
 *  设计人员：
 *  创建日期：
 *==========================================================================*/
u16 HexToBCD(u16 Input)
{
	Input &= 0xFF;
	return (Input % 10) + (Input /10) * 16;
}
/*===========================================================================*
 函数名称：void ReadRTCTime(void)
 功能描述：从PCF8563读取时间
 输入参数：无
 返回值：无
 使用说明：
 *==========================================================================*/
void ReadRTCTime(void)
{

	u8 Time_Data;

//	if (g_st_TimerFlag.b1s != 1)
//	{
//		return;
//	}
	
	Time_Data = BSP_I2C_RTC_Read(SECONDSREG);

  	Calendar.Second = BCDToHex(Time_Data & 0x7f); //秒
  
    Time_Data = BSP_I2C_RTC_Read(MINUTESREG);
	
   	Calendar.Minute= BCDToHex(Time_Data & 0x7f); //分

    Time_Data = BSP_I2C_RTC_Read(HOURSREG);
	
	Calendar.Hour = BCDToHex(Time_Data & 0x3f); //时
	
 	Time_Data = BSP_I2C_RTC_Read(DAYSREG);

  	Calendar.Day = BCDToHex(Time_Data & 0x3f); //日
  
    Time_Data = BSP_I2C_RTC_Read(WEEKDAYSREG);
	
   	Calendar.Week = BCDToHex(Time_Data & 0x07); //星期

    Time_Data = BSP_I2C_RTC_Read(MONTHSREG);	
	
   	Calendar.Month = BCDToHex(Time_Data & 0x1f); //月

    Time_Data = BSP_I2C_RTC_Read(YEARSREG);
	
	Calendar.Year.half.lbyte = BCDToHex(Time_Data & 0xff); //年
  
}


/*===========================================================================*
 函数名称：void BSP_RTC_Init(void)
 功能描述：初始化时钟芯片PCF8563
 输入参数：无
 返回值：无
 使用说明：
 *==========================================================================*/
void BSP_RTC_Init(void)
{	
/*
	Calendar.Second = 0x00;
	Calendar.Minute = 0x36;
	Calendar.Hour = 0x0E;
	Calendar.Day = 0x05;
	Calendar.Month = 0x02;
	Calendar.Year.half.lbyte = 0x12;
	Calendar.Year.half.hbyte = 0x14;

*/
	BSP_I2C_RTC_Write(SECONDSREG, HexToBCD(Calendar.Second));	//秒
	BSP_I2C_RTC_Write(MINUTESREG, HexToBCD(Calendar.Minute));	//分
	BSP_I2C_RTC_Write(HOURSREG, HexToBCD(Calendar.Hour));	//时
	BSP_I2C_RTC_Write(DAYSREG, HexToBCD(Calendar.Day));		//日
	BSP_I2C_RTC_Write(WEEKDAYSREG, HexToBCD(Calendar.Week));//星期
	BSP_I2C_RTC_Write(MONTHSREG, HexToBCD(Calendar.Month));	//月
	BSP_I2C_RTC_Write(YEARSREG, HexToBCD(Calendar.Year.half.lbyte));	//年

	BSP_I2C_RTC_Write(TIMERCTLREG, 0x03);				//定时器未使用，省电模式
	BSP_I2C_RTC_Write(CTLSTATUS2REG, 0x0C);				//告警和定时器标志使能

	vStartRTC();									//时钟开启
}

/***************************************************************************
函数名称:void WriteRTCTime(_CALENDAR m_st_Calendar)
功能描述:将日期和时间写入时钟芯片PCF8563
输入参数：_CALENDAR m_st_Calendar
返回值：无
使用说明：
****************************************************************************/

void WriteRTCTime(_CALENDAR m_st_Calendar)
{
	BSP_I2C_RTC_Write(SECONDSREG, HexToBCD(m_st_Calendar.Second));	//秒
	BSP_I2C_RTC_Write(MINUTESREG, HexToBCD(m_st_Calendar.Minute));	//分
	BSP_I2C_RTC_Write(HOURSREG, HexToBCD(m_st_Calendar.Hour));	//时
	BSP_I2C_RTC_Write(DAYSREG, HexToBCD(m_st_Calendar.Day));		//日
//	BSP_I2C_RTC_Write(WEEKDAYSREG, m_st_Calendar.Week);	//星期
	BSP_I2C_RTC_Write(MONTHSREG, HexToBCD(m_st_Calendar.Month));	//月
	BSP_I2C_RTC_Write(YEARSREG, HexToBCD(m_st_Calendar.Year.half.lbyte ));	//年
 /* 
	BSP_I2C_RTC_Write(TIMERCTLREG, 0x03);					//定时器无效
	BSP_I2C_RTC_Write(CTLSTATUS2REG, 0x0C);

	vStartRTC();									//时钟开启
*/
}

