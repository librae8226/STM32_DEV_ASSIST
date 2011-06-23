/************************ (C) COPYLEFT 2010 Leafgrass *************************
* File Name          : CPU_Init.h
* Author             : Librae
* Last Modified Date : 09/17/2010
* Description        : This file contains all the functions prototypes for the
*                      CPU initializaion.
******************************************************************************/

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __CPU_INIT_H__
#define __CPU_INIT_H__

/* Includes -----------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types -----------------------------------------------------------*/
extern SPI_InitTypeDef SPI_InitStructure;
extern GPIO_InitTypeDef GPIO_InitStructure;
extern USART_InitTypeDef USART_InitStructure;
extern DMA_InitTypeDef DMA_InitStructure;

/* Exported constants -------------------------------------------------------*/
#define ADC1_DR_Address   ((uint32_t)0x4001244C)

/* Exported macro -----------------------------------------------------------*/
#define EnableInterrupts()	NVIC_RESETPRIMASK()
#define DisableInterrupts()	NVIC_SETPRIMASK()
#define EnableEXTI()		EXTI->IMR = 0xFFFF; EXTI->EMR = 0xFFFF
#define DisableEXTI()		EXTI->IMR = 0x0000; EXTI->EMR = 0x0000

/* Exported variables ------------------------------------------------------ */
// USART DMA(use dual-buffer)
extern u8 USART1_DMA_TxBuff[16];
extern u8 USART1_DMA_RxBuff[16];

/* Exported functions ------------------------------------------------------ */
void CPU_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);
void USART_Configuration(void);
void SPI_Configuration(void);
void DMA_Configuration(void);
void TIM_Configuration(void);
void ADC1_Configuration(void);
void SysTick_Configuration(void);
void DMA_Configuration(void);
void Wait_ms(u32 nTime);
void Delay(vu32 nCount);
void delay_ms(unsigned long time);
void delay_us(unsigned long time);

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
