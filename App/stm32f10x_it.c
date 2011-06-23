/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : stm32f10x_it.c
* Author             : Librae
* Last Modified Date : 11/02/2010
* Description        : Main Interrupt Service Routines.
*						This file provides template for all exceptions handler and 
*						peripherals interrupt service routine.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
//#include "includes.h"

#include <stdio.h>
#include <string.h>
#include "uCOS_II.H"
#include "stm32f10x.h"
#include "c_lib_retarget.h"
#include "Serial_Shell.h"
#include "DataStructure.h"
#include "stm32f10x_it.h"
#include "platform_config.h"
#include "general_task.h"
#include "gps.h"
#include "CPU_Init.h"
#include "LTM022A69B.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/

vu32	wdg_clr_flag = 1;
//extern  vu16 ADC_SampleValue[2];
extern  OS_EVENT 	*ComSem;

unsigned int usart1_data_r;
unsigned int usart3_data_r;

BOOLEAN flag_usart1Rx;	//if received some data, this will be set
BOOLEAN flag_usart3Rx;	//if received some data, this will be set

unsigned int trigger_usart1_cnt;

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

/*****************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                        */
/*****************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	printf("Hard Fault occured!\r\n");
	while (1)
	{
	}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	printf("MemManage Fault occured!\r\n");
	while (1)
	{
	}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	printf("Bus Fault occured!\r\n");
	while (1)
	{
	}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	printf("Usage Fault occured!\r\n");
	while (1)
	{
	}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	OS_ENTER_CRITICAL(); // Tell uC/OS-II that we are starting an ISR
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();        // Call uC/OS-II's OSTimeTick()

    OSIntExit();         // Tell uC/OS-II that we are leaving the ISR
}

/*****************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                  */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the */
/*  available peripheral interrupt handler's name please refer to the startup*/
/*  file (startup_stm32f10x_xx.s).                                           */
/*****************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @brief  This function handles WWDG interrupt request.
  * @param  None
  * @retval None
  */
void WWDG_IRQHandler(void)
{
    OS_ENTER_CRITICAL(); 	/* Tell uC/OS-II that we are starting an ISR */
    OSIntNesting++;
    OS_EXIT_CRITICAL();

	/* Update WWDG counter */
	if(wdg_clr_flag == 1)
	{
  		WWDG_SetCounter(0x7F);
		wdg_clr_flag = 0;
	}
	
  	/* Clear EWI flag */
  	WWDG_ClearFlag();

	OSIntExit();         /* Tell uC/OS-II that we are leaving the ISR  */
}

void PVD_IRQHandler(void)
{
}

void TAMPER_IRQHandler(void)
{
}

void RTC_IRQHandler(void)
{
}

void FLASH_IRQHandler(void)
{
}

void RCC_IRQHandler(void)
{
}

void EXTI0_IRQHandler(void)
{
}

void EXTI1_IRQHandler(void)
{
}

void EXTI2_IRQHandler(void)
{
}

void EXTI3_IRQHandler(void)
{
}

void EXTI4_IRQHandler(void)
{
}

void DMA1_Channel1_IRQHandler(void)
{
}

void DMA1_Channel2_IRQHandler(void)
{
}

void DMA1_Channel3_IRQHandler(void)
{
}

void DMA1_Channel4_IRQHandler(void)
{
}

void DMA1_Channel5_IRQHandler(void)
{
}

void DMA1_Channel6_IRQHandler(void)
{
}

void DMA1_Channel7_IRQHandler(void)
{
}

void ADC1_2_IRQHandler(void)
{
}

void USB_HP_CAN_TX_IRQHandler(void)
{
}

void USB_LP_CAN_RX0_IRQHandler(void)
{
}

void CAN_RX1_IRQHandler(void)
{
}

void CAN_SCE_IRQHandler(void)
{
}

void EXTI9_5_IRQHandler(void)
{
	/*
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		// Toggle GPIO_LED pin 0
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0))));

		// Clear the Key Button EXTI line pending bit
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	else if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		// Toggle GPIO_LED pin 0
		GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)((1-GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0))));

		// Clear the Key Button EXTI line pending bit
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	else;
	*/
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		// Clear the Key Button EXTI line pending bit
		EXTI_ClearITPendingBit(EXTI_Line6);

		// Toggle LED3
		//LED4_TOGGLE();
		//printf( "EXTI_Line6 ISR triggered!\r\n" );
		
		// To write to this register, you must write 0x05FA to the VECTKEY[15:0] field, otherwise the processor ignores the write. 
		//SCB->AIRCR = 0X05FA0000 | SCB_AIRCR_SYSRESETREQ;	//Software reset

//		Temp = GetTemp(AD_Value[1]);
//		Volt = GetVolt(AD_Value[0]);
//		printf("Voltage: %d.%02d\tTemperature: %d.%02d\r\n", Volt/100, Volt%100, Temp/100, Temp%100);
//		LCD_DispString( "Voltage: ", WHITE, BLACK, 0, 2 );
//		LCD_DispChar( Volt/100 + 0x30, RED, BLACK, 9, 2 );
//		LCD_DispChar( '.', RED, BLACK, 10, 2 );
//		LCD_DispChar( Volt%100/10 + 0x30, RED, BLACK, 11, 2 );
//		LCD_DispChar( Volt%100%10 + 0x30, RED, BLACK, 12, 2 );
	}
}

void TIM1_BRK_IRQHandler(void)
{
}

void TIM1_UP_IRQHandler(void)
{
}

void TIM1_TRG_COM_IRQHandler(void)
{
}

void TIM1_CC_IRQHandler(void)
{
}

void TIM2_IRQHandler(void)
{
}

void TIM3_IRQHandler(void)
{
}

void TIM4_IRQHandler(void)
{
}

void I2C1_EV_IRQHandler(void)
{
}

void I2C1_ER_IRQHandler(void)
{
}

void I2C2_EV_IRQHandler(void)
{
}

void I2C2_ER_IRQHandler(void)
{
}

void SPI1_IRQHandler(void)
{
}

void SPI2_IRQHandler(void)
{
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART3,  USART_IT_RXNE);
		flag_usart1Rx = 1;
		trigger_usart1_cnt++;
		usart1_data_r = USART_ReceiveData(USART1);
		QueueInput( &USART_Q, usart1_data_r );
		// RXNE pending bit can be also cleared by a read to the USART_DR register (USART_ReceiveData())
	}
}

void USART2_IRQHandler(void)
{
}

void USART3_IRQHandler(void)
{
//	unsigned int temp = 0;
//	unsigned char *p_data = 0;
//	unsigned char *p_data_temp = 0;
//	unsigned char *p_GPS_type = 0;
//	unsigned int com_cnt = 0; //number of the comment symbol
//
//	temp = temp;
//
//	//static unsigned int GPS_data_cnt = 0;	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART3,  USART_IT_RXNE);
//		flag_usart3Rx = 1;
//		usart3_data_r = USART_ReceiveData(USART3);
//		// RXNE pending bit can be also cleared by a read to the USART_DR register (USART_ReceiveData())
//		*p_GPS_data_buff = usart3_data_r;
//		if( usart3_data_r == 0x0A )
//		{
//			//USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//			LED3_ON();
//			flag_GPS_data_updated = 1;
//			*(p_GPS_data_buff + 1) = '\0';
//			p_GPS_data_buff = GPS_data_buff;
//		}
//		else
//		{
//			LED3_OFF();
//			flag_GPS_data_updated = 0;
//			p_GPS_data_buff++;
//		}	
//
//		if( flag_GPS_data_updated )
//		{
//			p_data = &GPS_data_buff[1];
//			p_GPS_keyword = GPS_keyword;
//			while( p_GPS_keyword < &GPS_keyword[5] )
//			{
//				*p_GPS_keyword++ = *p_data++;
//			}
//			*p_GPS_keyword = '\0';
//
//			if( strcmp( GPS_keyword, "GPGGA" ) == 0 )
//			{
//				com_cnt = 0;
//				for( p_data = &GPS_data_buff[6]; *p_data != '\0'; p_data++ )
//				{
//					if( *p_data == ',' )
//					{
//						com_cnt += 1;
//						if( com_cnt == 2 )
//						{
//							p_GPS_type = GPS_data.latitude;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							//*p_GPS_type = '\0';
//							p_data = p_data_temp - 1;
//						}
//						else if( com_cnt == 3 )
//						{
//							p_GPS_type = &GPS_data.lat_ew;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							//*p_GPS_type = '\0';
//							p_data = p_data_temp - 1;
//						}
//						else if( com_cnt == 4 )
//						{
//							p_GPS_type = GPS_data.longitude;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							//*p_GPS_type = '\0';
//							p_data = p_data_temp - 1;
//						}
//						else if( com_cnt == 5 )
//						{
//							p_GPS_type = &GPS_data.long_ns;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							//*p_GPS_type = '\0';
//							p_data = p_data_temp - 1;
//						}
//						else if( com_cnt == 7 )
//						{
//							p_GPS_type = GPS_data.satellites;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							//*p_GPS_type = '\0';
//							p_data = p_data_temp - 1;
//						}
//						else if( com_cnt == 9 )
//						{
//							p_GPS_type = GPS_data.altitude;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							//*p_GPS_type = '\0';
//							p_data = p_data_temp - 1;
//						}
//						else
//						{
//							continue;
//						}
//					}
//				}
//			}
//			else if( strcmp( GPS_keyword, "GPGSA" ) == 0 )
//			{
//			}
//			else if( strcmp( GPS_keyword, "GPRMC" ) == 0 )
//			{
//				com_cnt = 0;
//				for( p_data = &GPS_data_buff[6]; *p_data != '\0'; p_data++ )
//				{
//					if( *p_data == ',' )
//					{
//						com_cnt += 1;
//						if( com_cnt == 1 )
//						{
//							p_GPS_type = GPS_data.time;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							p_data = p_data_temp - 1;
//						}
//						else if( com_cnt == 2 )
//						{
//							p_GPS_type = &GPS_data.status;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							p_data = p_data_temp - 1;
//						}
//						else if( com_cnt == 9 )
//						{
//							p_GPS_type = GPS_data.date;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							p_data = p_data_temp - 1;
//						}
//						else
//						{
//							continue;
//						}
//					}
//				}
//			}
//			else if( strcmp( GPS_keyword, "GPGSV" ) == 0 )
//			{
//				com_cnt = 0;
//				for( p_data = &GPS_data_buff[6]; *p_data != '\0'; p_data++ )
//				{
//					if( *p_data == ',' )
//					{
//						com_cnt += 1;
//						if( com_cnt == 3 )
//						{
//							p_GPS_type = GPS_data.satellites_inview;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							p_data = p_data_temp - 1;
//						}
//						else if( com_cnt == 4 )
//						{
//							p_GPS_type = GPS_data.satellites_id;
//							p_data_temp = p_data + 1;
//							while( *p_data_temp != ',' )
//							{
//								*p_GPS_type++ = *p_data_temp++;
//							}
//							p_data = p_data_temp - 1;
//						}
//						else
//						{
//							continue;
//						}
//					}
//				}
//			}
//			else
//			{
//			}
//		}
//		else
//		{
//		}
//		
//		//printf( "%c", usart3_data_r );
//		//Wait_ms(1);	//Why???????
	}
}

void EXTI15_10_IRQHandler(void)
{
}

void RTCAlarm_IRQHandler(void)
{
}

void USBWakeUp_IRQHandler(void)
{
}

void TIM8_BRK_IRQHandler(void)
{
}

void TIM8_UP_IRQHandler(void)
{
}

void TIM8_TRG_COM_IRQHandler(void)
{
}

void TIM8_CC_IRQHandler(void)
{
}

void ADC3_IRQHandler(void)
{
}

void FSMC_IRQHandler(void)
{
}

void SDIO_IRQHandler(void)
{
}

void TIM5_IRQHandler(void)
{
}

void SPI3_IRQHandler(void)
{
}

void UART4_IRQHandler(void)
{
}

void UART5_IRQHandler(void)
{
}

void TIM6_IRQHandler(void)
{
}

void TIM7_IRQHandler(void)
{
}

void DMA2_Channel1_IRQHandler(void)
{
}

void DMA2_Channel2_IRQHandler(void)
{
}

void DMA2_Channel3_IRQHandler(void)
{
}

void DMA2_Channel4_5_IRQHandler(void)
{
}

/*********************** (C) COPYLEFT 2010 Leafgrass *************************/
