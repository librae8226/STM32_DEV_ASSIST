/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : CPU_Init.c
* Author             : Librae
* Last Modified Date : 11/10/2010
* Description        : This file provides all the CPU initialization functions.

******************************************************************************/

/* Includes ---------------------------------------------------------------- */
//#include "includes.h"
#include <stdio.h>
#include "os_cpu.h"
#include "os_cfg.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "platform_config.h"
#include "LTM022A69B.h"
#include "SD_Driver.h"
#include "ffext.h"
#include "NRF24L01.h"
#include "OV7725.h"
#include "servo.h"
#include "general_task.h"
#include "CPU_Init.h"

/* Private typedef ----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED } TestStatus;
SPI_InitTypeDef SPI_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
DMA_InitTypeDef DMA_InitStructure;

/* Private define -----------------------------------------------------------*/

/* Private macro ------------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/

ErrorStatus HSEStartUpStatus;

u8 USART1_DMA_TxBuff[16];
u8 USART1_DMA_RxBuff[16];

/* Private function prototypes ----------------------------------------------*/

/* Private functions --------------------------------------------------------*/
/******************************************************************************
* Function Name  : CPU_Init
* Description    : Controls CPU initialization
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void CPU_Init(void)
{
	flag_usart1Rx = 0;
	flag_usart3Rx = 0;
	trigger_usart1_cnt = 0;

	//===================

	RCC_Configuration();	// System clocks configuration
	NVIC_Configuration();
	EXTI_Configuration();
	DMA_Configuration();
	TIM_Configuration();
	USART_Configuration();
	SPI_Configuration();
	GPIO_Configuration();
	ADC1_Configuration();
	SysTick_Configuration();

#if defined USE_STM32_DQ_128K_64P
	LED2_OFF();
	LED3_OFF();
	LEDUSB_ON();
#elif defined USE_STM32_CORE_512K_100P
	LED0_OFF();
	LED1_OFF();
	LED2_OFF();
	LED3_OFF();
	LEDUSB_OFF();
#endif
}

/******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures Leds and Keys.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

#if defined USE_STM32_DQ_128K_64P
	//Configure PA0 PA1 as output for LED2 LED3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Configure PD2 as LED4(USB LED)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// Configure PA3 & PA8 as input for S1 & S2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_8;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// LCD SPI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// LCD control related
// 	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Pin = 	LCD_RST | LCD_CS | LCD_RS;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(LCD_CTRL_PORT, &GPIO_InitStructure);

	/*
	// TOUCH control related
 	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_Init(TOUCH_CTRL_PORT, &GPIO_InitStructure);
	
 	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_6 | GPIO_Pin_7;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(TOUCH_CTRL_PORT, &GPIO_InitStructure);
	*/

	// Configure Key Button GPIO Pin as input floating (Key Button EXTI Line)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	// Configure USART1 Tx (PA9) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Configure USART1 Rx (PA10) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Configure USART3 Tx (PB10) as alternate function push-pull
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// Configure USART3 Rx (PB11) as input floating
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*
	// Configure SPI2 pins: SCK, MISO and MOSI --------------------------------
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	*/
	// PWM Mode: TIM3_CH3, TIM3_CH4
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//ADC_CH10--> PC0
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// ------ NRF24L01 ------
	// Configure SPI2 pins: SCK, MISO and MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// Control
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE | NRF24L01_CSN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(NRF24L01_CTRL_PORT, &GPIO_InitStructure);

	// SPI1 -> SD
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA4 pin: SD CS
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//PA2: SD detect
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

#elif defined USE_STM32_CORE_512K_100P
	// ------ Camera Related ------
	// OV7725 XCLK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    RCC_MCOConfig(RCC_MCO_HSE);	//HSI
	
	// SCCB
	GPIO_InitStructure.GPIO_Pin = SCCB_SIC_BIT | SCCB_SID_BIT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCCB_CTRL_PORT, &GPIO_InitStructure);

	// VSYNC
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_VSYNC_CMOS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIO_VSYNC_CMOS, &GPIO_InitStructure);

	// FIFO Control
	GPIO_InitStructure.GPIO_Pin = FIFO_RD_BIT | FIFO_WRST_BIT | FIFO_RRST_BIT | FIFO_CS_BIT | FIFO_WE_BIT;                               
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FIFO_CTRL_PORT, &GPIO_InitStructure);
	// FIFO Input
	GPIO_InitStructure.GPIO_Pin = FIFO_D0 | FIFO_D1 | FIFO_D2 | FIFO_D3 | FIFO_D4 | FIFO_D5 | FIFO_D6 | FIFO_D7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FIFO_DATA_PORT, &GPIO_InitStructure);

	// ------ Configure LEDs ------
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//Configure PD2 as LEDUSB
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// ------ USART1 ------
	// Configure USART1 Tx (PA9) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Configure USART1 Rx (PA10) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ------ GPS ------
	// Configure USART3 Tx (PB10) as alternate function push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// Configure USART3 Rx (PB11) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//  GPS_EN 14, GPS_BOOT 15
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	// ------ NRF24L01 ------
	// Configure SPI2 pins: SCK, MISO and MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// Control
	GPIO_InitStructure.GPIO_Pin = NRF24L01_CE | NRF24L01_CSN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(NRF24L01_CTRL_PORT, &GPIO_InitStructure);

	// ------ TF Card ------
	// Configure SPI1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

#endif
}

/******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks and Enable peripherals.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;

	// RCC system reset(for debug purpose)
	RCC_DeInit();

	// Enable HSE
	RCC_HSEConfig(RCC_HSE_ON);
	
	// Wait till HSE is ready
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	
	if(HSEStartUpStatus == SUCCESS)
	{
		// Enable Prefetch Buffer
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		// Flash 2 wait state
		FLASH_SetLatency(FLASH_Latency_2);
		
		// HCLK = SYSCLK
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		// PCLK2 = HCLK/2
		RCC_PCLK2Config(RCC_HCLK_Div2);
		
		// PCLK1 = HCLK/2
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		// PLLCLK = 8MHz * 9 = 72 MHz
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		
		// Enable PLL
		RCC_PLLCmd(ENABLE);
		
		// Wait till PLL is ready
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}		
		// Select PLL as system clock source
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);		
		// Wait till PLL is used as system clock source
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
	
	// Enable peripheral clocks ---------------------------------------------
#if defined USE_STM32_DQ_128K_64P
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | 
							RCC_APB2Periph_GPIOB | 
							RCC_APB2Periph_GPIOC | 
							RCC_APB2Periph_GPIOD |
							RCC_APB2Periph_AFIO | 
							RCC_APB2Periph_TIM1 |  
							RCC_APB2Periph_USART1 | 
							RCC_APB2Periph_ADC1	|
							RCC_APB2Periph_SPI1, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2 | 
							RCC_APB1Periph_TIM3 |
							RCC_APB1Periph_USART3, ENABLE );
    RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_DMA1, ENABLE );

#elif defined USE_STM32_CORE_512K_100P
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | 
							RCC_APB2Periph_GPIOB | 
							RCC_APB2Periph_GPIOC | 
							RCC_APB2Periph_GPIOD |
							RCC_APB2Periph_GPIOE |
							RCC_APB2Periph_GPIOF |
							RCC_APB2Periph_AFIO | 
							RCC_APB2Periph_TIM1 |  
							RCC_APB2Periph_USART1 | 
							RCC_APB2Periph_ADC1	|
							RCC_APB2Periph_SPI1, ENABLE );
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2 | 
							RCC_APB1Periph_TIM3 |
							RCC_APB1Periph_USART3, ENABLE );
    RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_DMA1, ENABLE );

#endif
}

/******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 

#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

	/* 1 bit for pre-emption priority, 3 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Configure and enable SPI2 interrupt ----------------------------------*/
	/*
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	*/
	// Enable the EXTI9_5 Interrupt
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	/*
	//TIM1的NVIC设置：允许中断，设置优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;     //更新事件
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;           //响应优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //允许中断
	NVIC_Init(&NVIC_InitStructure);                              //写入设置
	*/ 
	
	// Enable the USART1 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the USART3 Interrupt
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************
* Function Name  : TIM_Configuration
* Description    : Configures TIM.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* -----------------------------------------------------------------------
	
	TIM3 ARR Register = 1894 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	TIM3 Frequency = 37.99 KHz (接近38KHz 红外发射管的调制基频)
	TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
	----------------------------------------------------------------------- */
	
	/*
		TIM3 配置: 产生1路PWM信号;
		TIM3CLK = 72 MHz, Prescaler = 0(不分频), TIM3 counter clock = 72 MHz
		计算公式：
		PWM输出频率 = TIM3 counter clock /(ARR + 1)
	
		我们期望设置为100Hz
	
		如果不对TIM3CLK预分频，那么不可能得到100Hz低频。
		我们设置分频比 = 1000， 那么  TIM3 counter clock = 72KHz
		TIM_Period = 720 - 1;
		f = 72K / 720 = 100Hz
	*/
	TIM_TimeBaseInitStructure.TIM_Period = 720 - 1;	/* TIM_Period = TIM3 ARR Register */
	TIM_TimeBaseInitStructure.TIM_Prescaler = 2000;	// 50Hz
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*
		_bright = 1 时, TIM_Pulse = 1
		_bright = 255 时, TIM_Pulse = TIM_Period
	*/
	// 20ms period, for servo. high level time about 1.5ms
	TIM_OCInitStructure.TIM_Pulse = (TIM_TimeBaseInitStructure.TIM_Period * (SERVOMID*11 + 500)) / 20000;
	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// TIM3_CH3
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	// TIM3_CH4
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);

	/*
	//TIM1 使用内部时钟
	//TIM_InternalClockConfig(TIM1);
	//TIM1 基本设置
	//TIM_RepetitionCounter(TIM1_RCR)=0，每次向上溢出都产生更新事件
	TIM_BaseInitStructure.TIM_Period = 1000;	//TIM_Period（TIM1_ARR）=1000，计数器向上计数到1000后产生更新事件，计数值归零
	TIM_BaseInitStructure.TIM_Prescaler = 71;	//设置预分频器分频系数71，即APB2=72M, TIM1_CLK=72/72=1MHz
	TIM_BaseInitStructure.TIM_ClockDivision = 0;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数模式
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
	
	//清中断，以免一启用中断后立即产生中断
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);

	//使能TIM1中断源
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	//TIM1总开关：开启
	TIM_Cmd(TIM1, ENABLE);
	*/
}

/******************************************************************************
* Function Name  : EXTI_Configuration
* Description    : 
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void EXTI_Configuration(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
	/*
    // connect PC8 to EXTI
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
    // Config EXTI8
    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	*/

	// connect PC6 to EXTI
    GPIO_EXTILineConfig(GPIO_PORT_SOURCE_VSYNC_CMOS, GPIO_PIN_SOURCE_VSYNC_CMOS);
    // Config EXTI6
    EXTI_InitStructure.EXTI_Line = EXTI_LINE_VSYNC_CMOS;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
}

/******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures Universal Synchronous Asynchronous Receiver Transmitter.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void USART_Configuration(void)
{
	/*
		USART1 configured as follow:
	    - BaudRate = 9600 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);

	// Enable the USART Receive interrupt: this interrupt is generated when the
	// USART1 receive data register is not empty
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
	
	printf("\r");	//不知道为什么这个加上第1个字节才能发出去. 据说是内部移位寄存器的毛病?

	/*
		USART3 configured as follow:
	    - BaudRate = 9600 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &USART_InitStructure);

	// Enable the USART Receive interrupt: this interrupt is generated when the
	// USART1 receive data register is not empty
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);
}

/******************************************************************************
* Function Name  : SPI_Configuration
* Description    : Configures SPI
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void SPI_Configuration(void)
{
	// SPI1 Config for SD -----------------------------------------------------
//	MSD_SPIConfig();
	
	// SPI1 Config for LCD ----------------------------------------------------
	SPI_Cmd(SPI_LCD, DISABLE);
	SPI_I2S_DeInit(SPI_LCD); 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI_LCD, &SPI_InitStructure);
	SPI_Cmd(SPI_LCD, ENABLE);

	// SPI2 Config for NRF24L01
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	//SPI_CPOL_High=模式3，时钟空闲为高 SPI_CPOL_Low=模式0，时钟空闲为低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//SPI_CPHA_2Edge, SPI_CPHA_1Edge, SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	//SPI_NSS_Soft, SPI_NSS_Hard
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;	//SPI_BaudRatePrescaler_2=18M; SPI_BaudRatePrescaler_4=9MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//数据从高位开始发送
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
}

/******************************************************************************
* Function Name  : ADC1_Configuration
* Description    : Configures AD Converter 1
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void ADC1_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //连续转换开启
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 2;     //设置转换序列长度为2
    ADC_Init(ADC1, &ADC_InitStructure);
    
    //ADC内置温度传感器使能（要使用片内温度传感器，切忌要开启它）
    ADC_TempSensorVrefintCmd(ENABLE);
    
    //常规转换序列1：通道10
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_1Cycles5);
    //常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 2, ADC_SampleTime_239Cycles5);
    
    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
    ADC_DMACmd(ADC1, ENABLE);
    
    // 下面是ADC自动校准，开机后需执行一次，保证精度
    // Enable ADC1 reset calibaration register 
    ADC_ResetCalibration(ADC1);
    // Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));

    // Start ADC1 calibaration
    ADC_StartCalibration(ADC1);
    // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
    // ADC自动校准结束---------------
	//启动第一次AD转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configures SysTick
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void SysTick_Configuration(void)
{
	RCC_ClocksTypeDef  rcc_clocks;
    INT32U cnts;
    RCC_GetClocksFreq(&rcc_clocks);
    cnts = (INT32U)rcc_clocks.HCLK_Frequency/OS_TICKS_PER_SEC;
	SysTick_Config(cnts);	   //设置滴答的中断间隔为1MS
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); //设置滴答的时钟源为系统时钟
}

/******************************************************************************
* Function Name  : DMA_Configuration
* Description    : Configures DMA
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void DMA_Configuration(void)
{
	// DMA_Channel7 - USART1_Tx (peripheral --> mem)
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel7);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);				//serial tx register as source buffer
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_DMA_TxBuff;				//destination buffer
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;							//peripheral as data destination
	DMA_InitStructure.DMA_BufferSize = 16;										//buffer size
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址寄存器不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设字节为单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;			//内存字节为单位
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;						//4优先级之一的普通(Medium)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//非内存到内存
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);								//DMA_Channel7
	
	//DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);							//DMA_Channel7传输完成中断
	//USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);								//DMA方式从串口发送
	//DMA_Cmd(DMA1_Channel7, ENABLE);											//正式允许DMA

	// DMA_Channel6 - USART1_Rx (peripheral --> mem)
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);				//serial rx register as source buffer
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_DMA_RxBuff;				//destination buffer
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;							//peripheral as data source
	DMA_InitStructure.DMA_BufferSize = 16;										//buffer size
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//外设地址寄存器不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设字节为单位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;			//内存字节为单位
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;						//4优先级之一的最高(VeryHigh)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//非内存到内存
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);								//DMA_Channel6
	
	//DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);							//DMA_Channel6传输完成中断
	//USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);								//DMA方式从串口接收
	//DMA_Cmd(DMA1_Channel6, ENABLE);												//正式允许DMA

	// DMA1_Channel1 - ADC1
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //BufferSize=2，因为ADC转换序列有2个通道
    // 如此设置，使序列1结果放在AD_Value[0]，序列2结果放在AD_Value[1]
    DMA_InitStructure.DMA_BufferSize = 2;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    // 循环模式开启，Buffer写满后，自动回到初始地址开始传输
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    // 配置完成后，启动DMA通道
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

/******************************************************************************
* Function Name  : Wait
* Description    : Wait time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
******************************************************************************/
void Wait_ms(u32 nTime)
{
	u32 j;
	while(nTime--)
	{
		j = 0x3100;
		while(j--);
	}
}

/******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
******************************************************************************/
void Delay(vu32 nCount)
{
	for(; nCount != 0; nCount--);
}

void delay_ms(unsigned long time)
{
    unsigned int i,j;
    i=0;
    for(i=0;i<time;i++)
    {
       j=108;
       while(j--);
    }
}

void delay_us(unsigned long i)
{     
	while( i--)
	{	
	}
}
/*********************** (C) COPYLEFT 2010 Leafgrass *************************/
