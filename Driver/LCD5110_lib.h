/*********************** (C) COPYLEFT 2010 Leafgrass **************************

* File Name          : LCD5110_lib.h
* Author             : Librae
* Version            : V1.0
* Last Modified Date : 06/12/2010
* Description        : This file provides the NOKIA LCD5110 LCD
						character and font library.

*******************************************************************************/

//-------------------------------------------------------------------\\
//	Nokia5110指令集													 \\
//  D7  D6  D5  D4  D3  D2  D1  D0									 \\
//  0   0   1   0   0   PD  V   H									 \\
//                      |   |   |_______1->扩展指令集  0->基本指令集 \\
//                      |   |___________1->垂直寻址    0->水平寻址 	 \\
//                      |_______________1->低功耗模式  0->正常模式	 \\
//-------------------------------------------------------------------\\
//	显示模式														 \\
//  D7  D6  D5  D4  D3  D2  D1  D0									 \\
//  0   0   0   0   1   D   0   E									 \\
//                      |       |_______0    0    1    1			 \\
//                      |_______________0    1    0    1			 \\
//                                      白屏 正常 全显 反转			 \\
//-------------------------------------------------------------------\\

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD5110_LIB_H__
#define __LCD5110_LIB_H__

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Exported types ------------------------------------------------------------*/
//枚举 D/C模式选择 
typedef enum
{		                           
    DC_CMD  = 0,	//command
	DC_DATA = 1		//data
} DCType;

/* Exported variables --------------------------------------------------------*/
extern vu32 TimingDelay;
extern u8 bmp[];
extern u8 mcu[];
extern u8 pic[];
extern u8 car[];

/* Exported constants --------------------------------------------------------*/
#define LCD_X_RES		84	//LCD length
#define LCD_Y_RES		48	//LCD width
#define LCD_CTRL_PORT	GPIOB
#define LCD_RST			GPIO_Pin_5
#define LCD_CE			GPIO_Pin_7
#define LCD_DC			GPIO_Pin_9
#define LCD_VCC			GPIO_Pin_11
#define LCD_BGL			GPIO_Pin_3
#define LCD_GND			GPIO_Pin_1
//config as spi2 
#define LCD_MOSI		GPIO_Pin_15
#define LCD_CLK			GPIO_Pin_13

/* Exported macro ------------------------------------------------------------*/
#define USESPI 1
#define LCD_RST_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_RST)
#define LCD_RST_L()		GPIO_ResetBits(LCD_CTRL_PORT, LCD_RST)
#define LCD_CE_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_CE)
#define LCD_CE_L()		GPIO_ResetBits(LCD_CTRL_PORT, LCD_CE)
#define LCD_DC_DATA()	GPIO_SetBits(LCD_CTRL_PORT, LCD_DC)
#define LCD_DC_CMD()	GPIO_ResetBits(LCD_CTRL_PORT, LCD_DC)
#define LCD_MOSI_H()	GPIO_SetBits(LCD_CTRL_PORT, LCD_MOSI)
#define LCD_MOSI_L()	GPIO_ResetBits(LCD_CTRL_PORT, LCD_MOSI)
#define LCD_CLK_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_CLK)
#define LCD_CLK_L()		GPIO_ResetBits(LCD_CTRL_PORT, LCD_CLK)
#define LCD_VCC_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_VCC)
#define	LCD_BGL_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_BGL)
#define LCD_GND_L()		GPIO_ResetBits(LCD_CTRL_PORT, LCD_GND)
#define LCD_INVERSE()	LCD_Send(0x0D, DC_CMD)
#define LCD_NORMAL()	LCD_Send(0x0C, DC_CMD)

/* Exported functions ------------------------------------------------------- */
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetXY(u8 X, u8 Y);
void LCD_SetContrast(u8 contrast);
void LCD_Send(u8 data, DCType dc);
void LCD_Write_Char(u8 ascii);
void LCD_Write_EnStr(u8 X, u8 Y, u8* s);
void LCD_Write_ChStr(u8 X, u8 Y, u8 num);
void LCD_Draw(u8 X1, u8 Y1, u8 X2, u8 Y2, u8 type);
void LCD_draw_bmp_pixel(u8 X, u8 Y,u8 *map, u8 Pix_x,u8 Pix_y);
void LCD_write_chinese_string(u8 X, u8 Y, u8 ch_with, u8 num, u8 line, u8 row);

#endif

/*********************** (C) COPYLEFT 2010 Leafgrass *************************/
