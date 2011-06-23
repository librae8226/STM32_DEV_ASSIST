/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : task_manage.h
* Author             : Librae
* Last Modified Date : 08/15/2010
* Description        : Tasks management.

******************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __TASK_MANAGE_H__
#define __TASK_MANAGE_H__

/* Includes ---------------------------------------------------------------- */
/* Exported types ---------------------------------------------------------- */
/* Exported constants ------------------------------------------------------ */

// define copacity of task stack
#define START_STK_SIZE		128
#define LED_STK_SIZE		64
#define SHELL_STK_SIZE		512
#define GENERAL_STK_SIZE	512

//define task priority
#define START_TASK_Prio     10
#define LED_TASK_Prio       9
#define SHELL_TASK_Prio		8
#define GENERAL_TASK_Prio   4

/* Exported macro ---------------------------------------------------------- */
/* Exported variables ------------------------------------------------------ */

//∂®“Â OS EVENT ECB//
//extern OS_EVENT   *ComSem;
//extern OS_EVENT   *task1;
//extern OS_EVENT   *task2;
//extern OS_EVENT   *task3;

//define task stack
extern OS_STK  TASK_START_STK[START_STK_SIZE];
extern OS_STK  TASK_LED_STK[LED_STK_SIZE];
extern OS_STK  TASK_SHELL_STK[SHELL_STK_SIZE];
extern OS_STK  TASK_GENERAL_STK[GENERAL_STK_SIZE];

/* Exported functions ------------------------------------------------------ */

void TaskStart(void *pdata);
void TaskLed(void *pdata);
void TaskShell(void *pdata);
void TaskGeneral(void *pdata);

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
