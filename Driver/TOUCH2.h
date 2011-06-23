
#ifndef __TOUCH_H__ 
#define __TOUCH_H__ 

#include "stm32f10x_lib.h"

//Mini STM32 开发板 
//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 驱动函数 
//正点原子@ALIENTEK 
//2010/6/13 
//V1.3  
     
//按键状态   
#define Key_Down 0x01 
#define Key_Up   0x00 
 
//笔杆结构体 
typedef struct   
{ 
	u16 X0;//LCD坐标  
	275
	u16 Y0; 
	u16 X; //物理坐标/暂存坐标 
	u16 Y;               
	u8  Key_Sta;//笔的状态      
	//触摸屏校准参数 
	float xfac; 
	float yfac; 
	short xoff; 
	short yoff; 
}Pen_Holder;
     
extern Pen_Holder Pen_Point; 
//与触摸屏芯片连接引脚 
     
#define PEN  PCin(1)   //PC1  INT 
#define DOUT PCin(2)   //PC2  MISO 
#define TDIN PCout(3)  //PC3  MOSI 
#define TCLK PCout(0)  //PC0  SCLK 
#define TCS  PCout(13) //PC13 CS 
    
//ADS7843/7846/UH7843/7846/XPT2046/TSC2046 指令集 
#define CMD_RDY 0X90    //0B10010000 即用差分方式读 X坐标 
#define CMD_RDX  0XD0    //0B11010000 即用差分方式读 Y坐标          
         
#define TEMP_RD 0XF0    //0B11110000 即用差分方式读 Y坐标 
    
//使用保存 
#define ADJ_SAVE_ENABLE           
void Touch_Init(void);      //初始化 
u8 Read_ADS(u16 *x,u16 *y);    //带舍弃的双方向读取 
u8 Read_ADS2(u16 *x,u16 *y); //带加强滤波的双方向坐标读取 
u16 ADS_Read_XY(u8 xy);      //带滤波的坐标读取(单方向) 
u16 ADS_Read_AD(u8 CMD);    //读取 AD转换值 
void ADS_Write_Byte(u8 num); //向控制芯片写入一个数据 
void Drow_Touch_Point(u8 x,u16 y);//画一个坐标叫准点 
void Draw_Big_Point(u8 x,u16 y);    //画一个大点 
void Touch_Adjust(void);          //触摸屏校准 
void Save_Adjdata(void);      //保存校准参数 
u8 Get_Adjdata(void);      //读取校准参数 
void Pen_Int_Set(u8 en);       //PEN中断使能/关闭 
void Convert_Pos(void);           //结果转换函数