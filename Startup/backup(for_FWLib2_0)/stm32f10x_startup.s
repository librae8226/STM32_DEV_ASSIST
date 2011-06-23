;/****************************Copyright (c)*********************************************                                     
;*
;*               (c) Copyright 2006-2008, hui lian. luo, china, zj. hz 
;*                            All Rights Reserved
;*
;*							  深圳市英蓓特信息技术有限公司
;*                            http://www.embedinfo.com
;*                            博格达科技有限公司
;*                            http://www.bogodtech.com                         
;*
;*---------- File Info ---------------------------------------------------------------
;* File name: 		stm32f10x_startup.s
;* Created By: 		罗辉联(armgcc@foxmail.com, wyuyun@hotmail.com, lhlzjut@hotmail.com) 	
;* Created date: 	2007.11.28
;* Descriptions: 	This module performs:
;*                  - Set the initial SP
;*                  - Set the initial PC == Reset_Handler,
;*                  - Set the vector table entries with the exceptions ISR address,
;*                  - Branches to __main in the C library (which eventually
;*                  calls main()).
;*                  After Reset the CortexM3 processor is in Thread mode,
;*                  priority is Privileged, and the Stack is set to Main.
;* Adviser: 		楼东武(professor)  浙江大学信电系
;*
;*---------- History Info -------------------------------------------------------------
;* Version: 		Ver1.0
;* Descriptions: 	Create Initilze file
;*
;*-------------------------------------------------------------------------------------
;**************************************************************************************/


;***************************************************************************************
; Amount of memory (in bytes) allocated for Stack and Heap
; Tailor those values to your application needs          
;***************************************************************************************
Stack_Size   EQU     0x200
Heap_Size    EQU     0x200

;*******************************************************************************
; Allocate space for the Stack
;*******************************************************************************
  AREA    STACK, NOINIT, READWRITE, ALIGN=3

Stack
    SPACE   Stack_Size

;*******************************************************************************
; Allocate space for the Heap
;*******************************************************************************
  AREA    HEAP, NOINIT, READWRITE, ALIGN=3

Heap
    SPACE   Heap_Size

;********************************************************************************
;*  Declarations for the interrupt handlers that are used by the application.                                                                     
;********************************************************************************
  		IMPORT  __main
		IMPORT	NMIException
		IMPORT	HardFaultException
		IMPORT	MemManageException
		IMPORT	BusFaultException
		IMPORT	UsageFaultException
		IMPORT	DebugMonitor
		IMPORT	SVCHandler
  		IMPORT  OSPendSV
  		IMPORT  SysTickHandler
  		IMPORT  WWDG_IRQHandler
  		IMPORT  PVD_IRQHandler
  		IMPORT  TAMPER_IRQHandler
  		IMPORT  RTC_IRQHandler
  		IMPORT  FLASH_IRQHandler
  		IMPORT  RCC_IRQHandler
  		IMPORT  EXTI0_IRQHandler
  		IMPORT  EXTI1_IRQHandler
  		IMPORT  EXTI2_IRQHandler
  		IMPORT  EXTI3_IRQHandler
  		IMPORT  EXTI4_IRQHandler
        IMPORT  DMA1_Channel1_IRQHandler
        IMPORT  DMA1_Channel2_IRQHandler
        IMPORT  DMA1_Channel3_IRQHandler
        IMPORT  DMA1_Channel4_IRQHandler
        IMPORT  DMA1_Channel5_IRQHandler
        IMPORT  DMA1_Channel6_IRQHandler
        IMPORT  DMA1_Channel7_IRQHandler
        IMPORT  ADC1_2_IRQHandler
        IMPORT  USB_HP_CAN_TX_IRQHandler
        IMPORT  USB_LP_CAN_RX0_IRQHandler
        IMPORT  CAN_RX1_IRQHandler
        IMPORT  CAN_SCE_IRQHandler
        IMPORT  EXTI9_5_IRQHandler
        IMPORT  TIM1_BRK_IRQHandler
        IMPORT  TIM1_UP_IRQHandler
        IMPORT  TIM1_TRG_COM_IRQHandler
        IMPORT  TIM1_CC_IRQHandler
        IMPORT  TIM2_IRQHandler
        IMPORT  TIM3_IRQHandler
        IMPORT  TIM4_IRQHandler
        IMPORT  I2C1_EV_IRQHandler
        IMPORT  I2C1_ER_IRQHandler
        IMPORT  I2C2_EV_IRQHandler
        IMPORT  I2C2_ER_IRQHandler
        IMPORT  SPI1_IRQHandler
        IMPORT  SPI2_IRQHandler
        IMPORT  USART1_IRQHandler
        IMPORT  USART2_IRQHandler
        IMPORT  USART3_IRQHandler
        IMPORT  EXTI15_10_IRQHandler
        IMPORT  RTCAlarm_IRQHandler
        IMPORT  USBWakeUp_IRQHandler
        IMPORT  TIM8_BRK_IRQHandler
        IMPORT  TIM8_UP_IRQHandler
        IMPORT  TIM8_TRG_COM_IRQHandler
        IMPORT  TIM8_CC_IRQHandler
        IMPORT  ADC3_IRQHandler
        IMPORT  FSMC_IRQHandler
        IMPORT  SDIO_IRQHandler
        IMPORT  TIM5_IRQHandler
        IMPORT  SPI3_IRQHandler
        IMPORT  UART4_IRQHandler
        IMPORT  UART5_IRQHandler
        IMPORT  TIM6_IRQHandler
        IMPORT  TIM7_IRQHandler
        IMPORT  DMA2_Channel1_IRQHandler
        IMPORT  DMA2_Channel2_IRQHandler
        IMPORT  DMA2_Channel3_IRQHandler
        IMPORT  DMA2_Channel4_5_IRQHandler
   
		PRESERVE8

;**********************************************************************************
;*  Reset code section.                                                                                                           
;**********************************************************************************
        AREA    RESET, CODE, READONLY
        THUMB

;*******************************************************************************
; Fill-up the Vector Table entries with the exceptions ISR address
;*******************************************************************************
    	EXPORT  __Vectors
__Vectors                      
    	DCD  Stack + Stack_Size            ; Top of Stack
    	DCD  Reset_Handler
    	DCD  NMIException
    	DCD  HardFaultException
    	DCD  MemManageException
    	DCD  BusFaultException
    	DCD  UsageFaultException
    	DCD  0                 ; Reserved
    	DCD  0                 ; Reserved
    	DCD  0                 ; Reserved
    	DCD  0                 ; Reserved
    	DCD  SVCHandler
    	DCD  DebugMonitor
    	DCD  0                 ; Reserved
    	DCD  OSPendSV
    	DCD  SysTickHandler
    	DCD  WWDG_IRQHandler
    	DCD  PVD_IRQHandler
    	DCD  TAMPER_IRQHandler
    	DCD  RTC_IRQHandler
    	DCD  FLASH_IRQHandler
    	DCD  RCC_IRQHandler
    	DCD  EXTI0_IRQHandler
    	DCD  EXTI1_IRQHandler
    	DCD  EXTI2_IRQHandler
    	DCD  EXTI3_IRQHandler
    	DCD  EXTI4_IRQHandler
        DCD  DMA1_Channel1_IRQHandler
        DCD  DMA1_Channel2_IRQHandler
        DCD  DMA1_Channel3_IRQHandler
        DCD  DMA1_Channel4_IRQHandler
        DCD  DMA1_Channel5_IRQHandler
        DCD  DMA1_Channel6_IRQHandler
        DCD  DMA1_Channel7_IRQHandler
        DCD  ADC1_2_IRQHandler
        DCD  USB_HP_CAN_TX_IRQHandler
        DCD  USB_LP_CAN_RX0_IRQHandler
        DCD  CAN_RX1_IRQHandler
        DCD  CAN_SCE_IRQHandler
        DCD  EXTI9_5_IRQHandler
        DCD  TIM1_BRK_IRQHandler
        DCD  TIM1_UP_IRQHandler
        DCD  TIM1_TRG_COM_IRQHandler
        DCD  TIM1_CC_IRQHandler
        DCD  TIM2_IRQHandler
        DCD  TIM3_IRQHandler
        DCD  TIM4_IRQHandler
        DCD  I2C1_EV_IRQHandler
        DCD  I2C1_ER_IRQHandler
        DCD  I2C2_EV_IRQHandler
        DCD  I2C2_ER_IRQHandler
        DCD  SPI1_IRQHandler
        DCD  SPI2_IRQHandler
        DCD  USART1_IRQHandler
        DCD  USART2_IRQHandler
        DCD  USART3_IRQHandler
        DCD  EXTI15_10_IRQHandler
        DCD  RTCAlarm_IRQHandler
        DCD  USBWakeUp_IRQHandler 
        DCD  TIM8_BRK_IRQHandler
        DCD  TIM8_UP_IRQHandler
        DCD  TIM8_TRG_COM_IRQHandler
        DCD  TIM8_CC_IRQHandler
        DCD  ADC3_IRQHandler
        DCD  FSMC_IRQHandler
        DCD  SDIO_IRQHandler
        DCD  TIM5_IRQHandler
        DCD  SPI3_IRQHandler
        DCD  UART4_IRQHandler
        DCD  UART5_IRQHandler
        DCD  TIM6_IRQHandler
        DCD  TIM7_IRQHandler
        DCD  DMA2_Channel1_IRQHandler
        DCD  DMA2_Channel2_IRQHandler
        DCD  DMA2_Channel3_IRQHandler
        DCD  DMA2_Channel4_5_IRQHandler

;******************************************************************************************
;*  Reset entry
;******************************************************************************************
        EXPORT  Reset_Handler
Reset_Handler
        IMPORT  __main
        LDR     R0, =__main
        BX      R0

;*******************************************************************************************
;*  Make sure the end of this section is aligned.
;*******************************************************************************************
        ALIGN


;********************************************************************************************
;*  Code section for initializing the heap and stack                                                                                                          
;********************************************************************************************
		AREA    |.text|, CODE, READONLY


;********************************************************************************************
;*  The function expected of the C library startup 
;*  code for defining the stack and heap memory locations. 
;********************************************************************************************
        IMPORT  __use_two_region_memory
        EXPORT  __user_initial_stackheap 
__user_initial_stackheap
        LDR     R0, =Heap
        LDR     R1, =(Stack + Stack_Size)
        LDR     R2, =(Heap + Heap_Size)
        LDR     R3, =Stack
        BX      LR

;******************************************************************************************
;*  Make sure the end of this section is aligned.
;******************************************************************************************
        ALIGN


;*******************************************************************************************
;*  End Of File                                                     
;*******************************************************************************************
        END

