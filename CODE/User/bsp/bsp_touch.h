#ifndef _BSP_TOUCH_H_
#define _BSP_TOUCH_H_

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//触摸屏驱动（支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046/GT9147/GT9271/FT5206等）代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/5/5
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									   
//********************************************************************************
//修改说明 
//无
////////////////////////////////////////////////////////////////////////////////// 

#define TP_PRES_DOWN 	0x8000  //触屏被按下	  
#define TP_CATH_PRES 	0x4000  //有按键按下了 
#define CT_MAX_TOUCH 	5    	//电容屏支持的点数,固定为5点

//触摸屏控制器
typedef struct
{
	u8 (*init)(void);			//初始化触摸屏控制器
	u16 (*scan)(u8);			//扫描触摸屏.0,屏幕扫描;1,物理坐标;	 
	void (*adjust)(void);		//触摸屏校准 
	u16 x[CT_MAX_TOUCH]; 		//当前坐标
	u16 y[CT_MAX_TOUCH];		//电容屏有最多10组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
								//x[9],y[9]存储第一次按下时的坐标. 
	u16 sta;					//笔的状态 
								//b15:按下1/松开0; 
	                            //b14:0,没有按键按下;1,有按键按下. 
								//b13~b10:保留
								//b9~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
/////////////////////触摸屏校准参数(电容屏不需要校准)//////////////////////								
	unsigned char fac_data_type;
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
//b0:0,竖屏(适合左右为X坐标,上下为Y坐标的TP)
//   1,横屏(适合左右为Y坐标,上下为X坐标的TP) 
//b1~6:保留.
//b7:0,电阻屏
//   1,电容屏 
	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//触屏控制器在touch.c里面定义


//电阻屏/电容屏 共用函数
unsigned char TP_Init(void);								//初始化

void BSP_ad7843_loop_1ms(void);
unsigned char BSP_ad7843_get_press(void);
void BSP_ad7843_get_position(unsigned short *x, unsigned short *y);


#endif

