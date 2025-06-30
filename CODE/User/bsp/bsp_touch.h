#ifndef _BSP_TOUCH_H_
#define _BSP_TOUCH_H_

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32H7������
//������������֧��ADS7843/7846/UH7843/7846/XPT2046/TSC2046/GT9147/GT9271/FT5206�ȣ�����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/5/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									   
//********************************************************************************
//�޸�˵�� 
//��
////////////////////////////////////////////////////////////////////////////////// 

#define TP_PRES_DOWN 	0x8000  //����������	  
#define TP_CATH_PRES 	0x4000  //�а��������� 
#define CT_MAX_TOUCH 	5    	//������֧�ֵĵ���,�̶�Ϊ5��

//������������
typedef struct
{
	u8 (*init)(void);			//��ʼ��������������
	u16 (*scan)(u8);			//ɨ�败����.0,��Ļɨ��;1,��������;	 
	void (*adjust)(void);		//������У׼ 
	u16 x[CT_MAX_TOUCH]; 		//��ǰ����
	u16 y[CT_MAX_TOUCH];		//�����������10������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
								//x[9],y[9]�洢��һ�ΰ���ʱ������. 
	u16 sta;					//�ʵ�״̬ 
								//b15:����1/�ɿ�0; 
	                            //b14:0,û�а�������;1,�а�������. 
								//b13~b10:����
								//b9~b0:���ݴ��������µĵ���(0,��ʾδ����,1��ʾ����)
/////////////////////������У׼����(����������ҪУ׼)//////////////////////								
	unsigned char fac_data_type;
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//�����Ĳ���,��������������������ȫ�ߵ�ʱ��Ҫ�õ�.
//b0:0,����(�ʺ�����ΪX����,����ΪY�����TP)
//   1,����(�ʺ�����ΪY����,����ΪX�����TP) 
//b1~6:����.
//b7:0,������
//   1,������ 
	u8 touchtype;
}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��


//������/������ ���ú���
unsigned char TP_Init(void);								//��ʼ��

void BSP_ad7843_loop_1ms(void);
unsigned char BSP_ad7843_get_press(void);
void BSP_ad7843_get_position(unsigned short *x, unsigned short *y);


#endif

