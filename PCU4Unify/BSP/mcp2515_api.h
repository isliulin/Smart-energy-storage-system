/*********************************************************************************
*MCP2515 CAN controller operation function header file                             					                                                          *
*Version: 1.0.0                                                                                                                 *
*2017/10/14 writed SE R&D                       
*(C) Copyright 2017-2017, SE R&D China Inc.     
*
*   
*********************************************************************************/

/*******************************************************************
 *                   Register Definitions                          *
 *******************************************************************/
#include "STM32F107_BSP_spi.h"
#include "STM32F107_BSP_timer.h"
#define SPI_MCP2515_CS_LOW()        GPIO_ResetBits(GPIOB, GPIO_Pin_12)	//片选引脚/CS拉低
#define SPI_MCP2515_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)  //片选引脚/CS拉高


/* Configuration Registers */
#define CANSTAT         0x0E
#define CANCTRL         0x0F
#define BFPCTRL         0x0C
#define TEC             0x1C
#define REC             0x1D
#define CNF3            0x28
#define CNF2            0x29
#define CNF1            0x2A
#define CANINTE         0x2B
#define CANINTF         0x2C
#define EFLG            0x2D
#define TXRTSCTRL       0x0D

/*  Recieve Filters */
#define RXF0SIDH        0x00
#define RXF0SIDL        0x01
#define RXF0EID8        0x02
#define RXF0EID0        0x03
#define RXF1SIDH        0x04
#define RXF1SIDL        0x05
#define RXF1EID8        0x06
#define RXF1EID0        0x07
#define RXF2SIDH        0x08
#define RXF2SIDL        0x09
#define RXF2EID8        0x0A
#define RXF2EID0        0x0B
#define RXF3SIDH        0x10
#define RXF3SIDL        0x11
#define RXF3EID8        0x12
#define RXF3EID0        0x13
#define RXF4SIDH        0x14
#define RXF4SIDL        0x15
#define RXF4EID8        0x16
#define RXF4EID0        0x17
#define RXF5SIDH        0x18
#define RXF5SIDL        0x19
#define RXF5EID8        0x1A
#define RXF5EID0        0x1B

/* Receive Masks */
#define RXM0SIDH        0x20
#define RXM0SIDL        0x21
#define RXM0EID8        0x22
#define RXM0EID0        0x23
#define RXM1SIDH        0x24
#define RXM1SIDL        0x25
#define RXM1EID8        0x26
#define RXM1EID0        0x27

/* Tx Buffer 0 */
#define TXB0CTRL        0x30
#define TXB0SIDH        0x31
#define TXB0SIDL        0x32
#define TXB0EID8        0x33
#define TXB0EID0        0x34
#define TXB0DLC         0x35
#define TXB0D0          0x36
#define TXB0D1          0x37
#define TXB0D2          0x38
#define TXB0D3          0x39
#define TXB0D4          0x3A
#define TXB0D5          0x3B
#define TXB0D6          0x3C
#define TXB0D7          0x3D
                         
/* Tx Buffer 1 */
#define TXB1CTRL        0x40
#define TXB1SIDH        0x41
#define TXB1SIDL        0x42
#define TXB1EID8        0x43
#define TXB1EID0        0x44
#define TXB1DLC         0x45
#define TXB1D0          0x46
#define TXB1D1          0x47
#define TXB1D2          0x48
#define TXB1D3          0x49
#define TXB1D4          0x4A
#define TXB1D5          0x4B
#define TXB1D6          0x4C
#define TXB1D7          0x4D

/* Tx Buffer 2 */
#define TXB2CTRL        0x50
#define TXB2SIDH        0x51
#define TXB2SIDL        0x52
#define TXB2EID8        0x53
#define TXB2EID0        0x54
#define TXB2DLC         0x55
#define TXB2D0          0x56
#define TXB2D1          0x57
#define TXB2D2          0x58
#define TXB2D3          0x59
#define TXB2D4          0x5A
#define TXB2D5          0x5B
#define TXB2D6          0x5C
#define TXB2D7          0x5D
                         
/* Rx Buffer 0 */
#define RXB0CTRL        0x60
#define RXB0SIDH        0x61
#define RXB0SIDL        0x62
#define RXB0EID8        0x63
#define RXB0EID0        0x64
#define RXB0DLC         0x65
#define RXB0D0          0x66
#define RXB0D1          0x67
#define RXB0D2          0x68
#define RXB0D3          0x69
#define RXB0D4          0x6A
#define RXB0D5          0x6B
#define RXB0D6          0x6C
#define RXB0D7          0x6D
                         
/* Rx Buffer 1 */
#define RXB1CTRL        0x70
#define RXB1SIDH        0x71
#define RXB1SIDL        0x72
#define RXB1EID8        0x73
#define RXB1EID0        0x74
#define RXB1DLC         0x75
#define RXB1D0          0x76
#define RXB1D1          0x77
#define RXB1D2          0x78
#define RXB1D3          0x79
#define RXB1D4          0x7A
#define RXB1D5          0x7B
#define RXB1D6          0x7C
#define RXB1D7          0x7D
                         

/*******************************************************************
 *               Bit register masks                                *
 *******************************************************************/

/* TXBnCTRL */
#define TXREQ           0x08
#define TXP_LOW         0x00
#define TXP_HIGH        0x01

/* RXBnCTRL */
#define RXM             0x60
#define BUKT            0x04

/* CANCTRL */
#define REQOP           0xE0
#define ABAT            0x10
#define CLKEN           0x04
#define CLKPRE          0x03

/* CANSTAT */
#define REQOP           0xE0
#define ICOD            0x0E

/* CANINTE */   
#define RX0IE           0x01
#define RX1IE           0x02
#define TX0IE           0x04
#define TX1IE           0x08
#define TX2IE           0x10
#define ERRIE           0x20
#define WAKIE           0x40
#define MERRE           0x80

/* CANINTF */   
#define RX0IF           0x01
#define RX1IF           0x02
#define TX0IF           0x04
#define TX1IF           0x80
#define TX2IF           0x10
#define ERRIF           0x20
#define WAKIF           0x40
#define MERRF           0x80

/* BFPCTRL */
#define B1BFS           0x20
#define B0BFS           0x10
#define B1BFE           0x08
#define B0BFE           0x04
#define B1BFM           0x02
#define B0BFM           0x01

/* CNF1 Masks */
#define SJW             0xC0
#define BRP             0x3F

/* CNF2 Masks */
#define BTLMODE         0x80
#define SAM             0x40
#define PHSEG1          0x38
#define PRSEG           0x07

/* CNF3 Masks */
#define WAKFIL          0x40
#define PHSEG2          0x07

/* TXRTSCTRL Masks */
#define TXBNRTS			0X07
#define TXB2RTS         0x04
#define TXB1RTS         0x02
#define TXB0RTS         0x01

#define TXREQ		0x08

/*******************************************************************
 *                    Bit Timing Configuration                     *
 *******************************************************************/
 
/* CNF1 */
#define SJW_1TQ         0x00
#define SJW_2TQ         0x40
#define SJW_3TQ         0x80
#define SJW_4TQ         0xC0

/* CNF2 */
#define BTLMODE_CNF3    0x80
#define BTLMODE_PH1_IPT 0x00

#define SMPL_3X         0x40
#define SMPL_1X         0x00

#define PHSEG1_8TQ      0x38
#define PHSEG1_7TQ      0x30
#define PHSEG1_6TQ      0x28
#define PHSEG1_5TQ      0x20
#define PHSEG1_4TQ      0x18
#define PHSEG1_3TQ      0x10
#define PHSEG1_2TQ      0x08
#define PHSEG1_1TQ      0x00

#define PRSEG_8TQ       0x07
#define PRSEG_7TQ       0x06
#define PRSEG_6TQ       0x05
#define PRSEG_5TQ       0x04
#define PRSEG_4TQ       0x03
#define PRSEG_3TQ       0x02
#define PRSEG_2TQ       0x01
#define PRSEG_1TQ       0x00

/* CNF3 */
#define PHSEG2_8TQ      0x07
#define PHSEG2_7TQ      0x06
#define PHSEG2_6TQ      0x05
#define PHSEG2_5TQ      0x04
#define PHSEG2_4TQ      0x03
#define PHSEG2_3TQ      0x02
#define PHSEG2_2TQ      0x01
#define PHSEG2_1TQ      0x00

#define WAKFIL_ENABLED  0x40
#define WAKFIL_DISABLED 0x00    


/*******************************************************************
 *                  Control/Configuration Registers                *
 *******************************************************************/

/* CANINTE */
#define RX0IE_ENABLED   0x01
#define RX0IE_DISABLED  0x00
#define RX1IE_ENABLED   0x02
#define RX1IE_DISABLED  0x00
#define G_RXIE_ENABLED  0x03
#define G_RXIE_DISABLED 0x00

#define TX0IE_ENABLED   0x04
#define TX0IE_DISABLED  0x00
#define TX1IE_ENABLED   0x08
#define TX2IE_DISABLED  0x00
#define TX2IE_ENABLED   0x10
#define TX2IE_DISABLED  0x00
#define G_TXIE_ENABLED  0x1C
#define G_TXIE_DISABLED 0x00

#define ERRIE_ENABLED   0x20
#define ERRIE_DISABLED  0x00
#define WAKIE_ENABLED   0x40
#define WAKIE_DISABLED  0x00
#define MERRE_ENABLED   0x80
#define MERRE_DISABLED  0x00

/* CANINTF */
#define RX0IF_SET       0x01
#define RX0IF_RESET     0x00
#define RX1IF_SET       0x02
#define RX1IF_RESET     0x00
#define TX0IF_SET       0x04
#define TX0IF_RESET     0x00
#define TX1IF_SET       0x08
#define TX2IF_RESET     0x00
#define TX2IF_SET       0x10
#define TX2IF_RESET     0x00
#define ERRIF_SET       0x20
#define ERRIF_RESET     0x00
#define WAKIF_SET       0x40
#define WAKIF_RESET     0x00
#define MERRF_SET       0x80
#define MERRF_RESET     0x00

/* CANCTRL */ 
#define REQOP_CONFIG    0x80
#define REQOP_LISTEN    0x60
#define REQOP_LOOPBACK  0x40
#define REQOP_SLEEP     0x20
#define REQOP_NORMAL    0x00

#define ABORT           0x10

#define CLKOUT_ENABLED  0x04
#define CLKOUT_DISABLED 0x00
#define CLKOUT_PRE_8    0x03
#define CLKOUT_PRE_4    0x02
#define CLKOUT_PRE_2    0x01
#define CLKOUT_PRE_1    0x00

/* CANSTAT */
#define OPMODE_CONFIG   0x80
#define OPMODE_LISTEN   0x60
#define OPMODE_LOOPBACK 0x40
#define OPMODE_SLEEP    0x20
#define OPMODE_NORMAL   0x00


/* RXBnCTRL */
#define RXM_RCV_ALL     0x60
#define RXM_VALID_EXT   0x40
#define RXM_VALID_STD   0x20
#define RXM_VALID_ALL   0x00

#define RXRTR_REMOTE    0x08
#define RXRTR_NO_REMOTE 0x00

#define BUKT_ROLLOVER    0x04
#define BUKT_NO_ROLLOVER 0x00

#define FILHIT0_FLTR_1  0x01
#define FILHIT0_FLTR_0  0x00

#define FILHIT1_FLTR_5  0x05
#define FILHIT1_FLTR_4  0x04
#define FILHIT1_FLTR_3  0x03
#define FILHIT1_FLTR_2  0x02
#define FILHIT1_FLTR_1  0x01
#define FILHIT1_FLTR_0  0x00


/* TXBnCTRL */
#define TXREQ_SEND      0x08
#define TXREQ_ABORT     0x00

#define TXP_HIGHEST     0x03
#define TXP_INTER_HIGH  0x02
#define TXP_INTER_LOW   0x01
#define TXP_LOWEST      0x00
    

/*******************************************************************
 *                  Register Bit Masks                             *
 *******************************************************************/
 
 #define DLC_0          0x00
 #define DLC_1          0x01
 #define DLC_2          0x02
 #define DLC_3          0x03
 #define DLC_4          0x04
 #define DLC_5          0x05
 #define DLC_6          0x06
 #define DLC_7          0x07    
 #define DLC_8          0x08
 

/*******************************************************************
 *                  CAN SPI commands                               *
 *******************************************************************/

#define CAN_RESET       0xC0
#define CAN_READ        0x03
#define CAN_WRITE       0x02
#define CAN_RTS         0x80
#define CAN_RTS_TXB0    0x81
#define CAN_RTS_TXB1    0x82
#define CAN_RTS_TXB2    0x84
#define CAN_RD_STATUS   0xA0
#define CAN_BIT_MODIFY  0x05    
#define CAN_RD_RX0  	0x92
#define CAN_RD_RX1  	0x96
#define CAN_RD_ID0  	0x90
#define CAN_RD_ID1  	0x94
#define CAN_WRITE_TX0   0x41
#define CAN_WRITE_TX1   0x43
#define CAN_WRITE_TX2   0x45
#define CAN_WRITE_ID0_DLC   0x40
#define CAN_WRITE_ID1_DLC   0x42
#define CAN_WRITE_ID2_DLC   0x44


/*******************************************************************
 *                  Miscellaneous                                  *
 *******************************************************************/

#define DUMMY_BYTE      0x00
#define TXB0            0x31
#define TXB1            0x41
#define TXB2            0x51
#define RXB0            0x61
#define RXB1            0x71
#define EXIDE_SET       0x08
#define EXIDE_RESET     0x00
#define RTR_SET			0X40
#define RTR_RESET		0X00

#define RTS0    		0x01	// use with SPI_RTS() function
#define	RTS1    		0x02
#define	RTS2    		0x04

#define RX0IF_STAT		0X01	// use with SPI_ReadStat() function
#define RX1IF_STAT		0x02
#define TXB0REQ_STAT	0x04



/**********************************************************************
  MCP2515 controller API functions
**********************************************************************/
/*
 * MCP2515 driver API:
 * Reset MCP2515
 */
static void mcp2515_Reset()
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
   
	spibuf[0] = CAN_RESET;
   	spi_ReadorWrite(SPI2,spibuf, 1);
   	
	//udelay(10);
}

/*
 * MCP2515 driver API:
 * Read state of MCP2515
 */
static char mcp2515_ReadStat(void)
{     
	 char spibuf[MAX_SPI_BUFF_LENGTH];
	
	spibuf[0] = CAN_RD_STATUS;
	spibuf[1] = 0x00;
   	spi_ReadorWrite(SPI2,spibuf,2);

	//udelay(5);
   	return spibuf[1];
}


/*
 *  MCP2515 driver API:
 *  Read  content of  the register in  MCP2515
 */ 
static char mcp2515_Read(unsigned char address)
{
	 char spibuf[MAX_SPI_BUFF_LENGTH];
	
	spibuf[0] = CAN_READ;  
	spibuf[1] = address;
	spibuf[2] = 0x00;

	spi_ReadorWrite(SPI2,spibuf,3);
	//udelay(5);

	//printk("spibuff: %d,  %d,  %d\n", spibuf[0],spibuf[1],spibuf[2]);
	return spibuf[2];
}


/*
 *  MCP2515 driver API:
 *  Read  content of  the register in  MCP2515
 */ 
static void mcp2515_Read_ID0_DLC(unsigned char *frame_ID)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_RD_ID0;
	
	for (i = 1; i < 6; i++)
	{
		spibuf[i] = 0x00;
	}

	spi_ReadorWrite(SPI2,spibuf,6);
	
	for (i = 1; i < 6; i++)
	{
		frame_ID[i-1] = spibuf[i];
	}
	
	//udelay(5);

	//printk("frame_ID: %d,  %d,  %d\n", frame_ID[0],frame_ID[1],frame_ID[4]);
	return;
}


/*
 *  MCP2515 driver API:
 *  Read  content of  the register in  MCP2515
 */ 
static void mcp2515_Read_ID1_DLC(unsigned char *frame_ID)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_RD_ID1;
	
	for (i = 1; i <= 5; i++)
	{
		spibuf[i] = 0x00;
	}

	spi_ReadorWrite(SPI2,spibuf,6);
	
	for (i = 1; i <= 5; i++)
	{
		frame_ID[i-1] = spibuf[i];
	}
	
	//udelay(5);

	//printk("spibuff: %d,  %d,  %d\n", spibuf[0],spibuf[1],spibuf[2]);
	return;
}


/*
 *  MCP2515 driver API:
 *  Read  content of  the register in  MCP2515
 */ 
static void mcp2515_ReadRX0( char *frame_data)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_RD_RX0;
	
	for (i = 1; i <= 8; i++)
	{
		spibuf[i] = 0x00;
	}

	spi_ReadorWrite(SPI2,spibuf,9);
	
	for (i = 1; i <= 8; i++)
	{
		frame_data[i-1] = spibuf[i];
	}
	
	//udelay(5);

	//printk("frame_data: %d,  %d,  %d\n", frame_data[0],frame_data[1],frame_data[2]);
	return;
}


/*
 *  MCP2515 driver API:
 *  Read  content of  the register in  MCP2515
 */ 
static void mcp2515_ReadRX1(unsigned char *frame_data)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_RD_RX1;
	
	for (i = 1; i <= 8; i++)
	{
		spibuf[i] = 0x00;
	}

	spi_ReadorWrite(SPI2,spibuf,9);
	
	for (i = 1; i <= 8; i++)
	{
		frame_data[i-1] = spibuf[i];
	}
	
	//udelay(5);

	//printk("spibuff: %d,  %d,  %d\n", spibuf[0],spibuf[1],spibuf[2]);
	return;
}


/*
 * MCP2515 driver API:
 * Write data to the register in MCP2515
 */
static void mcp2515_Write(unsigned char address, char spidata)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	
	spibuf[0] = CAN_WRITE;    
	spibuf[1] = address;
	spibuf[2] = spidata;
	
	//printk("mcp2515_Write fun\n");
	spi_ReadorWrite(SPI2,spibuf,3);
	
	//udelay(5);
}


/*
 * MCP2515 driver API:
 * Write data to the register in MCP2515
 */
static void mcp2515_WriteTX0(unsigned char *spidata)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_WRITE_TX0;
	
	for (i = 1; i <= 8; i++)
	{
		spibuf[i] = spidata[i-1];
	}
	
	//printk("mcp2515_Write fun\n");
	spi_ReadorWrite(SPI2,spibuf,9);
	
	//udelay(5);
}


/*
 * MCP2515 driver API:
 * Write data to the register in MCP2515
 */
static void mcp2515_WriteTX1(unsigned char *spidata)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_WRITE_TX1;
	
	for (i = 1; i <= 8; i++)
	{
		spibuf[i] = spidata[i-1];
	}
	
	//printk("mcp2515_Write fun\n");
	spi_ReadorWrite(SPI2,spibuf,9);
	
	//udelay(5);
}


/*
 * MCP2515 driver API:
 * Write data to the register in MCP2515
 */
static void mcp2515_WriteTX2(unsigned char *spidata)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_WRITE_TX2;
	
	for (i = 1; i <= 8; i++)
	{
		spibuf[i] = spidata[i-1];
	}
	
	//printk("mcp2515_Write fun\n");
	spi_ReadorWrite(SPI2,spibuf,9);
	
	//udelay(5);
}


/*
 * MCP2515 driver API:
 * Write data to the register in MCP2515
 */
static void mcp2515_Write_ID0_DLC(unsigned char *spidata)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_WRITE_ID0_DLC;
	
	for (i = 1; i <= 5; i++)
	{
		spibuf[i] = spidata[i-1];
	}
	
	//printk("mcp2515_Write fun\n");
	spi_ReadorWrite(SPI2,spibuf,6);
	
	//udelay(5);
}


/*
 * MCP2515 driver API:
 * Write data to the register in MCP2515
 */
static void mcp2515_Write_ID1_DLC(unsigned char *spidata)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_WRITE_ID1_DLC;
	
	for (i = 1; i <= 5; i++)
	{
		spibuf[i] = spidata[i-1];
	}
	
	//printk("mcp2515_Write fun\n");
	spi_ReadorWrite(SPI2,spibuf,6);
	
	//udelay(5);
}


/*
 * MCP2515 driver API:
 * Write data to the register in MCP2515
 */
static void mcp2515_Write_ID2_DLC(unsigned char *spidata)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	int i;
	
	spibuf[0] = CAN_WRITE_ID2_DLC;
	
	for (i = 1; i <= 5; i++)
	{
		spibuf[i] = spidata[i-1];
	}
	
	//printk("mcp2515_Write fun\n");
	spi_ReadorWrite(SPI2,spibuf,6);
	
	//udelay(5);
}


/*
 * MCP2515 driver API:
 * Write BitModify command to MCP2515
 */
static void mcp2515_BitModify(unsigned char address, unsigned char spimask, unsigned char spidata)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	
	spibuf[0] = CAN_BIT_MODIFY; 
	spibuf[1] = address;
	spibuf[2] = spimask;
	spibuf[3] = spidata;
 	
	spi_ReadorWrite(SPI2,spibuf,4); 
	
	//udelay(5); 
}


/*
 * MCP2515 driver API:
 * Write RTS command to MCP2515
 */
static void mcp2515_RTS(unsigned char TxBuff)
{
	char spibuf[MAX_SPI_BUFF_LENGTH];
	
	spibuf[0] = CAN_RTS + TxBuff;
  
	spi_ReadorWrite(SPI2,spibuf,1);
	
	//udelay(5);
}

/*********************************************************************************
函数名称：	SetDelay()
功能描述：	delay function
执行过程描述:
*******************************************************************************/

static void SetDelay(uint16_t Delaytime)  //ms
{
  u32 count=0;  
  for(count=0;count<11071*Delaytime;count++)
  {
    count++;
  }
}

/*
 * MCP2515 driver API:
 * Initialize CAN controller MCP2515 
 */
static int mcp2515_init()            
{	

	//mcp2515_Reset();
	// SPI_MCP2515_CS_HIGH();
	 
	//SetDelay(100);//need this delay for the MCP2515 to reset.
	
	// SPI_MCP2515_CS_LOW();
	if (mcp2515_Read(CANSTAT)& 0xE0 != (OPMODE_CONFIG) ) 
   	{

		return 1;
	}

	/* Confirm */
    

	if ( (mcp2515_Read(TXRTSCTRL) & TXBNRTS) != 0)
   	{

		return 1;
	}	

	/*  Set RxB0 to receive all messages */
	mcp2515_Write(RXB0CTRL, (RXM_VALID_EXT | BUKT_ROLLOVER));
	/*  Set RxB1 to receive all messages */
	mcp2515_Write(RXB1CTRL, RXM_VALID_EXT);
	
	/* Set TxB0 to lower priority, in order to send common frame */
	mcp2515_Write(TXB0CTRL, TXP_HIGH);
	
	/* Set TxB1/2 to higher priority, in order to send emergecy frame */
	mcp2515_Write(TXB1CTRL, TXP_LOW);
	mcp2515_Write(TXB2CTRL, TXP_LOW);

	// 配置屏蔽寄存器和滤波寄存器
	// 只比较SID7，其他都不比较
	mcp2515_Write(RXM0SIDH, 0x00);
	mcp2515_Write(RXM0SIDL, 0x00);  // 都不比较
	mcp2515_Write(RXM0EID8, 0x00);
	mcp2515_Write(RXM0EID0, 0xff);


	mcp2515_Write(RXM1SIDH, 0x00);
	mcp2515_Write(RXM1SIDL, 0x00);  // 都不比较
	mcp2515_Write(RXM1EID8, 0x00);
	mcp2515_Write(RXM1EID0, 0x00);


	// 滤波寄存器SID7 == 1，其他滤波寄存器不需要设置，因为Mask中的相应位为0
	mcp2515_Write(RXF0SIDH, 0x00);  // 只接收标准帧仲裁域SID7 == 1 的帧 
	mcp2515_Write(RXF0SIDL, 0x0b);
	mcp2515_Write(RXF0EID8,0xd1);
	mcp2515_Write(RXF0EID0,0xfa);

	mcp2515_Write(RXF1SIDH, 0x00);  // 只接收标准帧仲裁域SID7 == 1 的帧 
	mcp2515_Write(RXF1SIDL, 0x08);

	mcp2515_Write(RXF2SIDH, 0x00);  // 只接收标准帧仲裁域SID7 == 1 的帧 
	mcp2515_Write(RXF2SIDL, 0x08);

	mcp2515_Write(RXF3SIDH, 0x00);  // 只接收标准帧仲裁域SID7 == 1 的帧 
	mcp2515_Write(RXF3SIDL, 0x08);

	mcp2515_Write(RXF4SIDH, 0x00);  // 只接收标准帧仲裁域SID7 == 1 的帧 
	mcp2515_Write(RXF4SIDL, 0x08);
		
	mcp2515_Write(RXF5SIDH, 0x00);  // 只接收标准帧仲裁域SID7 == 1 的帧 
	mcp2515_Write(RXF5SIDL, 0x08);
	

	/* Configure CAN Bit Timing BRP = 2; SJW <= 2 * TQ */
	//mcp2515_Write(CNF1, SJW_1TQ | 1);	
	//mcp2515_Write(CNF2, BTLMODE_CNF3 | SMPL_1X | PHSEG1_8TQ | PRSEG_3TQ);
	//mcp2515_Write(CNF3, WAKFIL_DISABLED | PHSEG2_8TQ);	

	/* How to config the bit time. */
	// 时钟频率：Fosc = 16MHz
	// 分频控制器：CNF1.BRP[5:0] = 7
	// 最小时间份额：TQ = 2 * ( BRP + 1 ) / Fosc = 2 * ( 7 + 1 ) / 16M = 1uS
	
	// 同步段：Sync Seg   = 1TQ
	// 传播段：Prop Seg   = ( PRSEG + 1 ) * TQ  = 1 TQ
	// 相位缓冲段：Phase Seg1 = ( PHSEG1 + 1 ) * TQ = 3 TQ
	// 相位缓冲段：Phase Seg2 = ( PHSEG2 + 1 ) * TQ = 3 TQ
	// 同步跳转长度设置为 CNF1.SJW[1:0] = 00, 即 1TQ
	// 总线波特率 NBR = Fbit =  1/(sync seg + Prop seg + PS1 + PS2 ) 
	//                      =  1/(8TQ) = 1/8uS = 125kHz
	//mcp2515_Write(CNF1, 0x04);//0x00:500K;0x01:250K;0x03:125K	
	//mcp2515_Write(CNF2, 0x92);  //Phseg1=2; pheseg2=2;proseg=2  2.5M/10=250kbps
	//mcp2515_Write(CNF3, 0x02);	
	mcp2515_Write(CNF1, 0x04);//0x00:500K;0x01:250K;0x03:125K	
	mcp2515_Write(CNF2, 0x0b);  //Phseg1=2; pheseg2=2;proseg=2  2.5M/10=250kbps
	mcp2515_Write(CNF3, 0x02);	
	/* Enable TX0 and TX1 and TX2, RX0 and RX1 interrupt */
	//mcp2515_Write(CANINTE,TX2IE |TX1IE | TX0IE | RX0IE | RX1IE);
	//mcp2515_Write(CANINTE,TX0IE |RX0IE |RX1IE);
    //    mcp2515_Write(CANINTE,RX0IE |RX1IE|TX0IE|WAKIE|ERRIE|MERRE);
    mcp2515_Write(CANINTE,RX0IE |RX1IE|WAKIE|TX0IE|ERRIE|MERRE);
     // mcp2515_Write(CANINTE,RX0IE);
	//printk("CANINTE = %x\n", mcp2515_Read(CANINTE));
	
	/* Clear all interrupts */
       mcp2515_Write(CANINTF,0x00);
  
	
	/*Set CAN work mode*/

   	mcp2515_Write(CANCTRL, (REQOP_NORMAL | CLKEN | CLKPRE));
    

   	if ((mcp2515_Read(CANSTAT) & 0xE0 )!= OPMODE_NORMAL) 
   	{

		 return 1;
	} 
	
   	return 0;
        
}











