/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : DataStructure.h
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

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __DATASTRUCTURE_H__
#define __DATASTRUCTURE_H__

/* Includes ---------------------------------------------------------------- */
/* Exported constants ------------------------------------------------------ */
#define TRUE        1
#define FALSE       0
#define OK          0
#define ERROR       1
#define INFISIBLE   -1
#define OVERFLOW    -2

//#define QUEUE_SIZE	64

/* Exported types ---------------------------------------------------------- */
typedef signed char Status;
typedef unsigned char elem_t;

typedef struct QUEUE_STRUCT
{
	unsigned int rear;
	unsigned int front;
	unsigned int buff_size;
	unsigned int buff_used;
	elem_t *pBuffer;
}QUEUE, *PQUEUE;

/* Exported macro ---------------------------------------------------------- */
/* Exported variables ------------------------------------------------------ */
/* Exported functions ------------------------------------------------------ */

// ========== QUEUE ==========
Status QueueInput( PQUEUE Q, elem_t dat );
elem_t QueueOutput( PQUEUE Q );
unsigned int QueueGetDataCount( PQUEUE Q );
Status QueueClear( PQUEUE Q );
Status QueueCreate( PQUEUE Q, elem_t *buffer, unsigned int buff_size );
elem_t GetElem(PQUEUE Q, unsigned int rel_position);

#endif	/* __DATASTRUCTURE_H__ */

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
