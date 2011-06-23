/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : LTM022A69B.c
* Author             : Librae
* Last Modified Date : 08/12/2010
* Description        : This file provides the 
						LTM022A69B LCD related functions' declaration.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
//#include "includes.h"
#include <stdlib.h>
#include "CPU_Init.h"
#include "LCD_lib.h"
#include "LTM022A69B.h"

/* Private typedef ----------------------------------------------------------*/

/* Private define -----------------------------------------------------------*/

/* Private macro ------------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

/* Private functions --------------------------------------------------------*/

//=============================================================================
//							LCD Basic Functions
//=============================================================================
/******************************************************************************
* Function Name  : LCD_WR_REG
* Description    : write LCD register
* Input          : index
* Output         : None
* Return         : None
******************************************************************************/
void LCD_WR_REG(unsigned int index)
{
#if !USESPI
	unsigned char i;
#endif
	
	LCD_RS_L();
	LCD_CS_L();

#if USESPI

    while(SPI_I2S_GetFlagStatus(SPI_LCD, SPI_I2S_FLAG_BSY) != RESET);
	{
	}

	SPI_I2S_SendData( SPI_LCD, index );	//Send SPI_LCD data

    while(SPI_I2S_GetFlagStatus(SPI_LCD, SPI_I2S_FLAG_BSY) != RESET)
	{
	}


#else
	for(i=0;i<8;i++)
	{
		LCD_SCL_L();
		LCD_SI_L();
		LCD_SCL_H();
	}
	for(i=0;i<8;i++)
	{
		LCD_SCL_L();
		if(index&0x80)
		{
			LCD_SI_H();
		}
		else
		{
			LCD_SI_L();
		}
		index=index<<1;
		LCD_SCL_H();
	}
#endif

	LCD_CS_H();
}

/******************************************************************************
* Function Name  : LCD_WR_CMD
* Description    : send command to LCD
* Input          : index, data
* Output         : None
* Return         : None
******************************************************************************/
void LCD_WR_CMD(unsigned int index,unsigned int data)
{
#if !USESPI
	unsigned char i;
#endif
	
	//select command register
	LCD_RS_L();
	LCD_CS_L();

#if USESPI
    while(SPI_I2S_GetFlagStatus(SPI_LCD, SPI_I2S_FLAG_BSY) != RESET)
	{
	}

	SPI_I2S_SendData( SPI_LCD, index );	//Send SPI_LCD data

    while(SPI_I2S_GetFlagStatus(SPI_LCD, SPI_I2S_FLAG_BSY) != RESET)
	{
	}

#else
	for(i=0;i<8;i++)
	{
		LCD_SCL_L();
		LCD_SI_L();
		LCD_SCL_H();
	}
	for(i=0;i<8;i++)
	{
		LCD_SCL_L();
		if( index & 0x80 )
		{
			LCD_SI_H();
		}
		else
		{
			LCD_SI_L();
		}
		index=index<<1;
		LCD_SCL_H();
	}
#endif
	
	LCD_CS_H();

	//send data
	LCD_RS_H();
	LCD_CS_L();

#if USESPI
    while(SPI_I2S_GetFlagStatus(SPI_LCD, SPI_I2S_FLAG_BSY) != RESET)
	{
	}

	SPI_I2S_SendData( SPI_LCD, data );	//Send SPI_LCD data

    while(SPI_I2S_GetFlagStatus(SPI_LCD, SPI_I2S_FLAG_BSY) != RESET)
	{
	}

#else
    for(i=0;i<16;i++)
	{
		LCD_SCL_L();
		if( data & 0x8000 )
		{
			LCD_SI_H();
		}
		else
		{
			LCD_SI_L();
		}
		data=data<<1;
		LCD_SCL_H();
	}
#endif

	LCD_CS_H();
}

/*******************************************************************************
* Function Name  : LCD_WR_DATA
* Description    : write data to LCD
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WR_DATA(unsigned int data)
{
#if USESPI
    while(SPI_I2S_GetFlagStatus(SPI_LCD, SPI_I2S_FLAG_BSY) != RESET)
	{
	}

	SPI_I2S_SendData( SPI_LCD, data );	//Send SPI_LCD data

    while(SPI_I2S_GetFlagStatus(SPI_LCD, SPI_I2S_FLAG_BSY) != RESET)
	{
	}

#else
	unsigned char i;
	for(i=0;i<16;i++)
	{
		LCD_SCL_L();
		if( data & 0x8000 )
		{
			LCD_SI_H();
		}
		else
		{
			LCD_SI_L();
		}
		data=data<<1;
		LCD_SCL_H();
	}
#endif
}

/******************************************************************************
* Function Name  : LCD_Init
* Description    : LCD initialization
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void LCD_Init(void)
{
	LCD_RST_L();
	Delay(1000);
	LCD_RST_H();
	Delay(1000);

	//register reset
	LCD_WR_CMD(0x0003,0x0001);	//Soft reset (reset pulse occurs by writing in "1")

	//oscillator start
	LCD_WR_CMD(0x003A,0x0001);	//Oscillator control (0:oscillator stop, 1: oscillator operation)
	Delay(100);

	//y-setting
	LCD_WR_CMD(0x0024,0x007B);	//amplitude setting
	Delay(10);
	LCD_WR_CMD(0x0025,0x003B);	//amplitude setting	
	LCD_WR_CMD(0x0026,0x0034);	//amplitude setting
	Delay(10);
	LCD_WR_CMD(0x0027,0x0004);	//amplitude setting	
	LCD_WR_CMD(0x0052,0x0025);	//circuit setting 1
	Delay(10);
	LCD_WR_CMD(0x0053,0x0033);	//circuit setting 2	
	LCD_WR_CMD(0x0061,0x001C);	//adjustment V10 positive polarity
	Delay(10);
	LCD_WR_CMD(0x0062,0x002C);	//adjustment V9 negative polarity
	LCD_WR_CMD(0x0063,0x0022);	//adjustment V34 positive polarity
	Delay(10);
	LCD_WR_CMD(0x0064,0x0027);	//adjustment V31 negative polarity
	Delay(10);
	LCD_WR_CMD(0x0065,0x0014);	//adjustment V61 negative polarity
	Delay(10);
	LCD_WR_CMD(0x0066,0x0010);	//adjustment V61 negative polarity
	
	//Basical clock for 1 line (BASECOUNT[7:0]) number specified
	LCD_WR_CMD(0x002E,0x002D);
	
	//Power supply setting
	LCD_WR_CMD(0x0019,0x0000);	//DC/DC output setting
	Delay(200);
	LCD_WR_CMD(0x001A,0x1000);	//DC/DC frequency setting
	LCD_WR_CMD(0x001B,0x0023);	//DC/DC rising setting
	LCD_WR_CMD(0x001C,0x0C01);	//Regulator voltage setting
	LCD_WR_CMD(0x001D,0x0000);	//Regulator current setting
	LCD_WR_CMD(0x001E,0x0009);	//VCOM output setting
	LCD_WR_CMD(0x001F,0x0035);	//VCOM amplitude setting	
	LCD_WR_CMD(0x0020,0x0015);	//VCOMM cencter setting	
	LCD_WR_CMD(0x0018,0x1E7B);	//DC/DC operation setting

	//windows setting
	LCD_WR_CMD(0x0008,0x0000);	//Minimum X address in window access mode
	LCD_WR_CMD(0x0009,0x00EF);	//Maximum X address in window access mode
	LCD_WR_CMD(0x000a,0x0000);	//Minimum Y address in window access mode
	LCD_WR_CMD(0x000b,0x013F);	//Maximum Y address in window access mode

	//LCD display area setting
	LCD_WR_CMD(0x0029,0x0000);	//[LCDSIZE]  X MIN. size set
	LCD_WR_CMD(0x002A,0x0000);	//[LCDSIZE]  Y MIN. size set
	LCD_WR_CMD(0x002B,0x00EF);	//[LCDSIZE]  X MAX. size set
	LCD_WR_CMD(0x002C,0x013F);	//[LCDSIZE]  Y MAX. size set	

	//Gate scan setting
	LCD_WR_CMD(0x0032,0x0002);
	
	//n line inversion line number
	LCD_WR_CMD(0x0033,0x0000);

	//Line inversion/frame inversion/interlace setting
	LCD_WR_CMD(0x0037,0x0000);
	
	//Gate scan operation setting register
	LCD_WR_CMD(0x003B,0x0001);
	
	//Color mode
	LCD_WR_CMD(0x0004,0x0000);	//GS = 0: 260-k color (64 gray scale), GS = 1: 8 color (2 gray scale)

	//RAM control register
	LCD_WR_CMD(0x0005,0x0010);	//Window access control (0: Normal access, 1: Window access)
	
	//Display setting register 2
	LCD_WR_CMD(0x0001,0x0000);

	//display setting	
	LCD_WR_CMD(0x0000,0x0000);	//display on

	Delay(20);
	
	LCD_Clear(0);
}

/******************************************************************************
* Function Name  : LCD_Clear
* Description    : clear screen
* Input          : color: refresh color
* Output         : None
* Return         : None
******************************************************************************/
void LCD_Clear(unsigned int color)
{
	unsigned int i,j;

	LCD_WR_CMD(0x08,0x00);
	LCD_WR_CMD(0x0a,0x0000);
	LCD_WR_CMD(0x09,0xEF);
	LCD_WR_CMD(0x0b,0x013F);

	LCD_WR_CMD(0x06,0x0000);
	LCD_WR_CMD(0x07,0x0000);
	
	LCD_WR_REG(0x0E);	//RAM Write index

	LCD_CS_L();
	LCD_RS_H();

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
		  LCD_WR_DATA(color);
		}
	}

	LCD_CS_H();
}

void LCD_Clear_SelectArea(unsigned int color, unsigned char x, unsigned int y, unsigned int width, unsigned height)
{
	unsigned int i, j;

	LCD_WR_CMD( 0x08, x ); 	//x start point
	LCD_WR_CMD( 0x0a, y ); 	//y start point
	LCD_WR_CMD( 0x09, x + width - 1 );	//x end point
	LCD_WR_CMD( 0x0b, y + height - 1 );	//y end point
		
	LCD_WR_CMD( 0x06, x );
	LCD_WR_CMD( 0x07, y );

	LCD_WR_REG( 0x0E );

	LCD_CS_L();
	LCD_RS_H();

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			LCD_WR_DATA( color );
		}
	}
	LCD_CS_H();
}

/******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Set cursor
* Input          : x, y
* Output         : None
* Return         : None
******************************************************************************/
void LCD_SetCursor(unsigned char x, unsigned int y)
{
  	if( (x > 320) || (y > 240) )
	{
		return;
	}
	LCD_WR_CMD( 0x06, x );
	LCD_WR_CMD( 0x07, y );
}

/******************************************************************************
* Function Name  : LCD_DispChar
* Description    : ch_asc: ascii code of data. position_x, position_y.
					color, color_bkgd.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void LCD_DispChar(unsigned char ch_asc, unsigned int color, unsigned int color_bkgd, unsigned char postion_x, unsigned char postion_y)
{
	unsigned char i, j, b;
	const unsigned char *p = 0;
	
	LCD_WR_CMD(0x08,postion_x*8); 	//x start point
	LCD_WR_CMD(0x0a,postion_y*16); 	//y start point
	LCD_WR_CMD(0x09,postion_x*8+7);	//x end point
	LCD_WR_CMD(0x0b,postion_y*16+15);	//y end point

	LCD_WR_CMD(0x06,postion_x*8);	//RAM X address(0 ~ FF, actually 0 ~ EF)	
	LCD_WR_CMD(0x07,postion_y*16);	//RAM Y address(0 ~ 1FF, actually 0 ~ 13F)
	
	LCD_WR_REG(0x0E);	//RAM Write index(prepare to write data)

	LCD_CS_L();
	LCD_RS_H();

	p = ascii;
	p += ch_asc*16;
	for(j=0;j<16;j++)
	{
		b=*(p+j);
		for(i=0;i<8;i++)
		{
			if( b & 0x80 )
			{
				LCD_WR_DATA(color);
			}
			else
			{
				LCD_WR_DATA(color_bkgd);
			}
			b=b<<1;			
		}	
	}

	LCD_CS_H();
}

/******************************************************************************
* Function Name  : LCD_DispString
* Description    : *s: address of string data. 
					x: the xth row(0~30).
					y: the yth column(0~20).
					color, color_bkgd.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void LCD_DispString( unsigned char *s, 
						unsigned int color, 
						unsigned int color_bkgd, 
						unsigned char x, 
						unsigned char y )
{
	while (*s) 
	{ 
		LCD_DispChar( *s, color, color_bkgd, x, y);
		if(++x>=30)
		{
			x=0;
			if(++y>=20)
			{
				y=0;
			}
		}
		s++;
    }
}

/******************************************************************************
* Function Name  : 汉字显示
* Description    : 16x16点阵的汉字显示函数
* Input          : gb:汉字对应字模中的位置，x:显示位置第几列，y:显示位置第几行
					color, color_bkgd.  
* Output         : None
* Return         : None
******************************************************************************/
/*显示方法（年月日）
for( i = 2; i < 8; i++ )
{
	LCD_DispGB2312( i-2, (i-1)*2, 0 );
}
*/
void LCD_DispGB2312( unsigned char gb, 
						unsigned int color, 
						unsigned int color_bkgd, 
						unsigned char postion_x, 
						unsigned char postion_y )
{
	unsigned char i,j,b;
	unsigned char *p;
	
	LCD_WR_CMD(0x08,postion_x*16); 	//x start point
	LCD_WR_CMD(0x0a,postion_y*16); 	//y start point
	LCD_WR_CMD(0x09,postion_x*16+15);	//x end point
	LCD_WR_CMD(0x0b,postion_y*16+15);	//y end point
	LCD_WR_CMD(0x06,postion_x*16);	
	LCD_WR_CMD(0x07,postion_y*16);

	LCD_WR_REG(0x0E);	//RAM Write index

	LCD_CS_L();
	LCD_RS_H();

	p = (unsigned char *)GB2312;
	p += gb*32;
	for(j=0;j<32;j++)
	{
		b=*(p+j);
		for(i=0;i<8;i++)
		{
			if(b&0x80)
			{
				LCD_WR_DATA(color);
			}
			else
			{
				LCD_WR_DATA(color_bkgd);
			}
			b=b<<1;
			
		}	
	}
	LCD_CS_H();
}

/******************************************************************************
* Function Name  : LCD_DispImage
* Description    : Draw image
* Input          : x, y: image start at x, y. width, length, *img.
* Output         : None
* Return         : None
******************************************************************************/
void LCD_DispImage( const unsigned char *img, 
					unsigned char x, 
					unsigned int y, 
					unsigned int width, 
					unsigned height )
{
	unsigned int i, j;
	unsigned int data16;


	LCD_WR_CMD( 0x08, x ); 	//x start point
	LCD_WR_CMD( 0x0a, y ); 	//y start point
	LCD_WR_CMD( 0x09, x + width - 1 );	//x end point
	LCD_WR_CMD( 0x0b, y + height - 1 );	//y end point
		
	LCD_WR_CMD( 0x06, x );
	LCD_WR_CMD( 0x07, y );

	LCD_WR_REG( 0x0E );

	LCD_CS_L();
	LCD_RS_H();

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			//Be carful of MCU type, big endian or little endian
			//little endian
			data16 = ( *(img + 1) << 8 ) | (*img);
			LCD_WR_DATA( data16 );
			img += 2;
		}
	}
	LCD_CS_H();
}

//=============================================================================
//							Application Functions
//=============================================================================

/******************************************************************************
* Function Name  : LCD_test
* Description    : test
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void LCD_Test(void)
{
	unsigned int temp, num;
	unsigned int i;
	unsigned char n;

	LCD_Clear(0);
	LCD_DispString( "LTM022A69B", WHITE, BLACK, 0, 0 );
	LCD_DispString( "test starting...", WHITE, BLACK, 0, 1 );
	Wait_ms(1000);
	
	LCD_WR_CMD(0x08,0x00);		//x start point
	LCD_WR_CMD(0x0a,0x0000);	//y start point
	LCD_WR_CMD(0x09,0xEF);		//x end point
	LCD_WR_CMD(0x0b,0x013F);	//y end point

	LCD_WR_CMD(0x06,0x0000);
	LCD_WR_CMD(0x07,0x0000);
	LCD_WR_REG(0x0E);	//prepare to send data
	//dispaly color bar
	for(n=0;n<8;n++)
	{
		LCD_CS_L();
		LCD_RS_H();
		temp=color[n];
		for(num=40*240;num>0;num--)
		{
			LCD_WR_DATA(temp);
		}
	}

	Wait_ms(1000);

	//display pure color
	for(n=0;n<8;n++)
	{
		LCD_WR_CMD(0x08,0x00);
		LCD_WR_CMD(0x0a,0x0000);
		LCD_WR_CMD(0x09,0xEF);
		LCD_WR_CMD(0x0b,0x013F);

		LCD_WR_CMD(0x06,0x0000);
		LCD_WR_CMD(0x07,0x0000);
		
		LCD_WR_REG(0x0E);

		LCD_CS_L();
		LCD_RS_H();

	    temp=color[n];
		for(i=0;i<240;i++)
		{
			for(num=0;num<320;num++)
			{
		  		LCD_WR_DATA(temp);
			}
		}
	}
	
	
	LCD_CS_H();
	Wait_ms(1000);

	LCD_Clear(0);

	LCD_DispString( "test ok!", WHITE, BLACK, 0, 0 );
	Wait_ms(1000);
	LCD_Clear(0);
}

/******************************************************************************
* Function Name  : LCD_DrawDot
* Description    : Draw
* Input          : color, x, y
* Output         : None
* Return         : None
******************************************************************************/
void LCD_DrawDot(unsigned int color, unsigned char x, unsigned int y)
{
//	LCD_WR_CMD( 0x08, 0x00 );
//	LCD_WR_CMD( 0x0a, 0x0000 );
//	LCD_WR_CMD( 0x09, 0xEF );
//	LCD_WR_CMD( 0x0b, 0x013F );

	LCD_WR_CMD( 0x06, x );
	LCD_WR_CMD( 0x07, y );
		
	LCD_WR_CMD( 0x0E, color );
}

/******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : BresenhamLine
* Input          : c, x1, y1, x2, y2
* Output         : None
* Return         : None
******************************************************************************/
int LCD_DrawLine(int c, int x1, int y1, int x2, int y2)
{
	int dx , dy;
	int tx , ty;
	int inc1 , inc2;
	int d , iTag;
	int x , y;
	LCD_DrawDot( c , x1 , y1 );
	if( x1 == x2 && y1 == y2 )	/*如果两点重合，结束后面的动作*/
	{
		return 1;
	}
	iTag = 0;
	dx = abs ( x2 - x1 );
	dy = abs ( y2 - y1 );
	if( dx < dy )	/*如果dy为计长方向，则交换纵横坐标*/
	{
		iTag = 1 ;
		Swap ( &x1, &y1 );
		Swap ( &x2, &y2 );
		Swap ( &dx, &dy );
	}
	tx = ( x2 - x1 ) > 0 ? 1 : -1;	   /*确定是增1还是减1*/
	ty = ( y2 - y1 ) > 0 ? 1 : -1;
	x = x1;
	y = y1;
	inc1 = 2 * dy;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while( x != x2 )	 /*循环画点*/
	{
		if( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if( iTag )
		{
			LCD_DrawDot ( c, y, x ) ;
		}
		else
		{
			LCD_DrawDot ( c, x, y ) ;
		}
		x += tx ;
	}
	return 0;
}

void Swap(int *a , int *b)	//for BresenhamLine
{
	int tmp;
	tmp = *a ;
	*a = *b ;
	*b = tmp ;
}

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
