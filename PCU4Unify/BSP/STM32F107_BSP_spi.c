#include "STM32F107_BSP_spi.h"
#include "stm32f10x_spi.h"
#include "mcp2515_api.h"
#include "stm32f10x_exti.h"

#define SPI_MCP2515_CS_LOW()        GPIO_ResetBits(GPIOB, GPIO_Pin_12)	//片选引脚/CS拉低
#define SPI_MCP2515_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)  //片选引脚/CS拉高

#define SPI_MCP2515_RESET_LOW()        GPIO_ResetBits(GPIOD, GPIO_Pin_2)	//片选引脚/CS拉低
#define SPI_MCP2515_RESET_HIGH()      GPIO_SetBits(GPIOD, GPIO_Pin_2)  //片选引脚/CS拉高


static void GPIO_Config(void);
//static void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);
//static void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line);

void BSP_SPI_Init()
{

	GPIO_Config();
	SPI_Config(); // I2C configuration 
	EXTI_Configuration();//configure external interupt 
	SPI_MCP2515_RESET_HIGH();
        mcp2515_Reset();	 
	SetDelay(100);//need this delay for the MCP2515 to reset.
	mcp2515_init();
 




  /* depending on the EEPROM Address selected in the i2c_ee.h file */

}

void EXTI_Configuration(void)
{

       //*将EXTI线4连接到PD4
       EXTI_InitTypeDef  EXTI_InitStructure;
       NVIC_InitTypeDef NVIC_InitStructure;
	   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

       GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);

       //*配置EXTI线4上出现下降沿，则产生中断

       EXTI_InitStructure.EXTI_Line = EXTI_Line7;

       EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

       EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;

       EXTI_InitStructure.EXTI_LineCmd = ENABLE;

       EXTI_Init(&EXTI_InitStructure);

       EXTI_GenerateSWInterrupt(EXTI_Line7);
       
       NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
       NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
       NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
       NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
       NVIC_Init(&NVIC_InitStructure);

	          //*将EXTI线13连接到PD13,使能EPO中断
       EXTI_InitTypeDef  EXTI_InitStructure1;
       NVIC_InitTypeDef NVIC_InitStructure1;
       GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource13);
       //*配置EXTI线13上出现下降沿，则产生中断
       EXTI_InitStructure1.EXTI_Line = EXTI_Line13;

       EXTI_InitStructure1.EXTI_Mode = EXTI_Mode_Interrupt;

       EXTI_InitStructure1.EXTI_Trigger = EXTI_Trigger_Falling;

       EXTI_InitStructure1.EXTI_LineCmd = ENABLE;

       EXTI_Init(&EXTI_InitStructure1);

       //EXTI_GenerateSWInterrupt(EXTI_Line13);
       
       NVIC_InitStructure1.NVIC_IRQChannel = EXTI15_10_IRQn;
       NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0x02;
       NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0x01;
       NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
       NVIC_Init(&NVIC_InitStructure1);

}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  

  RCC_APB1PeriphClockCmd(sCAN_SPI_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);

 
 /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  
    //*config CS PIN    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  SPI_MCP2515_CS_LOW();
  
    //*config INT PIN    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB,&GPIO_InitStructure);


  //config MCP2515  Reset pin
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_Out_OD;
  GPIO_Init(GPIOD,&GPIO_InitStructure);
  
  
}





/***************************************************************
 * function name:	SPI_Config;
 * Description: 	config spi;
 * Input Parameter:	NONE;
 * Return:			NONE;
 * Last rev date:	12/25/2007;
 ***************************************************************/


void SPI_Config(void)
{

  SPI_InitTypeDef SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;   // (36MHz  / 128)  >500K
  SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  SPI_Cmd(SPI2, ENABLE);
}




/***************************************************************
 * function name:	spi_read;
 * Description: 	Read data from spi slave device;
 * Input Parameter:	NONE;
 * Return:			NONE;
 * Last rev date:	12/25/2007;


 ***************************************************************/

 char spi_ReadorWrite(SPI_TypeDef* SPIx,char *buf, uint16_t count)
{
	char i, data_r, data_t;
        SPI_MCP2515_CS_LOW();
       

	for (i=0; i<count; i++)
	{
		data_t = *(buf + i);
		//SPIx->DR= data_t;
		 while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
                 {
		   SPI_I2S_SendData(SPIx,data_t);
                 }
		//errcnt = 0;
		 while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)== RESET);
                 {
		  data_r =SPI_I2S_ReceiveData(SPIx);
		  *(buf + i) = data_r;
	
                 }
		
	
	}	
        SPI_MCP2515_CS_HIGH();
 return 0;	

}



 u8  SPI_MCP2525_Send(u8 byte)
{
/* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI2, byte);
    SetDelay(1);

    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);


}

 void MCP2525_RecieveCanMsg(CanRxMsg* RxMessage, uint8_t u8Rxn)
 {
 	unsigned char temp[5]={0,0,0,0,0};

	
   //  mcp2515_Read_ID0_DLC(temp);
   if(u8Rxn == 0x01) //Recieve bufff0
   {
	temp[0]=mcp2515_Read(RXB0SIDH);
	temp[1]=mcp2515_Read(RXB0SIDL);
	temp[2]=mcp2515_Read(RXB0EID8);
	temp[3]=mcp2515_Read(RXB0EID0);
	temp[4]=mcp2515_Read(RXB0DLC);
	  mcp2515_ReadRX0(RxMessage->Data);   
   }
   if(u8Rxn == 0x02)
   {
   	temp[0]=mcp2515_Read(RXB1SIDH);
	temp[1]=mcp2515_Read(RXB1SIDL);
	temp[2]=mcp2515_Read(RXB1EID8);
	temp[3]=mcp2515_Read(RXB1EID0);
	temp[4]=mcp2515_Read(RXB1DLC);
	  mcp2515_ReadRX1(RxMessage->Data);   

   }

      //mcp2515_ReadRX0(RxMessage->Data);   
	   
	 RxMessage->ExtId = (unsigned long)(temp[0] << 0x15) + (unsigned long)((temp[1] & 0xE0) << 0x0D)
		+ (unsigned long)((temp[1] & 0x03) << 0x10) + (unsigned long)(temp[2] << 0x08)
		+ (unsigned long)(temp[3]);
	 RxMessage->DLC = (unsigned int)(temp[4] & 0x0f);	
	 
  	RxMessage->IDE = (uint8_t)0x04 & (temp[1]>>1);
  
	RxMessage->RTR = (uint8_t)0x02 & (temp[1]>>3);
	/*   
	u8_CanInternalBMSMsgDef.u16PGN= uint16_t(frame_rx->id>>16) & 0x00ff;
	u8_CanInternalBMSMsgDef.u16MACId=uint16_t(frame_rx->id & 0x000000ff);
	u8_CanInternalBMSMsgDef.u16DestId= uint16_t(frame_rx->id>>8) & 0x00ff;
	u8_CanInternalBMSMsgDef.u16DataLen= frame_rx->length;  
	*/
 }

 void mcp2515_tx0_extend(CanTxMsg* TxMessage)
{	
          char temp[5];
	
	//if TXB0 is free
	if ((mcp2515_Read(TXB0CTRL) & TXREQ) != TXREQ)
   	{
		//bit28-21
	        temp[0] = (char)((TxMessage->ExtId) >> 0x15);
			//bit20-16 	//0x08 enable the EXIDE
	        temp[1] = (char)((((TxMessage->ExtId) >> 0x0D) & 0xE0) + 0x08 
				+ (((TxMessage->ExtId) >> 0x10) & 0x03));
			//bit15-8
	        temp[2] = (char)((TxMessage ->ExtId) >> 0x08);
			//bit7-0
	        temp[3] = (char)(TxMessage ->ExtId);
		temp[4] = RTR_RESET | (char)(TxMessage->DLC);

		mcp2515_Write_ID0_DLC(temp);
			
	   	/* Send data to MCP2515 */   	
	   	mcp2515_WriteTX0(TxMessage->Data);
	         
	    	/* Initiate transmission on TxB0 */
	   	mcp2515_RTS(RTS0);     
		
	}
}





