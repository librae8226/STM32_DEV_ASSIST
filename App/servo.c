/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : servo.c
* Author             : Librae
* Last Modified Date : 08/17/2010
* Description        : This file provides the definition of
						servo related things.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
///#include "includes.h"
#include <stdio.h>
#include "stm32f10x.h"
#include "os_cpu.h"
#include "CPU_Init.h"
#include "LTM022A69B.h"
#include "servo.h"
#include "c_lib_retarget.h"
#include "stm32f10x_it.h"
#include "platform_config.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

void Servo_SetAngle( unsigned int angle )
{
	unsigned int period;
	unsigned int pulse;
	unsigned int time_us;

	angle = angle > RIGHTLIMIT ? RIGHTLIMIT : angle;
	angle = angle < LEFTLIMIT ? LEFTLIMIT : angle;
	time_us = angle * 11 + 500;

	// 20ms period, for servo.
	period = 720 - 1;	/* TIM_Period = TIM3 ARR Register */
	pulse = (period * time_us) / 20000;	/* 改变占空比 */
	TIM3 -> CCR3 = pulse;
	TIM3 -> CCR4 = pulse;
}

void Servo_Test( void )
{
#ifdef USE_STM32_DQ_128K_64P
	unsigned int angle = SERVOMID;
	char str[10] = {0};
	//test servo
	printf( "servo test\n\r" );
	LCD_DispString( "servo test", YELLOW, BLACK, 0, 4 );
	LCD_DispString( "angle : ", WHITE, BLACK, 0, 6 );
	LCD_DispString( "duty time : ", WHITE, BLACK, 0, 7 );
	LCD_DispString( "us", WHITE, BLACK, 17, 7 );
	LCD_DispString( "uart received: ", WHITE, BLACK, 0, 8 );
	for(;;)
	{
        if( ISKEY1_PRESSED() )
        {
            if( angle > LEFTLIMIT )
            {
                angle--;
            }
			printf("angle: %d\r\n", angle);
        }
        else if( ISKEY2_PRESSED() )
        {
            if( angle < RIGHTLIMIT )
            { 
                angle++;
            }
			printf("angle: %d\r\n", angle);
        }
        else;

        Servo_SetAngle( angle );

		sprintf( str, "%d", angle);
		if( angle < 100 )
		{
			LCD_DispChar( ' ', WHITE, BLACK, 8, 6 );
			LCD_DispString( (u8*)str, WHITE, BLACK, 9, 6 );
		}
		else
		{
			LCD_DispString( (u8*)str, WHITE, BLACK, 8, 6 );
		}

		sprintf( str, "%d", (angle*11+500) );
		if( (angle*11+500) < 1000 )
		{
			LCD_DispChar( ' ', WHITE, BLACK, 12, 7 );
			LCD_DispString( (u8*)str, WHITE, BLACK, 13, 7 );
		}
		else
		{
			LCD_DispString( (u8*)str, WHITE, BLACK, 12, 7 );
		}
        if( flag_usart1Rx )
		{
			flag_usart1Rx = 0;
			LCD_DispChar( usart1_data_r, WHITE, BLACK, 15, 8 );	//Move this line into general task from ISR, avoid some messy code on LCD
			LCD_DispString( USART1_DMA_RxBuff, WHITE, BLACK, 0, 9 );
			//printf( "The character you typed is : " );
			//printf( "%c\r\n", usart1_data_r );
		}
	}
#endif
}

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
