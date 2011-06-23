/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : platform_config.h
* Author             : Librae
* Last Modified Date : 11/09/2010
* Description        : Board specific configuration file.

******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H__
#define __PLATFORM_CONFIG_H__

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#if !defined (USE_STM32_DQ_128K_64P) && !defined (USE_STM32_CORE_512K_100P)
//	#define USE_STM32_DQ_128K_64P
//	#define USE_STM32_CORE_512K_100P
#endif

#if defined (USE_STM32_DQ_128K_64P)
	#define ISKEY1_PRESSED()		(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
	#define ISKEY2_PRESSED()		(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))
	#define LED2_ON()				(GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1))	//RED
	#define LED2_OFF()				(GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0))
	#define LED2_TOGGLE()			( GPIO_WriteBit( GPIOA, GPIO_Pin_0, (BitAction)!(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0)) ) )
	#define LED3_ON()				(GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)1))	//YELLOW
	#define LED3_OFF()				(GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)0))
	#define LED3_TOGGLE()			( GPIO_WriteBit( GPIOA, GPIO_Pin_1, (BitAction)!(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1)) ) )
	#define LEDUSB_ON()				(GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)1))
	#define LEDUSB_OFF()			(GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)0))
	#define LEDUSB_TOGGLE()			( GPIO_WriteBit( GPIOD, GPIO_Pin_2, (BitAction)!(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2)) ) )

#elif defined (USE_STM32_CORE_512K_100P)
	#define LED0_ON()				(GPIO_WriteBit(GPIOC, GPIO_Pin_0, (BitAction)0))
	#define LED0_OFF()				(GPIO_WriteBit(GPIOC, GPIO_Pin_0, (BitAction)1))
	#define LED0_TOGGLE()			( GPIO_WriteBit( GPIOC, GPIO_Pin_0, (BitAction)!(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_0)) ) )
	#define LED1_ON()				(GPIO_WriteBit(GPIOC, GPIO_Pin_1, (BitAction)0))
	#define LED1_OFF()				(GPIO_WriteBit(GPIOC, GPIO_Pin_1, (BitAction)1))
	#define LED1_TOGGLE()			( GPIO_WriteBit( GPIOC, GPIO_Pin_1, (BitAction)!(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_1)) ) )
	#define LED2_ON()				(GPIO_WriteBit(GPIOC, GPIO_Pin_2, (BitAction)0))
	#define LED2_OFF()				(GPIO_WriteBit(GPIOC, GPIO_Pin_2, (BitAction)1))
	#define LED2_TOGGLE()			( GPIO_WriteBit( GPIOC, GPIO_Pin_2, (BitAction)!(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_2)) ) )
	#define LED3_ON()				(GPIO_WriteBit(GPIOC, GPIO_Pin_3, (BitAction)0))
	#define LED3_OFF()				(GPIO_WriteBit(GPIOC, GPIO_Pin_3, (BitAction)1))
	#define LED3_TOGGLE()			( GPIO_WriteBit( GPIOC, GPIO_Pin_3, (BitAction)!(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_3)) ) )
	#define LEDUSB_ON()				(GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)1))
	#define LEDUSB_OFF()			(GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)0))
	#define LEDUSB_TOGGLE()			( GPIO_WriteBit( GPIOD, GPIO_Pin_2, (BitAction)!(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_2)) ) )

#endif

#if defined USE_STM32_DQ_128K_64P
#elif defined USE_STM32_CORE_512K_100P
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
