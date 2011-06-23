/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : c_lib_retarget.c
* Author             : Librae
* Last Modified Date : 08/15/2010
* Description        : Retarget original C library.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
//#include "includes.h"
#include <stdio.h>
#include "stm32f10x.h"
#include "c_lib_retarget.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
#define USARTx	USART1

/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/*******************************************************************************
* Function Name  : PUTCHAR_PROTOTYPE
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
PUTCHAR_PROTOTYPE
{
	// Implementation of fputc
	USART_SendData(USARTx, (u8)ch);

	// Loop until the end of transmission
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/*******************************************************************************
* Function Name  : GETCHAR_PROTOTYPE
* Description    : Retargets the C library scanf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*
int fgetc(FILE *f)
{
	// Implementation of fgetc
	// Loop until the having received data
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
	{}

	return (int)USART_ReceiveData(USART1);
}
*/

/*********************** (C) COPYLEFT 2010 Leafgrass *************************/
