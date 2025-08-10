#include "bsp_dcan.h"
#include "bsp_encoder.h"
#include "app_sdo.h"
#include "app_pid.h"
#include "app_delta_servo.h"
#include "app_RowknifeSystem.h"
#include "app_HqServoProtocol.h"
#include <stdlib.h>

#define YC_CANSendID 0x352
#define YC_CANRevID 0x353
//放料运行参数
RowknifeServo Rowknife_S={
                            .speed=2682,
                            .speed_1=1341,
                            .speed_2=447,
                            .Actual_1=8,
                            .Actual_2=13,
                            .EndActualPos=27,
                            .MovePos=371,
                         };       
// 加解密
void commSecProc(short *data)
{
    unsigned char tmp[8];
    int i;
    int sdat;
    int idx = 7;

    for (i = 0; i < 4; i++)
    {
        tmp[i * 2] = data[i] & 0xFF;
        tmp[i * 2 + 1] = (data[i] >> 8) & 0xFF;
    }

    for (i = 0; i < 4; i++)
    {
        if (tmp[7] & (0x1 << (i * 2)))
        {
            idx = 2 + i;
            goto sec_it;
        }
    }
    for (i = 0; i < 4; i++)
    {
        if (tmp[7]&(0x2 << (i * 2)))
        {
            idx = 6 - i;
            goto sec_it;
        }
    }
    sec_it:
    sdat = tmp[idx]^tmp[7];
    for (i = 0; i < 7; i++)
    {
      if ((i == 1) ||(i == idx))
      {
        continue;
      }
      tmp[i] ^= sdat;
    }
    for (i = 0; i < 4; i++)
    {
        data[i] = tmp[i * 2] & 0xFF;
        data[i] |= (tmp[i * 2 + 1] & 0xFF) << 8;
    }
}                         
//CAN发送ID
#define  CAN_RevID    0x752    
int ntervalTime=0;  
void HqSlaveeturnLoopTask(void)
{
    CAN_RevTye Dpack={0}; 
    signed short upspeed=0;
    short downspeed=0;

    if(!CAN1_RevQueue_dequeue(&Can1RevQueue,&Dpack))
       return;      
    Rowknife_S.RevCnt=0; 
    if(Dpack.StdId==YC_CANRevID)
    {
        commSecProc((short *)Dpack.RxData);
    }
    if(Dpack.RxData[0]==0x13)
    {
        Rowknife_S.ReturnFlag=1;
    }
    //下放料实时速度放回
    else if(Dpack.RxData[0]==Cmd_TypeReadRunPmr&&Dpack.RxData[1]==DOWN_NODE_ID&&Dpack.RxData[2]==Rspeed)
    {
        downspeed =*((short *)&Dpack.RxData[3]);
        Rowknife_S.RealTimeSpeed=(downspeed>0?downspeed:-downspeed)/10;
    } 
    else if(Dpack.RxData[0]==Cmd_TypeReadRunPmr&&Dpack.RxData[1]==DOWN_NODE_ID&&Dpack.RxData[2]==Rspos)
    {
        Rowknife_S.RealTimePos =*((int *)&Dpack.RxData[3]);
    }      
       //下放料实时速度放回
    else if(Dpack.RxData[0]==Cmd_RunMode&&Dpack.RxData[1]==DOWN_NODE_ID&&Dpack.RxData[2]==SpeedMode)
    { 
           downspeed =*((short *)&Dpack.RxData[3]);
           Rowknife_S.RealTimeSpeed=(downspeed>0?downspeed:-downspeed)/10;
    }          
    else if(Dpack.RxData[0]==Cmd_Alarm&&Dpack.RxData[1]==DOWN_NODE_ID)
    { 
         Rowknife_S.AlarmCode=Dpack.RxData[2]; 
    }   
}





void appRowKnifeSpeedmodeCatn(char mDir,short spd)
{
    if(!mDir)
    {
        DELTA_Servo_SetTargetSpeed(0x01,spd);
    }
    else
    {
        DELTA_Servo_SetTargetSpeed(0x01,-spd);         
    }
}

void appRowKnifeDeviationCalibration_3(void)
{
    static char stoplock=0;
    short catnDeviation=0;
    short catnDetn_abs=0;    
    short catnStep=0;
    short cabtpos=0;
    char  cabtdir=0; 
    catnStep=BSP_ENCODER_getSteps(Rowknife_S.EndLast,BSP_getENCODER());    
    if(catnStep-Rowknife_S.EndActualPos>2)
    { 
        catnStep-=4;
    }
    Rowknife_S.MoveDistance=(float)(_EndeMinPrecision*catnStep);       
    if(Rowknife_S.Exetime>1000&& Rowknife_S.MoveFlag)
    {
        Rowknife_S.MoveFlag=0;
        printf("stop:%d \n",catnStep); 
        DELTA_Servo_SetTargetSpeed(0x01,0);
    }
    
    if(catnStep>=Rowknife_S.Actual_1&& Rowknife_S.MoveFlag&&!stoplock)
    {
        stoplock=1;
        printf("Act_1:%d \n",catnStep); 
        appRowKnifeSpeedmodeCatn(Rowknife_S.MoveDir,Rowknife_S.speed_1);
    }
    if(catnStep>=Rowknife_S.Actual_2&& Rowknife_S.MoveFlag&&(stoplock==1||!stoplock))
    {
        stoplock=2;
        printf("Act_2:%d \n",catnStep); 
        appRowKnifeSpeedmodeCatn(Rowknife_S.MoveDir,Rowknife_S.speed_2);    
    }    
    if(catnStep>=Rowknife_S.EndActualPos&&Rowknife_S.MoveFlag)
    {
        stoplock=0;
        printf("pp1_sp:%d \n",catnStep);    
        Rowknife_S.MoveFlag=0;
        appRowKnifeSpeedmodeCatn(Rowknife_S.MoveDir,0);
    }
}

MotorController motor_ctrl;

// 初始化电机控制器
void appRowKnifeMotorController_Init(MotorController *ctrl)
{
  // 高速PID参数(>70%速度)
    ctrl->high_speed_pid.Kp = 60.0f;
    ctrl->high_speed_pid.Ki = 5.0f;
    ctrl->high_speed_pid.Kd = 1.0f;
    ctrl->high_speed_pid.output_limit = MAX_MOTOR_SPEED;
    ctrl->high_speed_pid.integral_limit = MAX_MOTOR_SPEED * 0.7f;
    
    // 中速PID参数(30%-70%速度)
    ctrl->mid_speed_pid.Kp = 90.0f;
    ctrl->mid_speed_pid.Ki = 1.0f;
    ctrl->mid_speed_pid.Kd = 1.5f;
    ctrl->mid_speed_pid.output_limit = MAX_MOTOR_SPEED * 0.7f;
    ctrl->mid_speed_pid.integral_limit = MAX_MOTOR_SPEED * 0.35f;
    
    // 低速PID参数(<30%速度) - 小行程专用
    ctrl->low_speed_pid.Kp = 100.0f;
    ctrl->low_speed_pid.Ki = 2.0f;
    ctrl->low_speed_pid.Kd = 3.0f;
    ctrl->low_speed_pid.output_limit = MAX_MOTOR_SPEED * SMALL_MOVE_SPEED_RATIO;
    ctrl->low_speed_pid.integral_limit = MAX_MOTOR_SPEED * 0.15f;
    
    // 初始化状态
    ctrl->max_speed = MAX_MOTOR_SPEED;
    ctrl->enabled = 0;
    ctrl->fault = 0;
    ctrl->is_small_move = 0;
    ctrl->move_timeout = 100; // 默认100ms超时
}

// 停止电机
void MotorController_Stop(MotorController *ctrl) 
{
    ctrl->enabled = 0;
    DELTA_Servo_SetTargetSpeed(0x01,0);
}

// 应用速度限制
static void Apply_Limits(MotorController *ctrl, float *velocity)
{
    // 确保不超过最大速度
    if (fabsf(*velocity) > ctrl->max_speed) {
        *velocity = (*velocity > 0) ? ctrl->max_speed : -ctrl->max_speed;
    }
    
    // 小行程特殊限制
    if (ctrl->is_small_move) {
        float max_speed = ctrl->max_speed * SMALL_MOVE_SPEED_RATIO;
        if (fabsf(*velocity) > max_speed) {
            *velocity = (*velocity > 0) ? max_speed : -max_speed;
        }
    }
}

// 选择适当的PID参数组
static PID_Params* Select_PID_Params(MotorController *ctrl)
{
    if (ctrl->is_small_move) {
        return &ctrl->low_speed_pid;
    }
    
    // 计算当前速度比例
    float speed_ratio = fabsf(ctrl->current_velocity) / ctrl->max_speed;
    
    if (speed_ratio > 0.7f) {
        return &ctrl->high_speed_pid;
    } else if (speed_ratio > 0.3f) {
        return &ctrl->mid_speed_pid;
    } else {
        return &ctrl->low_speed_pid;
    }
}

// 设置电机速度(RPM)
void Set_Motor_Speed(uint16_t speed)
{
      Rowknife_S.MoveFlag=0;
    if(!Rowknife_S.MoveDir)
    {
        DELTA_Servo_SetTargetSpeed(0x01,speed); 
    }
    else
    {
        DELTA_Servo_SetTargetSpeed(0x01,-speed);        
    }
}

// 设置目标位置
void MotorController_SetTarget(MotorController *ctrl, int32_t position) 
{
//    if (ctrl->fault) return;
    
    // 重置PID状态
    memset(&ctrl->high_speed_pid.integral, 0, sizeof(float));
    memset(&ctrl->mid_speed_pid.integral, 0, sizeof(float));
    memset(&ctrl->low_speed_pid.integral, 0, sizeof(float));
    
    ctrl->EndLast =BSP_getENCODER();
    // 判断是否小行程
    float move_distance = fabsf((float)position - (float)BSP_ENCODER_getSteps(ctrl->EndLast,BSP_getENCODER()));
    ctrl->is_small_move = (move_distance <= SMALL_MOVE_THRESHOLD);
    
    ctrl->target_position = (float)position;
    ctrl->current_position = (float)BSP_ENCODER_getSteps(ctrl->EndLast,BSP_getENCODER()); 
    ctrl->current_velocity = 0;
    ctrl->start_time = HAL_GetTick();
    ctrl->enabled = 1;
}
// 主控制循环(在1ms定时器中断中调用)
void MotorController_Update(MotorController *ctrl) 
{
    static uint32_t last_time = 0;
    float position_error =0.0f;
    if(Rowknife_S.Exetime>200&& Rowknife_S.MoveFlag)
    {
        Rowknife_S.MoveFlag=0;
        printf("stop_mtr\n"); 
        Set_Motor_Speed(0);
    }
    // 安全检查和故障处理
    if (ctrl->fault ||!ctrl->enabled) {
         Set_Motor_Speed(0);
        return;
    }    
    // 获取时间增量(秒)
    uint32_t current_time = HAL_GetTick();
    float dt = (current_time - last_time) / 1000.0f;
    last_time = current_time;
    
    // 读取当前位置(编码器值)
    ctrl->current_position =(float)BSP_ENCODER_getSteps(ctrl->EndLast,BSP_getENCODER()); 
    Rowknife_S.MoveDistance=(float)(_EndeMinPrecision*ctrl->current_position);  
    // 检查是否到达目标位置
    position_error = ctrl->target_position - ctrl->current_position;
    if (fabsf(position_error) < (ctrl->is_small_move ? 0.5f : 1.0f))
    {
        printf("end_mtr\n"); 
        Rowknife_S.MoveDistance=(float)(_EndeMinPrecision*ctrl->current_position); 
        MotorController_Stop(ctrl);
        return;
    }
    
    // 检查运动超时
    if ((current_time - ctrl->start_time)>= ctrl->move_timeout) 
    {
//        ctrl->fault = 1;    
        Rowknife_S.MoveDistance=(float)(_EndeMinPrecision*ctrl->current_position); 
        printf("s:%d \n",(uint16_t)ctrl->current_velocity);
        MotorController_Stop(ctrl);
        return;
    }
    
    // 选择适当的PID参数组
    PID_Params *active_pid = Select_PID_Params(ctrl);
    
    // 计算PID输出
    float velocity_command = PID_Compute(active_pid, 
                                       ctrl->target_position, 
                                       ctrl->current_position, 
                                       dt);
    
    // 应用限制
    Apply_Limits(ctrl, &velocity_command);
    if((uint16_t)velocity_command==(uint16_t)ctrl->current_velocity)
        return;
    printf("sped:%d lst:%d\n",(uint16_t)ctrl->current_velocity,(uint16_t)fabsf(velocity_command));
    // 更新电机速度
    Set_Motor_Speed((uint16_t)fabsf(velocity_command));
    ctrl->current_velocity = velocity_command;
}
void appRowKnifeMovePos(char cmd ,int m_pos)
{
//  DELTA_Servo_enable(DOWN_NODE_ID,1);
//  Rowknife_S.EndLast=BSP_getENCODER();
  //PosRun(cmd!=0?0x20:0x21,m_pos,Rowknife_S.MoveSpeed,DOWN_NODE_ID);
  //DELTA_Servo_SetTargetSpeed(0x01,cmd==0?Rowknife_S.speed:-Rowknife_S.speed);    
  Rowknife_S.MoveDir=cmd;    
  Rowknife_S.MoveFlag=1;
  Rowknife_S.Exetime=0; 
  Rowknife_S.Cabttime=0;
  Rowknife_S.ReturnFlag=0;  
  Rowknife_S.CabtFlag=0;    
  MotorController_SetTarget(&motor_ctrl,Rowknife_S.EndActualPos);    
}

void appRowKnifeRealTimeing(void)
{
  Rowknife_S.time++; 
  Rowknife_S.Cabttime++;   
  if(motor_ctrl.enabled)
  {
     Rowknife_S.Exetime++;
     MotorController_Update(&motor_ctrl);
  }
  
}

/*
count:时间计数值
*/

void appRowKnifeSystemTask_loop(void)
{
    HqSlaveeturnLoopTask(); 
//    appRowKnifeDeviationCalibration_3();    
    if(Rowknife_S.time>1000)  
    {    
//       printf("D_%d\n",BSP_getENCODER());
//       printf("D_%f\n",(float)BSP_getENCODER()); 
       Rowknife_S.time=0;  
//       DELTA_Servo_GetSpeed(DOWN_NODE_ID);
       DELTA_Servo_GetPos(DOWN_NODE_ID);
    }

      
}


