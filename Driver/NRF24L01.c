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
u8  TX_ADDRESS[TX_ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};	//���ص�ַ
u8  RX_ADDRESS[RX_ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};	//���յ�ַ

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
///*NRF24L01��ʼ��
///***************************************************************************************/
void init_NRF24L01(void)
{
 u8 buf[5]={0};
 
  Delay_us(100);


  CE_L();    // chip enable
  CSN_H();   // Spi disable 
  
  SPI_Read_Buf(TX_ADDR, buf, TX_ADR_WIDTH);//debug ����ԭ���ı��ص�ַ����λֵ�ǣ�0xE7 0xE7 0xE7 0xE7 0xE7
  
//	SPI_Write_Buf(_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
//	SPI_Write_Buf(_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ

//	
//	SPI_WR_Reg(_WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
//	SPI_WR_Reg(_WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
//	SPI_WR_Reg(_WRITE_REG + SETUP_RETR, 0x1a); // �����Զ��ط�ʱ��ʹ�����500us + 86us, 10 retrans...
//	SPI_WR_Reg(_WRITE_REG + RF_CH, 22);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
//	SPI_WR_Reg(_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
//	SPI_WR_Reg(_WRITE_REG + RF_SETUP, 0x07);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB
//  
//  SPI_RD_Reg(_WRITE_REG + EN_AA);
//  SPI_RD_Reg(_WRITE_REG + EN_RXADDR);
//  SPI_RD_Reg(_WRITE_REG + RF_CH);
//  SPI_RD_Reg(_WRITE_REG + RX_PW_P0);
//  SPI_RD_Reg(_WRITE_REG + RF_SETUP);
}

///****************************************************************************************************
///*������uchar SPI_Read(u8 reg)
///*���ܣ�NRF24L01��SPIʱ��
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
//*���ܣ�NRF24L01��д�Ĵ�������
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
//*������uint SPI_Read_Buf(u8 reg, u8 *pBuf, u8 Len)
//*����: ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uchars���������ݵĸ���
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
//*������uint SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len)
//*����: ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uchars��д�����ݵĸ���
//*********************************************************************************************************/
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len)
{
	unsigned int status,i;
	
	CSN_L();            //SPIʹ��       
	status = NRF24SPI_Send_Byte(reg);   
	for(i=0; i<Len; i++) //
	{
	   NRF24SPI_Send_Byte(*pBuf);
		 pBuf ++;
	}
	CSN_H();           //�ر�SPI
	return(status);    // 
}

//****************************************************************************************************/
//*������void SetRX_Mode(void)
//*���ܣ����ݽ������� 
//****************************************************************************************************/
void RX_Mode(void)
{u8 buf[5]={0};
	CE_L();
	
	SPI_Read_Buf(TX_ADDR, buf, TX_ADR_WIDTH);//debug ����ԭ���ı��ص�ַ����λֵ�ǣ�0xE7 0xE7 0xE7 0xE7 0xE7
	//SPI_WR_Reg(_WRITE_REG + CONFIG, 0x03);//SPI_WR_Reg(_WRITE_REG + CONFIG, 0x0f); // IRQ�շ�����ж���Ӧ��16λCRC	��������
	
	//SPI_Write_Buf(_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	
	SPI_WR_Reg(_WRITE_REG + EN_AA, 0);//0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_WR_Reg(_WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_WR_Reg(_WRITE_REG + SETUP_RETR, 0x1a); // �����Զ��ط�ʱ��ʹ�����500us + 86us, 10 retrans...
	SPI_WR_Reg(_WRITE_REG + RF_CH, 40);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_WR_Reg(_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_WR_Reg(_WRITE_REG + RF_SETUP, 0x0F);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB
	
	SPI_WR_Reg(_WRITE_REG + CONFIG, 0x03);//0x0F);	
	CE_H(); 
	Delay_us(200);
}

void TX_Mode(void)
{
 CE_L();
	//SPI_WR_Reg(_WRITE_REG + CONFIG, 0x02);   //0x0E		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	
	SPI_Write_Buf(_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	
	SPI_WR_Reg(_WRITE_REG + EN_AA, 0);//0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_WR_Reg(_WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_WR_Reg(_WRITE_REG + SETUP_RETR, 0x1a); // �����Զ��ط�ʱ��ʹ�����500us + 86us, 10 retrans...
	SPI_WR_Reg(_WRITE_REG + RF_CH, 40);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
//	SPI_WR_Reg(_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_WR_Reg(_WRITE_REG + RF_SETUP, 0x0F);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB

	SPI_WR_Reg(_WRITE_REG + CONFIG, 0x02);//0x0E);
 CE_H();
}
//***********************************************************************************************************
//*������void nRF24L01_TxPacket(unsigned char * tx_buf)
//*���ܣ����� tx_buf������
//**********************************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE_L();			//StandBy Iģʽ	
	SPI_Write_Buf(_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // װ������	
//	SPI_WR_Reg(_WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	CE_H();		 //�ø�CE���������ݷ���
	//Delay_us(10);
}

//******************************************************************************************************/
//*������unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//*���ܣ����ݶ�ȡ�����rx_buf���ջ�������
//******************************************************************************************************/
u8 nRF24L01_RxPacket(unsigned char* rx_buf)
{u8 flag=0;
 u8 status;
 	
	status=SPI_RD_Reg(NRFRegSTATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
	
	if(status & 0x40)				// �ж��Ƿ���յ�����
	{
	   //CE_L(); 			//SPIʹ��
		 SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		 flag =1;			//��ȡ������ɱ�־
	}
	SPI_WR_Reg(_WRITE_REG + NRFRegSTATUS, status);   //���յ����ݺ�RX_DR,TX_DS,MAX_RT���ø�Ϊ1��ͨ��д1������жϱ�־
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
   
   if(nRF24L01_RxPacket(RxBuf))            //if ���ݶ�ȡ�ɹ�
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
