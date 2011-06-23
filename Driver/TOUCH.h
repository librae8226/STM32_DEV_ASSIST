/*********************** (C) COPYLEFT 2010 Leafgrass **************************

* File Name          : TOUCH.h
* Author             : Librae
* Last Modified Date : 08/04/2010
* Description        : This file provides the touch pad related functions' declaration.

*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOUCH_H__
#define __TOUCH_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/

#ifndef  uchar
#define   uchar  unsigned char
#endif

#ifndef  uint
#define   uint  unsigned short
#endif

#ifndef  ulong
#define  ulong  unsigned int
#endif

/* Exported variables --------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

// A/D 通道选择命令字和工作寄存器
#define CHX     0x90    //通道X+的选择控制字    
#define CHY     0xD0    //通道Y+的选择控制字

#define TOUCH_CTRL_PORT	GPIOC

#define TOUCH_CS		GPIO_Pin_3
#define TOUCH_CLK		GPIO_Pin_4
#define TOUCH_DIN		GPIO_Pin_5
#define TOUCH_DOUT		GPIO_Pin_6
#define TOUCH_INT		GPIO_Pin_7

/* Exported macro ------------------------------------------------------------*/
#define		TOUCH_CS_H()		TOUCH_CTRL_PORT->BSRR = TOUCH_CS
#define 	TOUCH_CS_L()		TOUCH_CTRL_PORT->BRR = TOUCH_CS
#define 	TOUCH_CLK_H()		TOUCH_CTRL_PORT->BSRR = TOUCH_CLK
#define 	TOUCH_CLK_L()		TOUCH_CTRL_PORT->BRR = TOUCH_CLK
#define 	TOUCH_DIN_H()		TOUCH_CTRL_PORT->BSRR = TOUCH_DIN
#define 	TOUCH_DIN_L()		TOUCH_CTRL_PORT->BRR = TOUCH_DIN
#define		TOUCH_DOUT_RD()		GPIO_ReadInputDataBit(TOUCH_CTRL_PORT, TOUCH_DOUT)
#define		TOUCH_INT_QUERY()	GPIO_ReadInputDataBit(TOUCH_CTRL_PORT, TOUCH_INT)

/* Exported functions ------------------------------------------------------- */
 
void touch_GetAdXY(uint *x,uint *y);

/* how to use

char str[10];
uint T_x = 3;
uint T_y = 4; 

while(1)
{
	if ( TOUCH_INT_QUERY() == 0 )
	{
		GPIO_WriteBit( GPIOA, GPIO_Pin_0, Bit_SET );
		touch_GetAdXY( &T_x, &T_y );

		LCD_DispString( "TP X Postion:    ", WHITE, BLACK, 0, 2 );
		sprintf( str, "%d", T_x );
		LCD_DispString( str, WHITE, BLACK, 13, 2 );
		 
		LCD_DispString( "TP Y Postion:    ", WHITE, BLACK, 0, 3 );
		sprintf( str, "%d", T_y );                        
		LCD_DispString( str, WHITE, BLACK, 13, 3 ); 
		   
		LCD_DrawDot( RED, (T_y)/16, T_x/12 );
	}
}
*/

#endif

/*********************** (C) COPYLEFT 2010 Leafgrass *************************/
