/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : Serial_Shell.h
* Author             : Librae
* Last Modified Date : 11/12/2010
* Description        : Serial_Shell

******************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __SERIAL_SHELL_H__
#define __SERIAL_SHELL_H__

/* Includes ---------------------------------------------------------------- */
#include "DataStructure.h"

/* Exported types ---------------------------------------------------------- */
/* Exported constants ------------------------------------------------------ */
#define SHELL_NODE	"$ "

#define USART_QUEUE_SIZE	32
#define CMD_BUFF_SIZE	  	32

#define MAX_CMDWORD_NUM		8
#define USED_CMDWORD_NUM	2

/* Exported macro ---------------------------------------------------------- */
/* Exported variables ------------------------------------------------------ */
extern QUEUE USART_Q;
extern unsigned char usart_buff[USART_QUEUE_SIZE];

extern void (*pCmdFunction[])( unsigned char pCmdParam[] );	// declare pCmdFunction as an array of pointers to function withouot returnning
extern const unsigned char *CmdWord[MAX_CMDWORD_NUM];		// The constant Command Word pointer array
extern unsigned char Cmd_buff[CMD_BUFF_SIZE];				// The whole string picked from uart buffer queue
extern unsigned char zCmdWord[CMD_BUFF_SIZE];				// string of command word contained in Cmd_buff[]
extern unsigned char zCmdParam[CMD_BUFF_SIZE];				// string of parameter(s) info contained in Cmd_buff[]
extern unsigned char *pCmd;		// for picking command out from the usart queue

/* Exported functions ------------------------------------------------------ */
void Serial_Shell(void);
void Cmd_Init(void);

int FindCmd(
				unsigned char Cmd_buff[], 			// The whole string picked from usart buffer queue
				const unsigned char *CmdWord[], 	// The constant Command Word pointer array
				unsigned char zCmdWord[], 			// string of command word contained in Cmd_buff[]
				unsigned char zCmdParam[] 			// string of parameter(s) info contained in Cmd_buff[]
				);

void Cmd_led( unsigned char zCmdParam[] );
void Cmd_reboot( unsigned char zCmdParam[] );


#endif	/* __SERIAL_SHELL_H__ */

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
