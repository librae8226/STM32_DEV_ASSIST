/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : NRF24L01.h
* Author             : Librae
* Last Modified Date : 11/03/2010
* Description        : Provide the stuffs needed by nrf24l01 module.

******************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __NRF24L01_H__
#define __NRF24L01_H__

/* Includes ---------------------------------------------------------------- */
/* Exported types ---------------------------------------------------------- */
// BYTE type definition
#ifndef _BYTE_DEF_
#define _BYTE_DEF_
typedef unsigned char BYTE;
#endif	/* _BYTE_DEF_ */

/* Exported constants ------------------------------------------------------ */
#define TX_PLOAD_WIDTH  20  	// 20 uints TX payload
#define RX_PLOAD_WIDTH  20  	// 20 uints TX payload

#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width

///*********************NRF24L01�Ĵ���ָ��*************************************
#define _READ_REG        0x00  	// ���Ĵ���ָ��
#define _WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����
///********************SPI(nRF24L01)�Ĵ�����ַ*********************************
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define NRFRegSTATUS    0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��1�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��2�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��3�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��4�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��5�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
///****************************************************************************

/* Exported macro ---------------------------------------------------------- */
#define uchar unsigned char

#define NRF24_TX_Mode  TX_Mode
#define NRF24_RX_Mode  RX_Mode

#define NRF24L01_CTRL_PORT	GPIOB
#define NRF24L01_CE			GPIO_Pin_5
#define NRF24L01_CSN		GPIO_Pin_7
#define NRF24L01_CE_H()		(NRF24L01_CTRL_PORT->BSRR = NRF24L01_CE)
#define NRF24L01_CE_L()		(NRF24L01_CTRL_PORT->BRR = NRF24L01_CE)
#define NRF24L01_CSN_H()	(NRF24L01_CTRL_PORT->BSRR = NRF24L01_CSN)
#define NRF24L01_CSN_L()	(NRF24L01_CTRL_PORT->BRR = NRF24L01_CSN)

#define CE_H()		NRF24L01_CE_H()
#define CE_L()		NRF24L01_CE_L()
#define CSN_H()		NRF24L01_CSN_H()
#define CSN_L()		NRF24L01_CSN_L()

#define NRF24SPI_Send_Byte	SPI_Send_Byte
#define Delay_us(x)			delay_us(x)
#define IntervalTimems(x)	Wait_ms(x)

/* Exported variables ------------------------------------------------------ */
extern u8 TxBuf[32];
extern u8 RxBuf[32];
extern u8 TX_ADDRESS[TX_ADR_WIDTH];	//���ص�ַ
extern u8 RX_ADDRESS[RX_ADR_WIDTH];	//���յ�ַ

/* Exported functions ------------------------------------------------------ */
unsigned char SPI_Send_Byte(unsigned char byte);
void init_NRF24L01(void);
//void NRF24L01_Init(void);
void TX_Mode(void);
void RX_Mode(void);
void NRF_Send(void);
void NRF_Receive(void);

u8 SPI_WR_Reg(u8 reg, u8 value);
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 Len);
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len);
u8 nRF24L01_RxPacket(unsigned char* rx_buf);
void nRF24L01_TxPacket(unsigned char * tx_buf);
u8 SPI_RD_Reg(u8 reg);

//void SPI_Init(BYTE Mode);     // Init HW or SW SPI
//BYTE SPI_RW(BYTE byte);                                // Single SPI read/write
//BYTE SPI_Read(BYTE reg);                               // Read one byte from nRF24L01
//BYTE SPI_RW_Reg(BYTE reg, BYTE byte);                  // Write one byte to register 'reg'
//BYTE SPI_Write_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);  // Writes multiply bytes to one register
//BYTE SPI_Read_Buf(BYTE reg, BYTE *pBuf, BYTE bytes);   // Read multiply bytes from one register

#endif	/* __NRF24L01_H__ */

/************************ (C) COPYLEFT 2010 Leafgrass ************************/


