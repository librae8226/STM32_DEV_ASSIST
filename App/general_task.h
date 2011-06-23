/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : general_task.h
* Author             : Librae
* Last Modified Date : 09/17/2010
* Description        : This file contains the general and miscellaneous task.

******************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __GENERAL_TASK_H__
#define __GENERAL_TASK_H__

/* Includes ---------------------------------------------------------------- */
/* Exported types ---------------------------------------------------------- */
/* Exported variables ------------------------------------------------------ */

extern s16  Temp;
extern u32  Volt;
extern vu16 AD_Value[2];

/* Exported constants ------------------------------------------------------ */
/* Exported macro ---------------------------------------------------------- */
/* Exported functions ------------------------------------------------------ */
void ADC_Test(void);
void SD_Test(void);
u16 GetTemp(u16 advalue);
u16 GetVolt(u16 advalue);

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
