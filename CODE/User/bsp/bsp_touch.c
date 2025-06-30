#include <string.h>
#include <stdio.h>
#include "gpio.h"
#include "stm32h7xx_hal.h"
#include "bsp_touch.h"
#include "bsp_lcd.h"
#include "bsp_mx25l_spi.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bsp_ad7843.h"

#define PEN		  		HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_11)  		//T_PEN


u16 TP_Scan(u8 tp);


_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	0,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};					
//默认为touchtype=0的数据.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;
 	 	 

//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 5 	//读取次数
#define LOST_VAL 1	  	//丢弃值
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)
	{		 
		buf[i] = AD7843_read_value(xy);			
	}
	
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
	{
		sum+=buf[i];
	}
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}
//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
#define ERR_RANGE 50 //误差范围 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  
 
extern void UI_my_deal_lock_time_feed(void);
unsigned char TP_is_press(void)
{
	if(PEN==0)//有按键按下
	{
		UI_my_deal_lock_time_feed();
		return true;
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////////////		  
//触摸按键扫描
//tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u16 TP_Scan(u8 tp)
{
	if(tp)
	{
		TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);//读取物理坐标
		//printf("TP_Scan x=%d, y=%d\r\n", tp_dev.x[0], tp_dev.y[0]);
	}
	else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))//读取屏幕坐标
	{
		//printf("TP_Scan x=%d, y=%d\r\n", tp_dev.x[0], tp_dev.y[0]);
		//printf("tp_dev.xfac=%f, tp_dev.xoff=%d\r\n", tp_dev.xfac, tp_dev.xoff);
		//printf("tp_dev.yfac=%f, tp_dev.yoff=%d\r\n", tp_dev.xfac, tp_dev.yoff);
		//tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//将结果转换为屏幕坐标
		//tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
		//printf("TP_Scan x=%d, y=%d\r\n", tp_dev.x[0], tp_dev.y[0]);
		
		if (tp_dev.fac_data_type == 0x0A)
		{// 老方式
			tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//将结果转换为屏幕坐标
			tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
		}
		else if (tp_dev.fac_data_type == 0x0B)
		{// 新方式
			tp_dev.x[0] = (tp_dev.x[0] - tp_dev.xoff) / tp_dev.xfac + 240;//将结果转换为屏幕坐标
			tp_dev.y[0] = (tp_dev.y[0] - tp_dev.yoff) / tp_dev.yfac + 136;
		}
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////	 
//保存在EEPROM里面的地址区间基址,占用13个字节(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE BSP_SAVE_DATA_BASE_ADDRESS + 40
//得到保存在EEPROM里面的校准值
//返回值：1，成功获取数据
//        0，获取失败，要重新校准
u8 TP_Get_Adjdata(void)
{					  
	int tempfac;
	
	tempfac = MX25L_SPI_read_byte(SAVE_ADDR_BASE+13,1);
	if ((tempfac == 0x0A) || (tempfac == 0x0B))//触摸屏已经校准过了			    
	{
		tp_dev.fac_data_type = tempfac;
		tempfac=MX25L_SPI_read_byte(SAVE_ADDR_BASE,4);
		tp_dev.xfac=(float)tempfac/100000000;//得到x校准参数
			
		tempfac=MX25L_SPI_read_byte(SAVE_ADDR_BASE+4,4);			
		tp_dev.yfac=(float)tempfac/100000000;//得到y校准参数
		
	    //得到x偏移量
		tp_dev.xoff = MX25L_SPI_read_byte(SAVE_ADDR_BASE+8,2);
		
 	    //得到y偏移量
		tp_dev.yoff=MX25L_SPI_read_byte(SAVE_ADDR_BASE+10,2);	
		
		tp_dev.touchtype=MX25L_SPI_read_byte(SAVE_ADDR_BASE+12,1);

		if(tp_dev.touchtype)//X,Y方向与屏幕相反
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	 
		}else				   //X,Y方向与屏幕相同
		{
			CMD_RDX=0XD0;
			CMD_RDY=0X90;	 
		}		 
		return 1;	 
	}
	return 0;
} 

unsigned char G_touch_press = 0;
unsigned short G_X_position;
unsigned short G_Y_position;

//触摸屏初始化  		    
//返回值:0,没有进行校准
//       1,进行过校准
unsigned char TP_Init(void)
{	  
	TP_Read_XY(&tp_dev.x[0],&tp_dev.y[0]);//第一次读取初始化	 
	TP_Get_Adjdata();	// 获取校准参数
	
	G_X_position = 0xFFFF;
	G_Y_position = 0xFFFF;
	
	return 0;
}



unsigned short BSP_TOUCH_get_max_offset(unsigned short x, unsigned short y)
{
	unsigned short offset_x;
	unsigned short offset_y;
    
	if(G_X_position == 0xFFFF)
	{
		return 0;
	}
	
	if(G_X_position > x)
	{
		offset_x = G_X_position - x;
	}
	else
	{
		offset_x = x - G_X_position;
	}
	
	if (G_Y_position > y)
	{
		offset_y = G_Y_position - y;
	}
	else
	{
		offset_y = y - G_Y_position;
	}
	
	return offset_y + offset_x;
}

void BSP_ad7843_loop_1ms(void)
{
	static unsigned char count = 0;
	unsigned char read_press;
	static unsigned char press_delay;
	static unsigned char position_delay;
	unsigned short offset = 0;
	
	count++;
	if (count > 10)
	{
		count = 0;
		read_press = 0;
		if (TP_is_press() == true)
		{
			read_press = 1;
		}

		if (read_press != G_touch_press)
		{
			if (read_press)
			{
				press_delay++;
				if (press_delay >= 5)
				{
					G_touch_press = read_press;
					//printf("press = %d\r\n", G_touch_press);
				}
			}
			else
			{
				G_touch_press = read_press;
				G_X_position = 0xFFFF;
				G_Y_position = 0xFFFF;
			}
		}
		else
		{
			press_delay = 0;
		}

		if (G_touch_press == 1)
		{
			TP_Scan(0);
			offset = BSP_TOUCH_get_max_offset(tp_dev.x[0], tp_dev.y[0]);
			if (offset <= 30)
			{
				G_X_position = tp_dev.x[0];
				G_Y_position = tp_dev.y[0];
				position_delay = 0;
			}
			else
			{
				position_delay++;
				if (position_delay >= 3)
				{
					G_X_position = tp_dev.x[0];
					G_Y_position = tp_dev.y[0];	
					position_delay = 0;
				}
			}
			
			//printf("X=%d,Y=%d\r\n",G_X_position,G_Y_position);
		}
	}
}

unsigned char BSP_ad7843_get_press(void)
{
	return G_touch_press;
}

void BSP_ad7843_get_position(unsigned short *x, unsigned short *y)
{
	*x = G_X_position;
	*y = G_Y_position;
}

