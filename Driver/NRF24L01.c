/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : NRF24L01.c
* Author             : Librae
* Last Modified Date : 11/03/2010
* Description        : Provide the stuffs needed by nrf24l01 module.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/

//#include "includes.h"
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "CPU_Init.h"
#include "platform_config.h"
#include "NRF24L01.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
#define NRF_GLOBALS

/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
u8 TxBuf[32]={0};
u8 RxBuf[32]={0};
u8  TX_ADDRESS[TX_ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};	//本地地址
u8  RX_ADDRESS[RX_ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};	//接收地址

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/******************************************************************************
* Function Name  : SPI_Send_Byte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
******************************************************************************/
u8 SPI_Send_Byte(u8 dat)
{
  // Loop while DR register in not emplty
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

  // Send byte through the SPI2 peripheral
  SPI_I2S_SendData(SPI2, dat);

  // Wait to receive a byte
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  // Return the byte read from the SPI bus
  return SPI_I2S_ReceiveData(SPI2);
}

///****************************************************************************************
///*NRF24L01初始化
///***************************************************************************************/
void init_NRF24L01(void)
{
 u8 buf[5]={0};
 
  Delay_us(100);


  CE_L();    // chip enable
  CSN_H();   // Spi disable 
  
  SPI_Read_Buf(TX_ADDR, buf, TX_ADR_WIDTH);//debug 测试原来的本地地址：复位值是：0xE7 0xE7 0xE7 0xE7 0xE7
  
//	SPI_Write_Buf(_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
//	SPI_Write_Buf(_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址

//	
//	SPI_WR_Reg(_WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
//	SPI_WR_Reg(_WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
//	SPI_WR_Reg(_WRITE_REG + SETUP_RETR, 0x1a); // 设置自动重发时间和次数：500us + 86us, 10 retrans...
//	SPI_WR_Reg(_WRITE_REG + RF_CH, 22);        //   设置信道工作为2.4GHZ，收发必须一致
//	SPI_WR_Reg(_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
//	SPI_WR_Reg(_WRITE_REG + RF_SETUP, 0x07);   		//设置发射速率为1MHZ，发射功率为最大值0dB
//  
//  SPI_RD_Reg(_WRITE_REG + EN_AA);
//  SPI_RD_Reg(_WRITE_REG + EN_RXADDR);
//  SPI_RD_Reg(_WRITE_REG + RF_CH);
//  SPI_RD_Reg(_WRITE_REG + RX_PW_P0);
//  SPI_RD_Reg(_WRITE_REG + RF_SETUP);
}

///****************************************************************************************************
///*函数：uchar SPI_Read(u8 reg)
///*功能：NRF24L01的SPI时序
///****************************************************************************************************/
u8 SPI_RD_Reg(u8 reg)
{
	u8 reg_val;
	
	CSN_L();                // CSN low, initialize SPI communication...
	NRF24SPI_Send_Byte(reg);            // Select register to read from..
	reg_val = NRF24SPI_Send_Byte(0);    // ..then read registervalue
	CSN_H();                // CSN high, terminate SPI communication
	
	return(reg_val);        // return register value
}

//****************************************************************************************************/
//*功能：NRF24L01读写寄存器函数
//****************************************************************************************************/
u8 SPI_WR_Reg(u8 reg, u8 value)
{
	u8 status;
	
	CSN_L();                   // CSN low, init SPI transaction
	status = NRF24SPI_Send_Byte(reg);// select register
	NRF24SPI_Send_Byte(value);             // ..and write value to it..
	CSN_H();                   // CSN high again
	
	return(status);            // return nRF24L01 status u8
}
///****************************************************************************************************/
//*函数：uint SPI_Read_Buf(u8 reg, u8 *pBuf, u8 Len)
//*功能: 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uchars：读出数据的个数
//****************************************************************************************************/
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 Len)
{
	unsigned int status,i;
	
	CSN_L();                    		// Set CSN low, init SPI tranaction
	status = NRF24SPI_Send_Byte(reg);       		// Select register to write to and read status u8
	
  for(i=0;i<Len;i++)
  {
     pBuf[i] = NRF24SPI_Send_Byte(0);
  }
	
	CSN_H();                           
	
	return(status);                    // return nRF24L01 status u8
}
//*********************************************************************************************************
//*函数：uint SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len)
//*功能: 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uchars：写入数据的个数
//*********************************************************************************************************/
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len)
{
	unsigned int status,i;
	
	CSN_L();            //SPI使能       
	status = NRF24SPI_Send_Byte(reg);   
	for(i=0; i<Len; i++) //
	{
	   NRF24SPI_Send_Byte(*pBuf);
		 pBuf ++;
	}
	CSN_H();           //关闭SPI
	return(status);    // 
}

//****************************************************************************************************/
//*函数：void SetRX_Mode(void)
//*功能：数据接收配置 
//****************************************************************************************************/
void RX_Mode(void)
{u8 buf[5]={0};
	CE_L();
	
	SPI_Read_Buf(TX_ADDR, buf, TX_ADR_WIDTH);//debug 测试原来的本地地址：复位值是：0xE7 0xE7 0xE7 0xE7 0xE7
	//SPI_WR_Reg(_WRITE_REG + CONFIG, 0x03);//SPI_WR_Reg(_WRITE_REG + CONFIG, 0x0f); // IRQ收发完成中断响应，16位CRC	，主接收
	
	//SPI_Write_Buf(_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	
	SPI_WR_Reg(_WRITE_REG + EN_AA, 0);//0x01);      //  频道0自动	ACK应答允许	
	SPI_WR_Reg(_WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_WR_Reg(_WRITE_REG + SETUP_RETR, 0x1a); // 设置自动重发时间和次数：500us + 86us, 10 retrans...
	SPI_WR_Reg(_WRITE_REG + RF_CH, 40);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_WR_Reg(_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_WR_Reg(_WRITE_REG + RF_SETUP, 0x0F);   		//设置发射速率为1MHZ，发射功率为最大值0dB
	
	SPI_WR_Reg(_WRITE_REG + CONFIG, 0x03);//0x0F);	
	CE_H(); 
	Delay_us(200);
}

void TX_Mode(void)
{
 CE_L();
	//SPI_WR_Reg(_WRITE_REG + CONFIG, 0x02);   //0x0E		 // IRQ收发完成中断响应，16位CRC，主发送
	
	SPI_Write_Buf(_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	
	SPI_WR_Reg(_WRITE_REG + EN_AA, 0);//0x01);      //  频道0自动	ACK应答允许	
	SPI_WR_Reg(_WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_WR_Reg(_WRITE_REG + SETUP_RETR, 0x1a); // 设置自动重发时间和次数：500us + 86us, 10 retrans...
	SPI_WR_Reg(_WRITE_REG + RF_CH, 40);        //   设置信道工作为2.4GHZ，收发必须一致
//	SPI_WR_Reg(_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_WR_Reg(_WRITE_REG + RF_SETUP, 0x0F);   		//设置发射速率为1MHZ，发射功率为最大值0dB

	SPI_WR_Reg(_WRITE_REG + CONFIG, 0x02);//0x0E);
 CE_H();
}
//***********************************************************************************************************
//*函数：void nRF24L01_TxPacket(unsigned char * tx_buf)
//*功能：发送 tx_buf中数据
//**********************************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE_L();			//StandBy I模式	
	SPI_Write_Buf(_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据	
//	SPI_WR_Reg(_WRITE_REG + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送
	CE_H();		 //置高CE，激发数据发送
	//Delay_us(10);
}

//******************************************************************************************************/
//*函数：unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//*功能：数据读取后放如rx_buf接收缓冲区中
//******************************************************************************************************/
u8 nRF24L01_RxPacket(unsigned char* rx_buf)
{u8 flag=0;
 u8 status;
 	
	status=SPI_RD_Reg(NRFRegSTATUS);	// 读取状态寄存其来判断数据接收状况
	
	if(status & 0x40)				// 判断是否接收到数据
	{
	   //CE_L(); 			//SPI使能
		 SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		 flag =1;			//读取数据完成标志
	}
	SPI_WR_Reg(_WRITE_REG + NRFRegSTATUS, status);   //接收到数据后RX_DR,TX_DS,MAX_RT都置高为1，通过写1来清楚中断标志
	return flag;
}

void NRF_Send(void)
{
//static u32 counter=0;
 static u8 flag=0;  
   
   //TX_Mode();
   
   if(flag==0)
   {
      flag=1;
      memcpy(TxBuf, "1234567890abcdefghij", 20);
      nRF24L01_TxPacket(TxBuf);
   }
   else
   {
      flag=0;
      memcpy(TxBuf, "abcdefghij1234567890", 20);
      nRF24L01_TxPacket(TxBuf);
   }
   //GPIO_Write(GPIOC, (u16)~GPIO_ReadOutputData(GPIOC));
}

void NRF_Receive(void)
{     
 //static u32 counter=0;
 //static u8 flag=0;  
  
   //TX_Mode();
   
//   IntervalTimems(10);
   
   if(nRF24L01_RxPacket(RxBuf))            //if 数据读取成功
   {
      if( memcmp(RxBuf, "1234567890abcdefghij", 20)==0 )
      {
         //GPIO_SetBits(GPIOC, GPIO_Pin_11);
		 //GPIO_SetBits(GPIOD, GPIO_Pin_2);
      }
      else if( memcmp(RxBuf, "abcdefghij1234567890", 20)==0 )
      {
         //GPIO_ResetBits(GPIOC, GPIO_Pin_11);
		 //GPIO_ResetBits(GPIOD, GPIO_Pin_2);
      }
	  //GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	  LED2_ON();
	  RxBuf[31] = '\0';
      printf("%s", RxBuf);
   }
   
   memset(RxBuf, 0, 20);
   //memcpy(TxBuf, "1234567890abcdefghijklmn", 20);
   //nRF24L01_TxPacket(TxBuf);
}

/*********************** (C) COPYLEFT 2010 Leafgrass *************************/
