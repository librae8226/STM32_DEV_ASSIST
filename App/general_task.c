/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name          : general_task.c
* Author             : Librae
* Last Modified Date : 11/10/2010
* Description        : This file contains the general and miscellaneous task.

******************************************************************************/

/* Includes -----------------------------------------------------------------*/
//#include "includes.h"
#include <stdio.h>
#include "uCOS_II.H"
#include "platform_config.h"
#include "LTM022A69B.h"
#include "ffext.h"
#include "NRF24L01.h"
#include "servo.h"
#include "general_task.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
FATFS fs;
char wbuff[] = "This is FATFS function test\r\n * f_open\r\n * f_write\r\n * f_close\r\n";
char wbuff2[512] = {0};
XCHAR *path = 0;

s16  Temp = 0;
u32  Volt = 0;

vu16 AD_Value[2];

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

void TaskGeneral(void * pdata)
{
	u16 max = 0;
	u16 min = 5000;
	pdata = pdata;

	printf( "<General Task Experiment>\n\r" );
	LCD_DispString( "<General Task Experiment>", WHITE, BLACK, 0, 0 );

	//SD_Test();
	//Servo_Test();

	for(;;)
	{
		Volt = GetVolt(AD_Value[0]);
		max = Volt > max ? Volt : max;
		min = Volt < min ? Volt : min;
		printf("Voltage: %d.%03d\r\n", Volt/1000, Volt%1000);
//		printf("Voltage: %d\tmax: %d | min :%d\trange: %d\r\n", Volt, max, min, max-min);
//		NRF_Receive();
		OSTimeDlyHMSM(0,0,0,200);
	}
}

void ADC_Test(void)
{
}

void SD_Test(void)
{
	int j;
	ffext_mount(0, &fs);
	ffext_scanfiles(path);

	for(j=0; j<512; j++)
	{
		wbuff2[j] = 'a';
	}

	ffext_write("0:CardInfo.txt", wbuff, sizeof(wbuff)-1, FA_OPEN_ALWAYS | FA_WRITE);
	ffext_write("0:ffext2.ini", wbuff2, (512), FA_OPEN_ALWAYS | FA_WRITE);

	for(j=0; j<512; j++)
	{
		wbuff2[j] = '0';
	}

	ffext_read("0:ffext2.ini", wbuff2, (512), FA_OPEN_ALWAYS | FA_READ);
	ffext_write("0:ffext1.txt", wbuff2, (512), FA_OPEN_ALWAYS | FA_WRITE);
}

/*******************************************************************************
* Function Name  : GetTemp
* Description    : ����ADC��������¶�
* Input          : u16 advalue
* Output         : 
* Return         : u16 temp
*******************************************************************************/
u16 GetTemp(u16 advalue)
{
    u32 Vtemp_sensor;
    s32 Current_Temp;
    
//    ADCת�������Ժ󣬶�ȡADC_DR�Ĵ����еĽ����ת���¶�ֵ���㹫ʽ���£�
//          V25 - VSENSE
//  T(��) = ------------  + 25
//           Avg_Slope
//   V25��  �¶ȴ�������25��ʱ �������ѹ������ֵ1.43 V��
//  VSENSE���¶ȴ������ĵ�ǰ�����ѹ����ADC_DR �Ĵ����еĽ��ADC_ConvertedValue֮���ת����ϵΪ�� 
//            ADC_ConvertedValue * Vdd
//  VSENSE = --------------------------
//            Vdd_convert_value(0xFFF)
//  Avg_Slope���¶ȴ����������ѹ���¶ȵĹ�������������ֵ4.3 mV/�档

    Vtemp_sensor = advalue * 330 / 4096;
    Current_Temp = (s32)(143 - Vtemp_sensor)*10000/43 + 2500; 
    return (s16)Current_Temp;
}  


/*******************************************************************************
* Function Name  : GetVolt
* Description    : ����ADC��������ѹ
* Input          : u16 advalue
* Output         : 
* Return         : u16 temp
*******************************************************************************/
u16 GetVolt(u16 advalue)
{
    return (u16)(advalue * 3300 / 4096);
} 

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
