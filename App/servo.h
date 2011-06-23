/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : servo.h
* Author             : Librae
* Last Modified Date : 08/16/2010
* Description        : Declaration of servo related things.

******************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __SERVO_H__
#define __SERVO_H__

/* Includes ---------------------------------------------------------------- */
/* Exported types ---------------------------------------------------------- */
/* Exported variables ------------------------------------------------------ */
/* Exported constants ------------------------------------------------------ */

//#define SERVOMID		115
//#define RIGHTLIMIT		SERVOMID + 50
//#define LEFTLIMIT		SERVOMID - 50
#define SERVOMID		75
#define RIGHTLIMIT		SERVOMID + 30
#define LEFTLIMIT		SERVOMID - 30


/* Exported macro ---------------------------------------------------------- */
/* Exported functions ------------------------------------------------------ */

void Servo_SetAngle( unsigned int angle );
void Servo_Test( void );

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
