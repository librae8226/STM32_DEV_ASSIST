/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : LTM022A69B.h
* Author             : Librae
* Last Modified Date : 08/10/2010
* Description        : This file provides the 
						LTM022A69B LCD related functions' declaration.

******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __LTM022A69B_H__
#define __LTM022A69B_H__

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"
																			 
/* Exported types -----------------------------------------------------------*/
typedef enum
{		                           
    DC_CMD  = 0,	//command
	DC_DATA = 1		//data
} DCType;

/* Exported variables -------------------------------------------------------*/

/* Exported constants -------------------------------------------------------*/
//if IO for LCD is to be changed, just modify the constants below
#define LCD_X_SIZE		240	//LCD width
#define LCD_Y_SIZE		320	//LCD height

#define SPI_LCD			SPI1

#define LCD_CTRL_PORT	GPIOC
#define LCD_RST			GPIO_Pin_8
#define LCD_CS			GPIO_Pin_9
#define LCD_RS			GPIO_Pin_10
#define LCD_SI			GPIO_Pin_11
#define LCD_SCL			GPIO_Pin_12

//color
#define RED				0xF800
#define GREEN			0x07E0
#define BLUE			0x001F
#define YELLOW			0xFFE0
#define BLACK			0x0000
#define WHITE			0xFFFF
#define CYAN			0x07FF
#define PURPLE			0xF81F

/* Exported macro ------------------------------------------------------------*/
#define USESPI			1
/*
#define LCD_RST_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_RST)
#define LCD_RST_L()		GPIO_ResetBits(LCD_CTRL_PORT, LCD_RST)
#define LCD_CS_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_CS)
#define LCD_CS_L()		GPIO_ResetBits(LCD_CTRL_PORT, LCD_CS)
#define LCD_RS_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_RS)
#define LCD_RS_L()		GPIO_ResetBits(LCD_CTRL_PORT, LCD_RS)
#define LCD_SI_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_SI)
#define LCD_SI_L()		GPIO_ResetBits(LCD_CTRL_PORT, LCD_SI)
#define LCD_SCL_H()		GPIO_SetBits(LCD_CTRL_PORT, LCD_SCL)
#define LCD_SCL_L()		GPIO_ResetBits(LCD_CTRL_PORT, LCD_SCL)
*/
//for a higher IO speed
#define LCD_RST_H()		LCD_CTRL_PORT->BSRR = LCD_RST
#define LCD_RST_L()		LCD_CTRL_PORT->BRR = LCD_RST
#define LCD_CS_H()		LCD_CTRL_PORT->BSRR = LCD_CS
#define LCD_CS_L()		LCD_CTRL_PORT->BRR = LCD_CS
#define LCD_RS_H()		LCD_CTRL_PORT->BSRR = LCD_RS
#define LCD_RS_L()		LCD_CTRL_PORT->BRR = LCD_RS
#define LCD_SI_H()		LCD_CTRL_PORT->BSRR = LCD_SI
#define LCD_SI_L()		LCD_CTRL_PORT->BRR = LCD_SI
#define LCD_SCL_H()		LCD_CTRL_PORT->BSRR = LCD_SCL
#define LCD_SCL_L()		LCD_CTRL_PORT->BRR = LCD_SCL

/* Exported functions ------------------------------------------------------ */

//=============================================================================
//							LCD Basic Functions
//=============================================================================

void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index, unsigned int data);
void LCD_WR_DATA(unsigned int data);

void LCD_Init(void);
void LCD_Clear(unsigned int color);
void LCD_Clear_SelectArea(unsigned int color, unsigned char x, unsigned int y, unsigned int width, unsigned height);
void LCD_SetCursor(unsigned char x, unsigned int y);
void LCD_DispChar(unsigned char ch_asc, unsigned int color, unsigned int color_bkgd, unsigned char postion_x, unsigned char postion_y);
//void LCD_DispChar_CursorPos( unsigned char ch_asc, unsigned int color, unsigned int color_bkgd );
void LCD_DispString(unsigned char *s, unsigned int color, unsigned int color_bkgd, unsigned char x, unsigned char y);
//void LCD_DispString_CursorPos( unsigned char *s, unsigned int color, unsigned int color_bkgd );
void LCD_DispGB2312(unsigned char gb, unsigned int color, unsigned int color_bkgd, unsigned char postion_x, unsigned char postion_y);
void LCD_DispImage(const unsigned char *img, unsigned char x, unsigned int y, unsigned int width, unsigned height);
void LCD_Test(void);

//=============================================================================
//							Application Functions
//=============================================================================

void LCD_DrawDot(unsigned int color, unsigned char x, unsigned int y);
int LCD_DrawLine(int c, int x1, int y1, int x2, int y2);	//BresenhamLine algorithm
void Swap ( int *a , int *b );	//for BresenhamLine

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
