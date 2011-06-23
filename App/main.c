/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : main.c
* Author             : Librae
* Version            : v0.0.2
* Last Modified Date : 11/16/2010
* Description        : main program body

* >>>>>>>>>>	Hardware	<<<<<<<<<<
*	SIGNAL:
*		PA0		---		LED2(red)
*		PA1		---		LED3(yellow)
*		PA3		---		S1
*		PA8		---		S2
*	LCD_CTRL_PORT:
*		PC8		---		LCD_RST
*		PC9		---		LCD_CS
*		PC10	---		LCD_RS
*		PC11	---		LCD_SI(soft)
*		PC12	---		LCD_SCL(soft)
*	USART:
*		PA9		---		UART1_Tx
*		PA10	---		UART1_Rx
*		PB10	---		UART3_Tx(GPS_Rx)
*		PB11	---		UART3_Rx(GPS_Tx)
*	SPI:
*		PA5		---		SPI1_SCK(SD_SCLK)
*		PA6		---		SPI1_MISO(SD_MISO)
*		PA7		---		SPI1_MOSI(SD_MOSI)
*		PB13	---		SPI2_SCK(LCD_SCL)
*		PB14	---		SPI2_MISO
*		PB15	---		SPI2_MOSI(LCD_SI)
*	PWM:
*		PB0		---		TIM3_CH3(servo)
*		PB1		---		TIM3_CH4(servo)
*	MSD:
*		PA2		---		DET
*		PA4		---		CS
*		PA5		---		SCLK
*		PA6		---		MISO
*		PA7		---		MOSI
*		
*	===============================================
*	LCD connector:
	1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
	----------------------------------------------
	|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	G VCC D0 1  2  3  4  5  6  7  SI CL CS RS RST NC
*	===============================================

******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	Wait_ms(1);	//Ensure I/O is ready

	#ifdef DEBUG
		debug();	//Debug Module
	#endif

	OSInit();

	OSTaskCreate( TaskStart,	//task pointer
					(void *)0,	//parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
					START_TASK_Prio );	//task priority
	
	OSStart();

	return 0;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	
	/* Infinite loop */
	while (1)
	{
	}
}

#endif

/*********************** (C) COPYLEFT 2010 Leafgrass *************************/
