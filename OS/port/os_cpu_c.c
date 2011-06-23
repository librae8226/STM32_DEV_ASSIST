/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name		: os_cpu_c.c 
* Author		: Librae 
* Date			: 08/10/2010
* Description	: ��COS-II��STM32�ϵ���ֲ����C���Բ��֣�
*				  ���������ջ��ʼ������͹��Ӻ�����

******************************************************************************/

#define  OS_CPU_GLOBALS
//#include "includes.h"
#include "uCOS_II.H"

/******************************************************************************
** ��������: OSTaskStkInit
** ��������: �����ջ��ʼ�����룬����������ʧ�ܻ�ʹϵͳ����
** �䡡��: task  : ����ʼִ�еĵ�ַ
**         pdata �����ݸ�����Ĳ���
**         ptos  ������Ķ�ջ��ʼλ��
**         opt   �����Ӳ�������ǰ�汾���ڱ��������ã���������μ�OSTaskCreateExt()��opt����
** �䡡��: ջ��ָ��λ��
** ȫ�ֱ���:
** ����ģ��: 

******************************************************************************/

OS_STK *OSTaskStkInit (void  (*task)(void  *parg), void  *parg, OS_STK  *ptos, INT16U  opt)
{
    OS_STK *stk;


    (void)opt;                                        /*  'opt' is not used, prevent  */
                                                      /*  warning û����'opt'��       */
                                                      /*  ��ֹ���뾯��                */
                                                                            
    stk = ptos;                                       /*  Load stack pointer          */
                                                      /*  װ�ض�ջָ��                */                                    

                                                      /*  Registers stacked as if     */
                                                      /*  auto-saved on exception     */             
	                                                  /*  ģ����쳣���Զ��ѼĴ���ѹջ*/

    *(stk) = (INT32U)0x01000000L;                     /*  xPSR                        */ 
    *(--stk) = (INT32U)task;                          /*  Entry Point of the task     */
                                                      /*  ������ڵ�ַ                */
    *(--stk) = (INT32U)0xFFFFFFFEL;                   /*  R14 (LR)  (init value will  */
                                                      /*  cause fault if ever used)   */
                                                                           
    *(--stk) = (INT32U)0x12121212L;                   /*  R12                         */
    *(--stk) = (INT32U)0x03030303L;                   /*  R3                          */
    *(--stk) = (INT32U)0x02020202L;                   /*  R2                          */
    *(--stk) = (INT32U)0x01010101L;                   /*  R1                          */
    *(--stk) = (INT32U)parg;                          /*  R0 : argument  �������     */
                                                      /*  Remaining registers saved on*/
                                                      /*  process stack               */   
	                                                  /*  ʣ�µļĴ������浽��ջ      */

    *(--stk) = (INT32U)0x11111111L;                   /*  R11                         */
    *(--stk) = (INT32U)0x10101010L;                   /*  R10                         */
    *(--stk) = (INT32U)0x09090909L;                   /*  R9                          */
    *(--stk) = (INT32U)0x08080808L;                   /*  R8                          */
    *(--stk) = (INT32U)0x07070707L;                   /*  R7                          */
    *(--stk) = (INT32U)0x06060606L;                   /*  R6                          */
    *(--stk) = (INT32U)0x05050505L;                   /*  R5                          */
    *(--stk) = (INT32U)0x04040404L;                   /*  R4                          */

    return(stk);
}


/* ����ΪһЩ���Ӻ�����ȫ��Ϊ�պ���������˵���뿴������� */

#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookEnd (void)
{
}
#endif


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void)
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OSTCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void)
{
}


/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION >= 251
void OSTaskIdleHook (void)
{
}

#endif

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
