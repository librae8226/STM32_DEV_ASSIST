/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : c_lib_retarget.h
* Author             : Librae
* Last Modified Date : 08/15/2010
* Description        : Retarget original C library.

******************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef __C_LIB_RETARGET_H__
#define __C_LIB_RETARGET_H__

/* Includes ---------------------------------------------------------------- */
/* Exported types ---------------------------------------------------------- */
/* Exported variables ------------------------------------------------------ */
/* Exported constants ------------------------------------------------------ */
/* Exported macro ---------------------------------------------------------- */
/* Exported functions ------------------------------------------------------ */

#ifdef __GNUC__
// With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar()
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#define GETCHAT_PROTOTYPE int __io_getchar()
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif // __GNUC__


#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
