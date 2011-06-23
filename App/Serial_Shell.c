/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : Serial_Shell.c
* Author             : Librae
* Last Modified Date : 11/12/2010
* Description        : A simple shell based on serial COM.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
//#include "includes.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "uCOS_II.H"
#include "CPU_Init.h"
#include "LTM022A69B.h"
#include "c_lib_retarget.h"
#include "stm32f10x_it.h"
#include "platform_config.h"
#include "Serial_Shell.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
QUEUE USART_Q;
elem_t usart_buff[USART_QUEUE_SIZE];

const unsigned char *CmdWord[MAX_CMDWORD_NUM] = 
{
	"reboot",
	"led",
	NULL
};

void (*pCmdFunction[])( unsigned char pCmdParam[] ) =
{
	Cmd_reboot,		// 0# cmd call
	Cmd_led,		// 1# cmd call
	NULL
};

unsigned char Cmd_buff[CMD_BUFF_SIZE];
unsigned char zCmdWord[CMD_BUFF_SIZE];
unsigned char zCmdParam[CMD_BUFF_SIZE];
unsigned char *pCmd = NULL;

//char video_data[100][75] = {0};

unsigned int i;
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

void TaskShell(void * pdata)
{
	pdata = pdata;
	pCmd = Cmd_buff;
	
	printf( "Starting Serial Shell..." );
	// ===== shell initialization
	memset( Cmd_buff, 0, sizeof(Cmd_buff) );
	memset( zCmdWord, 0, sizeof(zCmdWord) );
	memset( zCmdParam, 0, sizeof(zCmdParam) );
	QueueCreate( &USART_Q, usart_buff, USART_QUEUE_SIZE );
	// ===== shell initialization

	printf("\r\n\r\n");
	printf( SHELL_NODE );

	for(;;)
	{
		Serial_Shell();
		OSTimeDlyHMSM(0,0,0,20);
	}
}

void Serial_Shell()
{
	int ret_cmd_num;

    if( flag_usart1Rx )		// have finished receiving usart data
	{
		flag_usart1Rx = 0;
		//LCD_DispChar( usart1_data_r, WHITE, BLACK, 15, 8 );	//Move this line into general task from ISR, avoid some messy code on LCD
		//OSTaskCreate(TaskGeneral, (void * )0, (OS_STK *)&TASK_GENERAL_STK[GENERAL_STK_SIZE-1], GENERAL_TASK_Prio);

		switch( usart1_data_r )
		{
			case '\r':
				QueueOutput(&USART_Q);	// delete the '\r' character
				printf( "\r\n" );
				*pCmd = '\0';	// add an end character to the rear of cmd string
				pCmd = Cmd_buff;

				ret_cmd_num = FindCmd( Cmd_buff, CmdWord, zCmdWord, zCmdParam );	
				if( ret_cmd_num >= 0 && ret_cmd_num < USED_CMDWORD_NUM )
				{
					pCmdFunction[ret_cmd_num]( zCmdParam );
				}
				else
				{
					if( Cmd_buff[0] != '\0' )	// if this is not triggered by a single '\r', do print.
					{
						printf( "Command '%s' not found\r\n", Cmd_buff );
					}
				}

				QueueClear(&USART_Q);
				printf( SHELL_NODE );
				break;

			case '\b':
				printf( "\b \b" );	//backspace
				QueueOutput(&USART_Q);	// delete the '\b' character
				pCmd--;
				*pCmd = '\0';	// add a end flag to the rear of cmd string
				break;

			default:
				//printf( "%c", usart1_data_r );
				for( i = 0; i < trigger_usart1_cnt; i++ )
				{
					printf( "%c", *pCmd++ = QueueOutput(&USART_Q) );
				}
				break;
		}
		trigger_usart1_cnt = 0;
	}
}

void Cmd_Init(void)
{
//	pCmdFunction[0] = Cmd_reboot;
//	pCmdFunction[1] = Cmd_led;
}

// return the ID number of Command Word
int FindCmd( unsigned char Cmd_buff[], const unsigned char *CmdWord[], unsigned char zCmdWord[], unsigned char zCmdParam[] )
{
	int cmdnum = -1;
	unsigned int i;

	unsigned char *pCmd_buff = Cmd_buff;
	const unsigned char *p = CmdWord[0];
	unsigned char *pzCmdWord = zCmdWord;
	unsigned char *pzCmdParam = zCmdParam;

	if( pCmd_buff == NULL )
	{
		return -1;
	}

	while( *pCmd_buff != ' ' && *pCmd_buff != '\0' )
	{
		*pzCmdWord++ = *pCmd_buff++;
	}
	*pzCmdWord = '\0';
	if ( pCmd_buff != '\0' )
	{
		pCmd_buff++;
		while( *pCmd_buff != '\0' )
		{
			*pzCmdParam++ = *pCmd_buff++;
		}
	}
	*pzCmdParam = '\0';

//	printf( "zCmdWord: %X *%s*\r\n", zCmdWord, zCmdWord );
//	printf( "zCmdParam: %X *%s*\r\n", zCmdParam, zCmdParam );

	i = 0;
	while( p != NULL )
	{
		if( strcmp( zCmdWord, p ) == 0 )
		{
			cmdnum = i;
			return cmdnum;
		}
		i++;
		p = CmdWord[i];
	}
	return -1;
}

void Cmd_reboot( unsigned char pCmdParam[] )
{
	unsigned int time;
	if( pCmdParam[0] != '\0' )	// if without parameter, reboot right now
	{
		time = atoi(pCmdParam);
		if( time > 0 && time <= 65535 )
		{
			printf("System will reboot after %.3fs ...\r\n", (float)time/1000 );
			Wait_ms(time);
		}
		else
		{
			printf("USAGE: reboot Xms(0<X<65535, to reboot system after X ms)\r\n");
			printf("    OR reboot (without any patameter, to reboot system right now)\r\n");
			return;
		}
	}
	printf("System rebooting ...\r\n");
	SCB->AIRCR = 0X05FA0000 | SCB_AIRCR_SYSRESETREQ;	//Software reset
}

void Cmd_led( unsigned char pCmdParam[] )
{
	if( strcmp( pCmdParam, "on" ) == 0 )
	{
		LED2_ON();
	}
	else if( strcmp( pCmdParam, "off" ) == 0 )
	{
		LED2_OFF();
	}
	else
	{
		printf("USAGE: led on|off\r\n");
		return;
	}
}


/*********************** (C) COPYLEFT 2010 Leafgrass *************************/
