/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : OV7725.c
* Author             : Librae
* Last Modified Date : 10/08/2010
* Description        : OV7725.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
//#include "includes.h"
#include "os_cpu.h"
#include "integer.h"	// Basic integer types
#include "OV7725.h"
#include "CPU_Init.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
const char change_reg[CHANGE_REG_NUM][2]=
{	
   /*以下为OV7725QVGA RGB565参数  */
  
  {0x32,0x00},
  {0x2a,0x00},
  {0x11,0x01},
  {0x12,0x46},//QVGA RGB565
  {0x12,0x46},

  
  {0x42,0x7f},
  {0x4d,0x00},//0x09
  {0x63,0xf0},
  {0x64,0xff},
  {0x65,0x20},
  {0x66,0x00},
  {0x67,0x00},
  {0x69,0x5d},  
 
  
  {0x13,0xff},
  {0x0d,0x61},//PLL
  {0x0f,0xc5},
  {0x14,0x11},
  {0x22,0xFF},//7f
  {0x23,0x01},
  {0x24,0x34},
  {0x25,0x3c},
  {0x26,0xa1},
  {0x2b,0x00},
  {0x6b,0xaa},
  {0x13,0xff},

  {0x90,0x0a},//
  {0x91,0x01},//
  {0x92,0x01},//
  {0x93,0x01},
  
  {0x94,0x5f},
  {0x95,0x53},
  {0x96,0x11},
  {0x97,0x1a},
  {0x98,0x3d},
  {0x99,0x5a},
  {0x9a,0x1e},
  
  {0x9b,0x00},//set luma 
  {0x9c,0x25},//set contrast 
  {0xa7,0x65},//set saturation  
  {0xa8,0x65},//set saturation 
  {0xa9,0x80},//set hue 
  {0xaa,0x80},//set hue 
  
  {0x9e,0x81},
  {0xa6,0x06},

  {0x7e,0x0c},
  {0x7f,0x16},
  {0x80,0x2a},
  {0x81,0x4e},
  {0x82,0x61},
  {0x83,0x6f},
  {0x84,0x7b},
  {0x85,0x86},
  {0x86,0x8e},
  {0x87,0x97},
  {0x88,0xa4},
  {0x89,0xaf},
  {0x8a,0xc5},
  {0x8b,0xd7},
  {0x8c,0xe8},
  {0x8d,0x20},

  {0x33,0x00},
  {0x22,0x99},
  {0x23,0x03},
  {0x4a,0x00},
  {0x49,0x13},
  {0x47,0x08},
  {0x4b,0x14},
  {0x4c,0x17},
  {0x46,0x05},
  {0x0e,0xf5},
  {0x0c,0xd0},
};

BOOL vsync_flag;

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/
////////////////////////////
//功能：写OV7660寄存器
//返回：1-成功	0-失败
unsigned char wrov7725Reg(unsigned char regID, unsigned char regDat)
{
	startSCCB();
	if(0==SCCBwriteByte(0x42))
	{
		stopSCCB();
		return(0);
	}
	delay_us(100);
  	if(0==SCCBwriteByte(regID))
	{
		stopSCCB();
		return(0);
	}
	delay_us(100);
  	if(0==SCCBwriteByte(regDat))
	{
		stopSCCB();
		return(0);
	}
  	stopSCCB();
	
  	return(1);
}

////////////////////////////
//功能：读OV7660寄存器
//返回：1-成功	0-失败
unsigned char rdov7725Reg(unsigned char regID, unsigned char *regDat)
{
	//通过写操作设置寄存器地址
	startSCCB();
	if(0==SCCBwriteByte(0x42))
	{
		stopSCCB();
		return(0);
	}
	delay_us(100);
  	if(0==SCCBwriteByte(regID))
	{
		stopSCCB();
		return(0);
	}
	stopSCCB();
	
	delay_us(100);
	
	//设置寄存器地址后，才是读
	startSCCB();
	if(0==SCCBwriteByte(0x43))
	{
		stopSCCB();
		return(0);
	}
	delay_us(100);
  	*regDat=SCCBreadByte();
  	noAck();
  	stopSCCB();
  	return(1);
}


/* OV7725_init() */
//返回1成功，返回0失败
unsigned char ov7725_init(void)
{
	unsigned char temp;
	unsigned int i = 0;

	//XCLK_init_ON();
	//uchar ovidmsb=0, ovidlsb=0;
	//ov7725_GPIO_Init();
	//SCCB_GPIO_Config();
	
	temp = 0x80;
	if( 0 == wrov7725Reg(0x12, temp) ) //Reset SCCB
	{
		return 0;
	}
	delay_ms(50);
	
	for( i = 0; i < CHANGE_REG_NUM; i++ )
	{
		if( 0 == wrov7725Reg(change_reg[i][0], change_reg[i][1]) )
		{
			return 0;
		}
	}
	
	return 0x01; //ok
}

////////////////////////////////////////////////////
void SCCB_SID_GPIO_OUTPUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin =  SCCB_SID_BIT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCCB_CTRL_PORT, &GPIO_InitStructure);
}
void SCCB_SID_GPIO_INPUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin =  SCCB_SID_BIT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCCB_CTRL_PORT, &GPIO_InitStructure);
}
/*
-----------------------------------------------
   功能: start命令,SCCB的起始信号
   参数: 无
 返回值: 无
-----------------------------------------------
*/
void startSCCB(void)
{
    SCCB_SID_H();     //数据线高电平
    delay_us(500);

    SCCB_SIC_H();	   //在时钟线高的时候数据线由高至低
    delay_us(500);
 
    SCCB_SID_L();
    delay_us(500);

    SCCB_SIC_L();	 //数据线恢复低电平，单操作函数必要
    delay_us(500);


}
/*
-----------------------------------------------
   功能: stop命令,SCCB的停止信号
   参数: 无
 返回值: 无
-----------------------------------------------
*/
void stopSCCB(void)
{
    SCCB_SID_L();
    delay_us(500);
 
    SCCB_SIC_H();	
    delay_us(500);
  

    SCCB_SID_H();	
    delay_us(500);
   
}

/*
-----------------------------------------------
   功能: noAck,用于连续读取中的最后一个结束周期
   参数: 无
 返回值: 无
-----------------------------------------------
*/
void noAck(void)
{
	
	SCCB_SID_H();	
	delay_us(500);
	
	SCCB_SIC_H();	
	delay_us(500);
	
	SCCB_SIC_L();	
	delay_us(500);
	
	SCCB_SID_L();	
	delay_us(500);

}

/*
-----------------------------------------------
   功能: 写入一个字节的数据到SCCB
   参数: 写入数据
 返回值: 发送成功返回1，发送失败返回0
-----------------------------------------------
*/
unsigned char SCCBwriteByte(unsigned char m_data)
{
	unsigned char j,tem;

	for(j=0;j<8;j++) //循环8次发送数据
	{
		if((m_data<<j)&0x80)
		{
			SCCB_SID_H();	
		}
		else
		{
			SCCB_SID_L();	
		}
		delay_us(500);
		SCCB_SIC_H();	
		delay_us(500);
		SCCB_SIC_L();	
		delay_us(500);

	}
	delay_us(100);
	SCCB_SID_IN;/*设置SDA为输入*/
	delay_us(500);
	SCCB_SIC_H();	
	delay_us(100);
	if(SCCB_SID_STATE)
	{
		tem=0;
	}   //SDA=1发送失败，返回0}
	else
	{
		tem=1;
	}   //SDA=0发送成功，返回1
	SCCB_SIC_L();	
	delay_us(500);
	SCCB_SID_OUT;/*设置SDA为输出*/

	return (tem);  
}

/*
-----------------------------------------------
   功能: 一个字节数据读取并且返回
   参数: 无
 返回值: 读取到的数据
-----------------------------------------------
*/
unsigned char SCCBreadByte(void)
{
	unsigned char read,j;
	read=0x00;
	
	SCCB_SID_IN;/*设置SDA为输入*/
	delay_us(500);
	for(j=8;j>0;j--) //循环8次接收数据
	{		     
		delay_us(500);
		SCCB_SIC_H();
		delay_us(500);
		read=read<<1;
		if(SCCB_SID_STATE) 
		{
			read=read+1;
		}
		SCCB_SIC_L();
		delay_us(500);
	}	
	return(read);
}

////////////////////////////////////////////////////


/************************ (C) COPYLEFT 2010 Leafgrass ************************/
