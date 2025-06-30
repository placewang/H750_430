#include <string.h>
#include <stdio.h>
#include "ltdc.h"
#include "gpio.h"
#include "font.h"
#include "stm32h7xx_hal.h"

#include "bsp_lcd.h"
#include "bsp_sdram.h"

extern LTDC_HandleTypeDef hltdc;

unsigned char G_reload_flag = 0;

#ifdef BSP_SDRAM_NOT_USE
//unsigned short *LCD_frontbuf;
unsigned short LCD_frontbuf[LCD_WIDTH * (LCD_HEIGHT)]   __attribute__((at(0x24000000)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
//unsigned short LCD_front2buf[LCD_WIDTH * (LCD_HEIGHT)]   __attribute__((at(0x24040000)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
//unsigned short LCD_frontbuf[LCD_WIDTH * (LCD_HEIGHT)]   __attribute__((at(0x30000000)));
#else	// BSP_SDRAM_NOT_USE
unsigned short LCD_frontbuf[LCD_WIDTH * (LCD_HEIGHT)]   __attribute__((at(0xC0000000)));	//定义最大屏分辨率时,LCD所需的帧缓存数组大小
#endif	// BSP_SDRAM_NOT_USE

#define RTGRAPHIC_CTRL_RECT_UPDATE      0
#define RTGRAPHIC_CTRL_GET_INFO         1

//LCD的画笔颜色和背景色	   
unsigned int POINT_COLOR=0xFF000000;	//画笔颜色
unsigned int BACK_COLOR =0xFFFFFFFF;  	//背景色 

//static unsigned char G_cur_buf = 0;

//extern void LV_disp_cmpt(void);
void HAL_LTDC_ReloadEventCallback(LTDC_HandleTypeDef *hltdc_p)
{
	G_reload_flag = 0;
//	LV_disp_cmpt();
}

void HAL_LTDC_LineEventCallback(LTDC_HandleTypeDef *hltdc_p)
{
    
}

void LTDC_Draw_Point(unsigned short x, unsigned short y, unsigned short color)
{
	LCD_frontbuf[y * LCD_WIDTH + x] = color;
}

//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色
void LTDC_Fill(unsigned short sx,unsigned short sy,unsigned short ex,unsigned short ey,unsigned short color)
{
	if ((sx > ex) || (sy > ey) || (ex >= LCD_WIDTH) || (ey >= LCD_HEIGHT))
	{
		return;
	}
	
	for (int j = sy; j <= ey; j++)
	{
		for (int i = sx; i <= ex; i++)
		{
			LCD_frontbuf[j * LCD_WIDTH + i] = color;
		}
	}
}


//画点
//x,y:坐标
//POINT_COLOR:此点的颜色
void LCD_DrawPoint(unsigned short x, unsigned short y)
{ 
	LTDC_Draw_Point(x,y,POINT_COLOR);
}

//LCD清屏
//color:颜色值
void LCD_Clear(unsigned short color)
{
	LTDC_Fill(0,0,LCD_WIDTH-1,LCD_HEIGHT-1,color);
}

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
	unsigned short t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}  


//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(unsigned short x0,unsigned short y0,unsigned char r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  	LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 	


//在指定区域内填充指定颜色块,DMA2D填充	
//此函数仅支持u16,RGB565格式的颜色数组填充.
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)  
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色数组首地址
void LTDC_Color_Fill(unsigned short sx,unsigned short sy,unsigned short ex,unsigned short ey,unsigned short *color)
{
	unsigned int psx,psy,pex,pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
	unsigned int timeout=0; 
	unsigned short offline;
	unsigned int addr; 
	//坐标系转换
	psx=sx;psy=sy;
	pex=ex;pey=ey;

	offline=LCD_WIDTH-(pex-psx+1);
	//addr=((unsigned int)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	addr = (unsigned int)&LCD_frontbuf[psy * LCD_WIDTH + psx];
	RCC->AHB3ENR|=1<<4;				//使能DMA2D时钟
	DMA2D->CR&=~(1<<0);				//先停止DMA2D
	DMA2D->CR=0<<16;				//存储器到存储器模式
	DMA2D->FGPFCCR=LCD_PIXFORMAT_RGB565;	//设置颜色格式
	DMA2D->FGOR=0;					//前景层行偏移为0
	DMA2D->OOR=offline;				//设置行偏移 
	DMA2D->FGMAR=(unsigned int)color;		//源地址
	DMA2D->OMAR=addr;				//输出存储器地址
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//设定行数寄存器 
	DMA2D->CR|=1<<0;				//启动DMA2D
	while((DMA2D->ISR&(1<<1))==0)	//等待传输完成
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//超时退出
	}
	DMA2D->IFCR|=1<<1;				//清除传输完成标志
}  

//在指定区域内填充指定颜色块,DMA2D填充	
//此函数仅支持u16,RGB565格式的颜色数组填充.
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)  
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色数组首地址
void LTDC_Color_Fill2(unsigned short sx, unsigned short sy, unsigned short ex, unsigned short ey, unsigned short *color_p)
{
	unsigned short color;

	if ((sx > ex) || (sy > ey) || (ex >= LCD_WIDTH) || (ey >= LCD_HEIGHT))
	{
		return;
	}
	
	for (int j = sy; j <= ey; j++)
	{
		for (int i = sx; i <= ex; i++)
		{
			color = *color_p;
			LCD_frontbuf[j * LCD_WIDTH + i] = color;
			color_p++;
			
		}
	}
} 
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(unsigned short x,unsigned short y,unsigned short color)
{	   
	LTDC_Draw_Point(x,y,color);
}	

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned char size, unsigned char mode)
{  							  
  unsigned char temp,t1,t;
	unsigned short y0=y;
	unsigned char csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=LCD_HEIGHT)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LCD_WIDTH)return;	//超区域了
				break;
			}
		}  	 
	}  	    	   	 	  
}  


//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(unsigned short x,unsigned short y,unsigned short width,unsigned short height,unsigned char size,unsigned char *p)
{         
	unsigned char x0=x;
	width+=x;
	height+=y;
	while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
	{       
			if(x>=width){x=x0;y+=size;}
			if(y>=height)break;//退出
			LCD_ShowChar(x,y,*p,size,0);
			x+=size/2;
			p++;
	}
}
//m^n函数
//返回值:m^n次方.
unsigned int LCD_Pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}
//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);	 
void LCD_ShowNum(unsigned short x,unsigned short y,unsigned int num,unsigned char len,unsigned char size)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 

void BSP_LCD_backlight_enable(unsigned char enable)
{
	if (enable)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	}
}

/*****************************************************************
Uart_thread:
	串口数据收发处理线程。。。
******************************************************************/
void BSP_LCD_init(void)
{
	//LCD_frontbuf = (unsigned short *)0x24000000;
	memset(LCD_frontbuf, 0xFF, LCD_BUF_SIZE);
//	memset(LCD_backbuf, 0xFF, LCD_BUF_SIZE);
//	memset(LCD_framebuf, 0xFF, LCD_BUF_SIZE);
	
	//LTDC_init();

	printf("LTDC_init ..........\r\n");
	
	
	
//	while(1)
//	{
//		h(3000);	// 目前的设
//		LTDC_Clear(COLOR_BLUE);
//		rt_thread_mdelay(3000);	// 目前的设
//		LTDC_Clear(COLOR_RED);
//		rt_thread_mdelay(3000);	// 目前的设
//		LTDC_Clear(COLOR_YELLOW);
//		
//		
//	}
}

void BSP_LCD_loop_1ms(void)
{
//	static int count = 0;
//	count++;
//	if (count == 3000)
//	{
//		LTDC_Clear(COLOR_BLUE);
//	}
//	else if (count == 6000)
//	{
//		LTDC_Clear(COLOR_RED);
//	}
//	else if (count >= 9000)
//	{
//		count = 0;
//		LTDC_Clear(COLOR_YELLOW);
//	}
	
//	if (G_refresh_timer_count > 0)
//	{
//		G_refresh_timer_count--;
//	}
	
}

//void BSP_LCD_set_fb_startadress_IT(unsigned int fb)
//{
//	HAL_LTDC_SetAddress_NoReload(&hltdc, fb, LTDC_LAYER_1);
//	HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);   // 开启重装载中断
//	G_reload_flag = 1;
//}

//int BSP_LCD_if_fb_reload_cmpt(void)
//{
//	if (G_reload_flag == 0)
//	{
//		return 1;
//	}
//}
