/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : gps.c
* Author             : Librae
* Last Modified Date : 08/19/2010
* Description        : This file provides the definition of
						GPS related functions and variables.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
//#include "includes.h"
#include <stdio.h>
#include "stm32f10x.h"
#include "os_cpu.h"
#include "LTM022A69B.h"
#include "stm32f10x_it.h"
#include "platform_config.h"
#include "gps.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/

unsigned char GPS_keyword[5];
unsigned char GPS_data_buff[80];
unsigned char *p_GPS_data_buff = GPS_data_buff;
unsigned char *p_GPS_keyword = GPS_keyword;

t_GPS_data GPS_data;

BOOLEAN flag_GPS_data_updated = 1;
BOOLEAN flag_GPS_data_processed = 0;

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

void GPS_Test(void)
{
	LCD_DispString( "GPS Test", YELLOW, BLACK, 11, 1 );
	LCD_DispString( "Keyword: ", CYAN, BLACK, 0, 6 );
	LCD_DispString( "Latitude: ", CYAN, BLACK, 0, 7 );
	LCD_DispString( "Longitude: ", CYAN, BLACK, 0, 8 );

	LCD_DispString( "Altitude: ", CYAN, BLACK, 0, 10 );
	LCD_DispString( "Status: ", CYAN, BLACK, 0, 11 );
	LCD_DispString( "UTC Time: ", CYAN, BLACK, 0, 12 );
	LCD_DispString( "UTC Date: ", CYAN, BLACK, 0, 13 );

	LCD_DispString( "Satellites used: ", CYAN, BLACK, 0, 15 );
	LCD_DispString( "Satellites in view: ", CYAN, BLACK, 0, 16 );
	LCD_DispString( "Satellites ID: ", CYAN, BLACK, 0, 17 );

	for(;;)
	{	
		if( flag_GPS_data_updated )
		{
			LCD_Clear_SelectArea( BLACK, 0, 48, 240, 48 );
			LCD_DispString( GPS_data_buff, WHITE, BLACK, 0, 3 );
			LCD_DispString( GPS_keyword, WHITE, BLACK, 9, 6 );
			LCD_DispChar( GPS_data.lat_ew, WHITE, BLACK, 10, 7 );
			LCD_DispString( GPS_data.latitude, WHITE, BLACK, 12, 7 );
			LCD_DispChar( GPS_data.long_ns, WHITE, BLACK, 11, 8 );
			LCD_DispString( GPS_data.longitude, WHITE, BLACK, 13, 8 );

			LCD_DispString( GPS_data.altitude, WHITE, BLACK, 10, 10 );
			LCD_DispChar( GPS_data.status, RED, BLACK, 8, 11 );
			LCD_DispString( GPS_data.time, WHITE, BLACK, 10, 12 );
			LCD_DispString( GPS_data.date, WHITE, BLACK, 10, 13 );
			
			LCD_DispString( GPS_data.satellites, WHITE, BLACK, 17, 15 );
			LCD_DispString( GPS_data.satellites_inview, WHITE, BLACK, 20, 16 );
			LCD_DispString( GPS_data.satellites_id, WHITE, BLACK, 15, 17 );

			flag_GPS_data_updated = 0;
			//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		}
		if( flag_usart3Rx )
		{
			flag_usart3Rx = 0;
		}
	}
}

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
