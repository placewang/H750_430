#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "m_set.h"
#include "bsp_encoder.h"
#include "app_RowknifeSystem.h"

static void RowKnifeMotorIint(void)
{
  appRowKnifeMotorController_Init(&motor_ctrl); 
}
static void getMoveDistance(float* mdec)
{
   *mdec= Rowknife_S.MoveDistance;     
}

static void getMovePos(int* pos)
{
   *pos= Rowknife_S.MovePos;     
}
static void getMoveSpeed(int* speed)
{
   *speed=Rowknife_S.MoveSpeed;    
}
static void getRealTimePos(int* pos)
{
     *pos=Rowknife_S.RealTimePos;
}
static void getRealTimeSpeed(int* speed)
{
     *speed=Rowknife_S.RealTimeSpeed;    
}
static void getEncdeVale(int* encd)
{
     *encd=BSP_ENCODER_getSteps(motor_ctrl.EndLast,BSP_getENCODER());
//     if(*encd-Rowknife_S.EndActualPos>2)
//     { 
//        *encd-=4;
//     }    
}
static int getExeTime(void)
{
     return Rowknife_S.Exetime;    
}
/*******************************************/
//
static void setEncdeVale(int endc)
{
    BSP_ENCODER_setCounter(endc);   
}
#define _f_Move  0.3f
#define _f_Move2 0.5f
static void setMovePos(float* pos)
{   
    int e_pos=0, v_pos=0;
//    Rowknife_S.speed=(unsigned short)(((*pos/_MinPrecision)/90.0f)*2500.0f);
//    Rowknife_S.speed_1= Rowknife_S.speed/2;
//    Rowknife_S.speed_2= Rowknife_S.speed/6;
    Rowknife_S.EndActualPos=((int)(*pos/_EndeMinPrecision));
    Rowknife_S.Actual_1=(int)(((*pos*_f_Move)/_EndeMinPrecision)+0.5f);
    Rowknife_S.Actual_2=(int)(((*pos*_f_Move2)/_EndeMinPrecision));
}
static void setMoveSpeed(int* speed)
{
     Rowknife_S.MoveSpeed=*speed;    
}
/******************************************/
static void MotorMovePos(char cmd,int MovePos)
{

   if(MovePos>=32767)
       MovePos=32767; 
   appRowKnifeMovePos(cmd,MovePos);  
}
static void MotorStart(char flag)
{
    Rowknife_S.StartFlag=flag;
}
Model_set m_set={
                    .RowKnifeMotorIint=RowKnifeMotorIint,
                    .getExeTime=getExeTime,
                    .ReductionRatio=_ReductionRatio,
                    .MinimumPrecision=_MinPrecision,
                    .MotorStart=MotorStart,
                    .getMovePos=getMovePos,
                    .getMoveSpeed=getMoveSpeed,
                    .MotorMovePos=MotorMovePos,
                    .setEncdeVale =setEncdeVale,
                    .setMovePos   = setMovePos,
                    .setMoveSpeed = setMoveSpeed,
                    .getEncde=getEncdeVale,
                    .getRealTimePos=getRealTimePos,
                    .getMoveDistance=getMoveDistance,
                    .getRealTimeSpeed=getRealTimeSpeed 
                 };









