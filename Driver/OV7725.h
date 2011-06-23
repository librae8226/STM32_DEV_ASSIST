/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : OV7725.h
* Author             : Librae
* Last Modified Date : 10/08/2010
* Description        : OV7725.

******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __OV7725_H__
#define __OV7725_H__

/* Includes -----------------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported constants -------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/
//OV7725
#define CHANGE_REG_NUM 72

#define OV7725_VSYNC_BIT    			GPIO_Pin_6	//PC6

#define GPIO_VSYNC_CMOS					GPIOC
#define RCC_APB2Periph_GPIO_VSYNC_CMOS	RCC_APB2Periph_GPIOC
#define GPIO_PIN_VSYNC_CMOS				GPIO_Pin_6
#define EXTI_LINE_VSYNC_CMOS			EXTI_Line6
#define GPIO_PORT_SOURCE_VSYNC_CMOS		GPIO_PortSourceGPIOC
#define GPIO_PIN_SOURCE_VSYNC_CMOS		GPIO_PinSource6

//SCCB
#define SCCB_CTRL_PORT	GPIOC
#define SCCB_SIC_BIT	GPIO_Pin_4
#define SCCB_SID_BIT	GPIO_Pin_5

#define SCCB_SIC_H()	SCCB_CTRL_PORT->BSRR = SCCB_SIC_BIT;
#define SCCB_SIC_L()	SCCB_CTRL_PORT->BRR =  SCCB_SIC_BIT;

#define SCCB_SID_H()	SCCB_CTRL_PORT->BSRR = SCCB_SID_BIT;
#define SCCB_SID_L()	SCCB_CTRL_PORT->BRR =  SCCB_SID_BIT;

#define SCCB_SID_IN		SCCB_SID_GPIO_INPUT()
#define SCCB_SID_OUT	SCCB_SID_GPIO_OUTPUT()

#define SCCB_SID_STATE	SCCB_CTRL_PORT->IDR&0x0008

//FIFO
#define FIFO_CTRL_PORT	GPIOC
#define FIFO_DATA_PORT	GPIOD

#define FIFO_D0			GPIO_Pin_0
#define FIFO_D1			GPIO_Pin_1
#define FIFO_D2			GPIO_Pin_2
#define FIFO_D3			GPIO_Pin_3
#define FIFO_D4			GPIO_Pin_4
#define FIFO_D5			GPIO_Pin_5
#define FIFO_D6			GPIO_Pin_6
#define FIFO_D7			GPIO_Pin_7

#define FIFO_CS_BIT     GPIO_Pin_8  // OE
#define FIFO_WRST_BIT   GPIO_Pin_9  // WRST
#define FIFO_RRST_BIT   GPIO_Pin_10 // RRST
#define FIFO_RD_BIT     GPIO_Pin_11  // RCLK
#define FIFO_WE_BIT     GPIO_Pin_12  // WE

#define FIFO_CS_H()    FIFO_CTRL_PORT->BSRR =FIFO_CS_BIT
#define FIFO_CS_L()    FIFO_CTRL_PORT->BRR  =FIFO_CS_BIT

#define FIFO_WRST_H()  FIFO_CTRL_PORT->BSRR =FIFO_WRST_BIT
#define FIFO_WRST_L()  FIFO_CTRL_PORT->BRR  =FIFO_WRST_BIT

#define FIFO_RRST_H()  FIFO_CTRL_PORT->BSRR =FIFO_RRST_BIT
#define FIFO_RRST_L()  FIFO_CTRL_PORT->BRR  =FIFO_RRST_BIT

#define FIFO_RD_H()    FIFO_CTRL_PORT->BSRR =FIFO_RD_BIT
#define FIFO_RD_L()    FIFO_CTRL_PORT->BRR  =FIFO_RD_BIT

#define FIFO_WE_H()    FIFO_CTRL_PORT->BSRR =FIFO_WE_BIT
#define FIFO_WE_L()    FIFO_CTRL_PORT->BRR  =FIFO_WE_BIT

/* Exported variables -------------------------------------------------------*/

extern const char change_reg[CHANGE_REG_NUM][2];

extern BOOL vsync_flag;

/* Exported functions ------------------------------------------------------ */
unsigned char wrov7725Reg(unsigned char regID, unsigned char regDat);
unsigned char rdov7725Reg(unsigned char regID, unsigned char *regDat);
unsigned char ov7725_init(void);

//void SCCB_GPIO_Config(void);
void SCCB_SID_GPIO_OUTPUT(void);
void SCCB_SID_GPIO_INPUT(void);
void startSCCB(void);
void stopSCCB(void);
void noAck(void);
unsigned char SCCBwriteByte(unsigned char m_data);
unsigned char SCCBreadByte(void);

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
