
#ifndef __TOUCH_H__ 
#define __TOUCH_H__ 

#include "stm32f10x_lib.h"

//Mini STM32 ������ 
//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 �������� 
//����ԭ��@ALIENTEK 
//2010/6/13 
//V1.3  
     
//����״̬   
#define Key_Down 0x01 
#define Key_Up   0x00 
 
//�ʸ˽ṹ�� 
typedef struct   
{ 
	u16 X0;//LCD����  
	275
	u16 Y0; 
	u16 X; //��������/�ݴ����� 
	u16 Y;               
	u8  Key_Sta;//�ʵ�״̬      
	//������У׼���� 
	float xfac; 
	float yfac; 
	short xoff; 
	short yoff; 
}Pen_Holder;
     
extern Pen_Holder Pen_Point; 
//�봥����оƬ�������� 
     
#define PEN  PCin(1)   //PC1  INT 
#define DOUT PCin(2)   //PC2  MISO 
#define TDIN PCout(3)  //PC3  MOSI 
#define TCLK PCout(0)  //PC0  SCLK 
#define TCS  PCout(13) //PC13 CS 
    
//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 ָ� 
#define CMD_RDY 0X90    //0B10010000 ���ò�ַ�ʽ�� X���� 
#define CMD_RDX  0XD0    //0B11010000 ���ò�ַ�ʽ�� Y����          
         
#define TEMP_RD 0XF0    //0B11110000 ���ò�ַ�ʽ�� Y���� 
    
//ʹ�ñ��� 
#define ADJ_SAVE_ENABLE           
void Touch_Init(void);      //��ʼ�� 
u8 Read_ADS(u16 *x,u16 *y);    //��������˫�����ȡ 
u8 Read_ADS2(u16 *x,u16 *y); //����ǿ�˲���˫���������ȡ 
u16 ADS_Read_XY(u8 xy);      //���˲��������ȡ(������) 
u16 ADS_Read_AD(u8 CMD);    //��ȡ ADת��ֵ 
void ADS_Write_Byte(u8 num); //�����оƬд��һ������ 
void Drow_Touch_Point(u8 x,u16 y);//��һ�������׼�� 
void Draw_Big_Point(u8 x,u16 y);    //��һ����� 
void Touch_Adjust(void);          //������У׼ 
void Save_Adjdata(void);      //����У׼���� 
u8 Get_Adjdata(void);      //��ȡУ׼���� 
void Pen_Int_Set(u8 en);       //PEN�ж�ʹ��/�ر� 
void Convert_Pos(void);           //���ת������