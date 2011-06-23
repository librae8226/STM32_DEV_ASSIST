;************************ (C) COPYLEFT 2010 Leafgrass *************************
;* File Name          : cortexm3_macro.s
;* Author             : Librae
;* Date               : 09/02/2010
;* Description        : This file is create for ST FWLib V3.1.2 STM32F10x Medium Density.
;*						This module performs:
;*                  	- Set the initial SP
;*                  	- Set the initial PC == Reset_Handler,
;*                  	- Set the vector table entries with the exceptions ISR address,
;*                  	- Branches to __main in the C library (which eventually
;*                  	calls main()).
;*                  	After Reset the CortexM3 processor is in Thread mode,
;*                  	priority is Privileged, and the Stack is set to Main.
;******************************************************************************

;******************************************************************************
; Amount of memory (in bytes) allocated for Stack and Heap
; Tailor those values to your application needs          
;******************************************************************************
Stack_Size   EQU     0x400
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

Heap_Size    EQU     0x200
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

;******************************************************************************
; Allocate space for the Stack
;******************************************************************************
  AREA    STACK, NOINIT, READWRITE, ALIGN=3

Stack
    SPACE   Stack_Size
																			   
;******************************************************************************
; Allocate space for the Heap
;******************************************************************************
  AREA    HEAP, NOINIT, READWRITE, ALIGN=3

Heap
    SPACE   Heap_Size

;******************************************************************************
;*  Declarations for the interrupt handlers that are used by the application.                                                                     
;******************************************************************************
  		IMPORT  __main
		IMPORT	NMI_Handler
		IMPORT	HardFault_Handler
		IMPORT	MemManage_Handler
		IMPORT	BusFault_Handler
		IMPORT	UsageFault_Handler
		IMPORT	DebugMon_Handler
		IMPORT	SVC_Handler
  		;IMPORT  PendSV_Handler		;original, function is in stm32f10x_it.c
		IMPORT  OSPendSV			;modified, function is in os_cpu_a.asm
  		IMPORT  SysTick_Handler		;function is in stm32f10x_it.c

		; External Interrupts
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

;******************************************************************************
;*  Reset code section.                                                                                                           
;******************************************************************************
        AREA    RESET, CODE, READONLY
        THUMB

;******************************************************************************
; Fill-up the Vector Table entries with the exceptions ISR address
;******************************************************************************
    	EXPORT  __Vectors
__Vectors                      
    	DCD  Stack + Stack_Size		;Top of Stack
    	DCD  Reset_Handler
    	DCD  NMI_Handler
    	DCD  HardFault_Handler
    	DCD  MemManage_Handler
    	DCD  BusFault_Handler
    	DCD  UsageFault_Handler
    	DCD  0						;Reserved
    	DCD  0						;Reserved
    	DCD  0						;Reserved
    	DCD  0						;Reserved
    	DCD  SVC_Handler
    	DCD  DebugMon_Handler
    	DCD  0						;Reserved
    	;DCD  PendSV_Handler		;original, function is in stm32f10x_it.c
		DCD  OSPendSV				;modified, function is in os_cpu_a.asm
    	DCD  SysTick_Handler		;function is in stm32f10x_it.c
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

;******************************************************************************
;*  Reset entry
;******************************************************************************
;--------------original---------------
;        EXPORT  Reset_Handler
;Reset_Handler
;        IMPORT  __main
;        LDR     R0, =__main
;        BX      R0
;-------------------------------------

; Reset handler (added at 09/02/2010)
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
     IMPORT  __main
     IMPORT  SystemInit
                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

;******************************************************************************
;*  Make sure the end of this section is aligned.
;******************************************************************************
        ALIGN


;******************************************************************************
;*  Code section for initializing the heap and stack                                                                                                          
;******************************************************************************
		AREA    |.text|, CODE, READONLY


;******************************************************************************
;*  The function expected of the C library startup 
;*  code for defining the stack and heap memory locations. 
;******************************************************************************
		IF      :DEF:__MICROLIB           
		
		EXPORT  __initial_sp
		EXPORT  __heap_base
		EXPORT  __heap_limit
		
		ELSE
		
		IMPORT  __use_two_region_memory
		EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

		LDR     R0, =  Heap_Mem
		LDR     R1, =(Stack_Mem + Stack_Size)
		LDR     R2, = (Heap_Mem +  Heap_Size)
		LDR     R3, = Stack_Mem
		BX      LR
		
		ALIGN
		
		ENDIF
		
		END

;******************************************************************************
;*  Make sure the end of this section is aligned.
;******************************************************************************
        ALIGN


;******************************************************************************
;*  End Of File                                                     
;******************************************************************************
        END

;************************ (C) COPYLEFT 2010 Leafgrass *************************
