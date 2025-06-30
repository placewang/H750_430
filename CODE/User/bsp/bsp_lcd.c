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
unsigned short LCD_frontbuf[LCD_WIDTH * (LCD_HEIGHT)]   __attribute__((at(0x24000000)));	//����������ֱ���ʱ,LCD�����֡���������С
//unsigned short LCD_front2buf[LCD_WIDTH * (LCD_HEIGHT)]   __attribute__((at(0x24040000)));	//����������ֱ���ʱ,LCD�����֡���������С
//unsigned short LCD_frontbuf[LCD_WIDTH * (LCD_HEIGHT)]   __attribute__((at(0x30000000)));
#else	// BSP_SDRAM_NOT_USE
unsigned short LCD_frontbuf[LCD_WIDTH * (LCD_HEIGHT)]   __attribute__((at(0xC0000000)));	//����������ֱ���ʱ,LCD�����֡���������С
#endif	// BSP_SDRAM_NOT_USE

#define RTGRAPHIC_CTRL_RECT_UPDATE      0
#define RTGRAPHIC_CTRL_GET_INFO         1

//LCD�Ļ�����ɫ�ͱ���ɫ	   
unsigned int POINT_COLOR=0xFF000000;	//������ɫ
unsigned int BACK_COLOR =0xFFFFFFFF;  	//����ɫ 

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

//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ
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


//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(unsigned short x, unsigned short y)
{ 
	LTDC_Draw_Point(x,y,POINT_COLOR);
}

//LCD����
//color:��ɫֵ
void LCD_Clear(unsigned short color)
{
	LTDC_Fill(0,0,LCD_WIDTH-1,LCD_HEIGHT-1,color);
}

//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
	unsigned short t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
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


//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(unsigned short x0,unsigned short y0,unsigned char r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 	


//��ָ�����������ָ����ɫ��,DMA2D���	
//�˺�����֧��u16,RGB565��ʽ����ɫ�������.
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)  
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ�����׵�ַ
void LTDC_Color_Fill(unsigned short sx,unsigned short sy,unsigned short ex,unsigned short ey,unsigned short *color)
{
	unsigned int psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	unsigned int timeout=0; 
	unsigned short offline;
	unsigned int addr; 
	//����ϵת��
	psx=sx;psy=sy;
	pex=ex;pey=ey;

	offline=LCD_WIDTH-(pex-psx+1);
	//addr=((unsigned int)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	addr = (unsigned int)&LCD_frontbuf[psy * LCD_WIDTH + psx];
	RCC->AHB3ENR|=1<<4;				//ʹ��DMA2Dʱ��
	DMA2D->CR&=~(1<<0);				//��ֹͣDMA2D
	DMA2D->CR=0<<16;				//�洢�����洢��ģʽ
	DMA2D->FGPFCCR=LCD_PIXFORMAT_RGB565;	//������ɫ��ʽ
	DMA2D->FGOR=0;					//ǰ������ƫ��Ϊ0
	DMA2D->OOR=offline;				//������ƫ�� 
	DMA2D->FGMAR=(unsigned int)color;		//Դ��ַ
	DMA2D->OMAR=addr;				//����洢����ַ
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//�趨�����Ĵ��� 
	DMA2D->CR|=1<<0;				//����DMA2D
	while((DMA2D->ISR&(1<<1))==0)	//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//��ʱ�˳�
	}
	DMA2D->IFCR|=1<<1;				//���������ɱ�־
}  

//��ָ�����������ָ����ɫ��,DMA2D���	
//�˺�����֧��u16,RGB565��ʽ����ɫ�������.
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)  
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ�����׵�ַ
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
//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(unsigned short x,unsigned short y,unsigned short color)
{	   
	LTDC_Draw_Point(x,y,color);
}	

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24/32
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned char size, unsigned char mode)
{  							  
  unsigned char temp,t1,t;
	unsigned short y0=y;
	unsigned char csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else if(size==32)temp=asc2_3216[num][t];	//����3216����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=LCD_HEIGHT)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=LCD_WIDTH)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}  


//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(unsigned short x,unsigned short y,unsigned short width,unsigned short height,unsigned char size,unsigned char *p)
{         
	unsigned char x0=x;
	width+=x;
	height+=y;
	while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
	{       
			if(x>=width){x=x0;y+=size;}
			if(y>=height)break;//�˳�
			LCD_ShowChar(x,y,*p,size,0);
			x+=size/2;
			p++;
	}
}
//m^n����
//����ֵ:m^n�η�.
unsigned int LCD_Pow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
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
	���������շ������̡߳�����
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
//		h(3000);	// Ŀǰ����
//		LTDC_Clear(COLOR_BLUE);
//		rt_thread_mdelay(3000);	// Ŀǰ����
//		LTDC_Clear(COLOR_RED);
//		rt_thread_mdelay(3000);	// Ŀǰ����
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
//	HAL_LTDC_Reload(&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);   // ������װ���ж�
//	G_reload_flag = 1;
//}

//int BSP_LCD_if_fb_reload_cmpt(void)
//{
//	if (G_reload_flag == 0)
//	{
//		return 1;
//	}
//}
