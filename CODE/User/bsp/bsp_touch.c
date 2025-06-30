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
//Ĭ��Ϊtouchtype=0������.
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;
 	 	 

//��ȡһ������ֵ(x����y)
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
//xy:ָ�CMD_RDX/CMD_RDY��
//����ֵ:����������
#define READ_TIMES 5 	//��ȡ����
#define LOST_VAL 1	  	//����ֵ
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
	
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
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
//��ȡx,y����
//��Сֵ��������100.
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);	  												   
	//if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}
//����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���
//ERR_RANGE,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
#define ERR_RANGE 50 //��Χ 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
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
	if(PEN==0)//�а�������
	{
		UI_my_deal_lock_time_feed();
		return true;
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////////////		  
//��������ɨ��
//tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u16 TP_Scan(u8 tp)
{
	if(tp)
	{
		TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);//��ȡ��������
		//printf("TP_Scan x=%d, y=%d\r\n", tp_dev.x[0], tp_dev.y[0]);
	}
	else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))//��ȡ��Ļ����
	{
		//printf("TP_Scan x=%d, y=%d\r\n", tp_dev.x[0], tp_dev.y[0]);
		//printf("tp_dev.xfac=%f, tp_dev.xoff=%d\r\n", tp_dev.xfac, tp_dev.xoff);
		//printf("tp_dev.yfac=%f, tp_dev.yoff=%d\r\n", tp_dev.xfac, tp_dev.yoff);
		//tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//�����ת��Ϊ��Ļ����
		//tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
		//printf("TP_Scan x=%d, y=%d\r\n", tp_dev.x[0], tp_dev.y[0]);
		
		if (tp_dev.fac_data_type == 0x0A)
		{// �Ϸ�ʽ
			tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//�����ת��Ϊ��Ļ����
			tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
		}
		else if (tp_dev.fac_data_type == 0x0B)
		{// �·�ʽ
			tp_dev.x[0] = (tp_dev.x[0] - tp_dev.xoff) / tp_dev.xfac + 240;//�����ת��Ϊ��Ļ����
			tp_dev.y[0] = (tp_dev.y[0] - tp_dev.yoff) / tp_dev.yfac + 136;
		}
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////	 
//������EEPROM����ĵ�ַ�����ַ,ռ��13���ֽ�(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
#define SAVE_ADDR_BASE BSP_SAVE_DATA_BASE_ADDRESS + 40
//�õ�������EEPROM�����У׼ֵ
//����ֵ��1���ɹ���ȡ����
//        0����ȡʧ�ܣ�Ҫ����У׼
u8 TP_Get_Adjdata(void)
{					  
	int tempfac;
	
	tempfac = MX25L_SPI_read_byte(SAVE_ADDR_BASE+13,1);
	if ((tempfac == 0x0A) || (tempfac == 0x0B))//�������Ѿ�У׼����			    
	{
		tp_dev.fac_data_type = tempfac;
		tempfac=MX25L_SPI_read_byte(SAVE_ADDR_BASE,4);
		tp_dev.xfac=(float)tempfac/100000000;//�õ�xУ׼����
			
		tempfac=MX25L_SPI_read_byte(SAVE_ADDR_BASE+4,4);			
		tp_dev.yfac=(float)tempfac/100000000;//�õ�yУ׼����
		
	    //�õ�xƫ����
		tp_dev.xoff = MX25L_SPI_read_byte(SAVE_ADDR_BASE+8,2);
		
 	    //�õ�yƫ����
		tp_dev.yoff=MX25L_SPI_read_byte(SAVE_ADDR_BASE+10,2);	
		
		tp_dev.touchtype=MX25L_SPI_read_byte(SAVE_ADDR_BASE+12,1);

		if(tp_dev.touchtype)//X,Y��������Ļ�෴
		{
			CMD_RDX=0X90;
			CMD_RDY=0XD0;	 
		}else				   //X,Y��������Ļ��ͬ
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

//��������ʼ��  		    
//����ֵ:0,û�н���У׼
//       1,���й�У׼
unsigned char TP_Init(void)
{	  
	TP_Read_XY(&tp_dev.x[0],&tp_dev.y[0]);//��һ�ζ�ȡ��ʼ��	 
	TP_Get_Adjdata();	// ��ȡУ׼����
	
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

