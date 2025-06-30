#include "ui.h"
#include "main.h"
#include <stdio.h>
#include "app_delta_servo.h"
#include "app_DischargeSystem.h"
#include "app_balanceRoll_code.h"
extern uint8_t UpaxisEnb;
extern uint8_t DownaxisEnb;



/*
上放料点动移动
dir:0:正转 1:反转
sta:1:启动 0:停止
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
下放料点动移动
dir:0:正转 1:反转
sta:1:启动 0:停止
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
停止初始化准备
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
上放料气胀轴锁定/释放
enb:1:锁定 0：释放
*/

void Up_ServoAxisLockOrRelease(unsigned char enb)
{   
    DELTA_Servo_enable(UP_NODE_ID,enb);
}

/*
下放料气胀轴锁定/释放
enb:1:锁定 0：释放
*/

void Down_ServoAxisLockOrRelease(unsigned char enb)
{   
    DELTA_Servo_enable(DOWN_NODE_ID,enb);
}


