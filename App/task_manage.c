/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : task_manage.c
* Author             : Librae
* Last Modified Date : 11/10/2010
* Description        : Tasks management.

******************************************************************************/

#define VERSION		"v0.0.2"

/* Includes -----------------------------------------------------------------*/
//#include "includes.h"
#include <stdio.h>
#include "uCOS_II.H"
#include "c_lib_retarget.h"
#include "CPU_Init.h"
#include "platform_config.h"
#include "NRF24L01.h"
#include "LTM022A69B.h"
#include "LCD_Lib.h"
#include "general_task.h"
#include "task_manage.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/

//define task stack
OS_STK  TASK_START_STK[START_STK_SIZE];
OS_STK  TASK_LED_STK[LED_STK_SIZE];
OS_STK  TASK_SHELL_STK[SHELL_STK_SIZE];
OS_STK  TASK_GENERAL_STK[GENERAL_STK_SIZE];

//unsigned int uart_data_r;

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/******************************************************************************
* Function Name  : TaskStart
* Description    : For system and other tasks' initialization.
* Input          : *pdata, parameter may be useful
* Output         : None
* Return         : None
******************************************************************************/
void TaskStart(void * pdata)
{
	u32 i = 0;
	pdata = pdata;

	OS_ENTER_CRITICAL();

	//====================      initialization      ====================

	CPU_Init();
	Wait_ms(5);
	LCD_Init();
//	NRF24L01_Init();

	//====================      booting signal      ====================
	
	//led flashes -> sign of system reset ok
	for( i = 0; i < 6; i++ )
	{
		LEDUSB_TOGGLE();
		Wait_ms(50);
	}

	// Booting...
	LCD_Clear(BLACK);
	putchar(0x0C);	//clear screen
	LCD_DispImage( ucosii, 20, 110, 200, 45);
	printf("                       _\r\n");
	printf("     _                / /\r\n");
	printf("    | |	   ___  __ _ _| |_ __ _ _  __  __ _   _   _\r\n");
	printf("    | |	  / _ \\/ _` |_   _/ _` | \\/ _)/ _` | / / / /\r\n");
	printf("    | |_ _  __%c %c_| | | |  %c_| | | | %c %c_| | \\ \\ \\ \\\r\n", 0x28, 0x28, 0x28, 0x28, 0x28);
	printf("    |_ _ _\\___|\\__,_| | | \\__, / | |  \\__,_| /_/ /_/\r\n");
	printf("                      /_/ \\_ _/\r\n");
	printf( "\r\n" );
	printf( "uCOS-II version 2.52\r\n" );
	LCD_DispString( "version 2.52", WHITE, BLACK, 8, 10 );
	printf( "Developing Assistant " ); printf( VERSION );
	printf( "\r\n\r\n" );

	
	init_NRF24L01();
	NRF24_RX_Mode();
	printf("NRF24L01 configure RX_Mode ... \t[ok]\r\n");
	printf( "\r\n" );

	LCD_DispString( "Booting", WHITE, BLACK, 11, 17 );
	printf( "Booting" );
	for( i = 0; i < 30; i++ )
	{
		LCD_DispChar( '.', WHITE, BLACK, i, 18 );
		putchar( '.' );
		Wait_ms(10);
	}
	Wait_ms(500);
	LCD_Clear(0);
	printf( "\r\n\r\n" );

	//====================     tasks operation      ====================
	OSTaskCreate(TaskLed, (void * )0, (OS_STK *)&TASK_LED_STK[LED_STK_SIZE-1], LED_TASK_Prio);
//	OSTaskCreate(TaskShell, (void * )0, (OS_STK *)&TASK_SHELL_STK[SHELL_STK_SIZE-1], SHELL_TASK_Prio);
	OSTaskCreate(TaskGeneral, (void * )0, (OS_STK *)&TASK_GENERAL_STK[GENERAL_STK_SIZE-1], GENERAL_TASK_Prio);

	//OSTaskDel(START_TASK_Prio);
	OSTaskSuspend(START_TASK_Prio);	//suspend but not delete

	OS_EXIT_CRITICAL();
}

void TaskLed(void * pdata)
{
	pdata = pdata;
	
	//LCD_Test();
	for(;;)
	{

		//LCD_Test();
		//Servo_Test();
		//GPS_Test();
		//General();
		LED3_TOGGLE();
		OSTimeDlyHMSM(0,0,1,0);
		//LCD_Clear(0);
	}
}

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
