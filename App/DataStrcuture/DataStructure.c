/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : DataStructure.c
* Author             : Librae
* Last Modified Date : 11/15/2010
* Description        : Provide micro data structure.

The QUEUE operation partly refer to:
********************************
  基于队列的Mega8 UART通信驱动程序
  文件名:uart.c
  编译：WinAVR-20070122

  硬件：CA-M8X
  时钟：外部4MHz

  芯艺设计室 2004-2007  版权所有 
  转载请保留本注释在内的全部内容
  WEB: http://www.chipart.cn
  Email: changfutong@sina.com
*******************************

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
//#include "includes.h"
#include "DataStructure.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

// ========== QUEUE ==========
// Insert a data unit to rear
Status QueueInput( PQUEUE Q, elem_t dat )
{
	if( Q->buff_used < Q->buff_size )
	{
		*(Q->pBuffer + Q->rear) = dat;	//insert
		Q->rear = ( Q->rear + 1 ) % ( Q->buff_size );	//adjust rear position
		// note: the % operation makes queue performs as a circular queue
		Q->buff_used++;	//adjust number of data(this operation shuold not be interrupted)
		return OK;
	}
	else
	{
		return OVERFLOW;
	}
}

// Delete a data unit from front, and return it
elem_t QueueOutput( PQUEUE Q )
{
	unsigned int ret = 0;

	if ( Q->buff_used > 0 )
	{
		ret = *(Q->pBuffer + Q->front);	//Get data
		Q->front = ( Q->front + 1 ) % ( Q->buff_size );	//adjust front position
		// note: the % operation makes queue performs as a circular queue
		Q->buff_used--;
	}
	return ret;
}

// Get the number of data
unsigned int QueueGetDataCount( PQUEUE Q )
{
	return Q->buff_used;
}

// Clear queue, this operation should not be interrupted
Status QueueClear( PQUEUE Q )
{
	Q->rear = 0;
	Q->front = 0;
	Q->buff_used = 0;
	return OK;
}

// Creat and initialize a queue
Status QueueCreate( PQUEUE Q, elem_t *buffer, unsigned int buff_size )
{
	Q->pBuffer = buffer;
	Q->buff_size = buff_size;
	QueueClear(Q);
	return OK;
}

// Return the element in relative position of front
elem_t GetElem(PQUEUE Q, unsigned int rel_position)
{
	return *(Q->pBuffer + (Q->front + rel_position) % Q->buff_size);
}

/*********************** (C) COPYLEFT 2010 Leafgrass *************************/
