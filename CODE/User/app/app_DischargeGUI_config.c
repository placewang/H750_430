#include "ui.h"
#include "main.h"
#include <stdio.h>
#include "app_delta_servo.h"
#include "app_DischargeSystem.h"
#include "app_balanceRoll_code.h"
extern uint8_t UpaxisEnb;
extern uint8_t DownaxisEnb;



/*
�Ϸ��ϵ㶯�ƶ�
dir:0:��ת 1:��ת
sta:1:���� 0:ֹͣ
*/

void Up_DischargeJog(uint8_t dir,uint8_t sta)
{
    signed speed=0;
    if(Discharge.UpJogSpeed<=0)
        return;
    if(sta==1)
    {
        speed=dir==0?Discharge.UpJogSpeed:-(Discharge.UpJogSpeed);
        speed=Discharge.UpDir==0?speed:-speed;
        DELTA_Servo_SetTargetSpeed(UP_NODE_ID,speed);
    }
    else
    {
       DELTA_Servo_SetTargetSpeed(UP_NODE_ID,0); 
    }
}

/*
�·��ϵ㶯�ƶ�
dir:0:��ת 1:��ת
sta:1:���� 0:ֹͣ
*/

void Down_DischargeJog(uint8_t dir,uint8_t sta)
{
    signed speed=0;
    if(Discharge.DownJogSpeed<=0)
        return;
    if(sta==1)
    {
        speed=dir==0?Discharge.DownJogSpeed:-(Discharge.DownJogSpeed);
        speed=Discharge.DownDir ==0?speed:-speed;
        DELTA_Servo_SetTargetSpeed(DOWN_NODE_ID,speed);
    }
    else
    {
       DELTA_Servo_SetTargetSpeed(DOWN_NODE_ID,0); 
    }
}
/*
ֹͣ��ʼ��׼��
*/
void Up_DischargeStop(void)
{ 
    DELTA_Servo_SetTargetSpeed(UP_NODE_ID,0);
}
void Down_DischargeStop(void)
{
    DELTA_Servo_SetTargetSpeed(DOWN_NODE_ID,0);   
    
}

/*
�Ϸ�������������/�ͷ�
enb:1:���� 0���ͷ�
*/

void Up_ServoAxisLockOrRelease(unsigned char enb)
{   
    DELTA_Servo_enable(UP_NODE_ID,enb);
}

/*
�·�������������/�ͷ�
enb:1:���� 0���ͷ�
*/

void Down_ServoAxisLockOrRelease(unsigned char enb)
{   
    DELTA_Servo_enable(DOWN_NODE_ID,enb);
}


