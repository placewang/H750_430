#include "bsp_dcan.h"
#include "app_sdo.h"
#include "app_pid.h"
#include "app_queue.h"
#include "app_delta_servo.h"
#include "app_DischargeSystem.h"
#include "app_balanceRoll_code.h"
#include "app_delta_servo.h"
#include "app_HqServoProtocol.h"

#define DECEOffSET 10
#define DECELERATIONLIMIT  40

//放料运行参数
RunParameter Discharge={
                        .UpTensionPos=230,
                        .DownTensionPos=230,
                        .UpJogSpeed=100,
                        .DownJogSpeed=100,
                        .UpLowerlimi=10,
                        .DownLowerlimi=10,
                       };        

/*
SDO从设备返回接收处理
*/
void CanOpen_SlaveReturnLoopTask(void)
{
//    QUEUE_DATA_T Dpack={0}; 
    CAN_RevTye Dpack={0}; 
    signed short upspeed=0;
    signed short downspeed=0;
    unsigned short CanOpenID=0;
    unsigned short index=0;
    unsigned char  CS=0;
    unsigned char  SubIndex=0;
 
//    if(!DeQueue(&Can1_RevQueue,&Dpack))
//       return;
    if(!CAN1_RevQueue_dequeue(&Can1RevQueue,&Dpack))
       return;  
    CS=Dpack.RxData[0];
    CanOpenID=Dpack.StdId;
    SubIndex=Dpack.RxData[3];
    index=(Dpack.RxData[2]<<8)|Dpack.RxData[1];
    //上电机紧急事件（err）
    if((UP_NODE_ID+Obj_EMCY)==CanOpenID)
    {
       Discharge.AlarmCode_U =*((uint16_t*)(&Dpack.RxData[0]))+1;
    }//上电机紧急事件（err）
    else if((DOWN_NODE_ID+Obj_EMCY)==CanOpenID)
    {
       Discharge.AlarmCode_D =*((uint16_t*)(&Dpack.RxData[0]))+1;
    }
    //上放料实时速度放回
    if((UP_NODE_ID+ObjectID_SDO_Receive)==CanOpenID&&\
        CS==SDOCS_READ1BYTE_ACK&&index==OD_READ_CUR_SPEED&& \
        SubIndex==0)
    {
       Discharge.RevCnt_U=0; 
       upspeed=*((int32_t *)&Dpack.RxData[4]);
       Discharge.UpSpeed=(upspeed>0?upspeed:-upspeed)/10;
    }//下放料实时速度放回
    else if((DOWN_NODE_ID+ObjectID_SDO_Receive)==CanOpenID&&\
            CS==SDOCS_READ1BYTE_ACK&&index==OD_READ_CUR_SPEED&& \
            SubIndex==0)
    {
        Discharge.RevCnt_D=0;
        downspeed =*((int32_t *)&Dpack.RxData[4]);
        Discharge.DownSpeed=(downspeed>0?downspeed:-downspeed)/10;
    } //上放料节点上线通知
    else if((UP_NODE_ID+ObjectID_NodeOnline)==CanOpenID)
    {
        CanOpen_BusNodeStart(0,0);
        DELTA_Servo_SetMode(UP_NODE_ID,3);  
        DELTA_Servo_enable(UP_NODE_ID,1);        
    }//下放料节点上线通知
    else if((DOWN_NODE_ID+ObjectID_NodeOnline)==CanOpenID)
    {
        CanOpen_BusNodeStart(0,0);
        DELTA_Servo_SetMode(DOWN_NODE_ID,3);
        DELTA_Servo_enable(DOWN_NODE_ID,1);        
    }    
}
extern void APP_UpgradeBackAnalyze(unsigned char *canData);
//CAN发送ID
#define  CAN_RevID    0x752    
uint32_t IntervalTime=0;  
void HqSlaveReturnLoopTask(void)
{
//    QUEUE_DATA_T Dpack={0}; 
      CAN_RevTye Dpack={0}; 
    signed short upspeed=0;
    signed short downspeed=0;
    
//    if(!DeQueue(&Can1_RevQueue,&Dpack))
//       return;
     if(!CAN1_RevQueue_dequeue(&Can1RevQueue,&Dpack))
       return;      
    if(Dpack.StdId!=CAN_RevID)
       return;
//    APP_UpgradeBackAnalyze(Dpack.RxData);
    //上放料实时速度放回
    if(Dpack.RxData[0]==Cmd_TypeReadRunPmr&&Dpack.RxData[1]==UP_NODE_ID&&Dpack.RxData[2]==Rspeed)
    {
       Discharge.RevCnt_U=0; 
       upspeed=*((int16_t *)&Dpack.RxData[3]);
       Discharge.UpSpeed=(upspeed>0?upspeed:-upspeed)/10;
    }//下放料实时速度放回
    else if(Dpack.RxData[0]==Cmd_TypeReadRunPmr&&Dpack.RxData[1]==DOWN_NODE_ID&&Dpack.RxData[2]==Rspeed)
    {
       Discharge.RevCnt_D=0;  
       downspeed =*((int16_t *)&Dpack.RxData[3]);
       Discharge.DownSpeed=(downspeed>0?downspeed:-downspeed)/10;
    } 
    //上放料实时速度放回
    else if(Dpack.RxData[0]==Cmd_RunMode&&Dpack.RxData[1]==UP_NODE_ID&&Dpack.RxData[2]==SpeedMode)
    {
       upspeed=*((int16_t *)&Dpack.RxData[3]);
       Discharge.UpSpeed=(upspeed>0?upspeed:-upspeed)/10;
        
    }//下放料实时速度放回
    else if(Dpack.RxData[0]==Cmd_RunMode&&Dpack.RxData[1]==DOWN_NODE_ID&&Dpack.RxData[2]==SpeedMode)
    { 
       downspeed =*((int16_t *)&Dpack.RxData[3]);
       Discharge.DownSpeed=(downspeed>0?downspeed:-downspeed)/10;
    }     
    else if(Dpack.RxData[0]==Cmd_Alarm&&Dpack.RxData[1]==UP_NODE_ID)
    {
        Discharge.AlarmCode_U=Dpack.RxData[2];
    }
    else if(Dpack.RxData[0]==Cmd_Alarm&&Dpack.RxData[1]==DOWN_NODE_ID)
    { 
       Discharge.AlarmCode_D=Dpack.RxData[2]; 
    }   
}
/*
PID变化速度率计算调节(动态调节)  
*/
void PID_PercentageSpeedVariationCalculate(signed short*Encode,_pid *pid)
{
    float Acce_kp=48.0,Acce=28.0,Dece_kp=2;
    float Dw_p=0.0,Dw_i=0.01,Dw_d=0.08;
    if(*Encode>pid->target_val)
    {
        if(*Encode>500)
        {
            Dw_p=(*Encode-pid->target_val)/Acce;             
        }
        else
        {
            Dw_p=(*Encode-pid->target_val)/Acce_kp;
        }            
    }
    else if(*Encode<pid->target_val)
    {
        Dw_p=(pid->target_val-*Encode)/Dece_kp;        
    }    
    else if(*Encode==pid->target_val){
        return;
    }
    set_p_i_d(pid,Dw_p,Dw_i,Dw_d);
}

/*
放料速度转换处理
m_num:电机 1-下放料 2-上放料
*PidSpeed:PID计算出目标速度
*pos:  当前编码值
return :1-转换速度与上一次相异 0-与上一次速度相同
*/
signed char PIDSpeedConversion(char m_num,signed int * const PidSpeed,int16_t *pos)
{
    _pid *PID;
    int MaxSpeed=30000;
    signed int pidval=0;  
    unsigned char Dir=0;    
    signed int *lastSpeed=0;

    if(m_num==1)
    {
        PID=&DownMove_pid;
        Dir=Discharge.DownDir;
        lastSpeed=&Discharge.Down_LastPidspeed;
    }
    else if(m_num==2)
    {
        PID=&UpMove_pid; 
        Dir=Discharge.UpDir;
        lastSpeed=&Discharge.Up_LastPidspeed;
    }
    //计算目标值
    pidval=PID_realize(PID,*pos); 
    //取绝对值
    *PidSpeed=pidval>0?pidval:-pidval; 
    //最大速度限制 
    if(*PidSpeed>MaxSpeed)                    
    {
        *PidSpeed=MaxSpeed;
    }             
    //判断放料方向    
    *PidSpeed=Dir==0?*PidSpeed:-(*PidSpeed);  
    //速度相同判断
    if(*lastSpeed==*PidSpeed)
    {
        return 0;
    }
    //速度不同并记录    
    *lastSpeed=*PidSpeed;
    return 1;
}


/*
count:时间计数值
*/
void MotorDroppedLineDetection_loop(int *time)
{
    if (*time>EncodeRefreshTime*3)
    {
        *time=0;
        if(Discharge.devMode!=_SingAxleMod)   
        {
            Discharge.RevCnt_U==0?Discharge.RevCnt_U++ \
                                 :Discharge.RevCnt_U++;
            DELTA_Servo_GetSpeed(UP_NODE_ID);
        }
        Discharge.RevCnt_D==0?Discharge.RevCnt_D++ \
                             :Discharge.RevCnt_D++;
        DELTA_Servo_GetSpeed(DOWN_NODE_ID);
    }        
}
/*
判断编码是否上升
mrtid: 1-下放料 2-上放料
*/
int JudgmentEncodingValue(short* last,int16_t *Encde)
{
    if(*Encde>*last)
    {
        *last=*Encde;
        return 1;
    }
    return 0;
}

/*
双轴放料（每个放料轴与张力反馈一对一调节）
Upos:上放料编码反馈
Dpos:下放料编码反馈
*/
signed int DualDown_speed=0;  
signed int Dualup_speed=0;
signed int testDown_speed=0;  
signed int testup_speed=0;
void AxisIndependentDischarge(RunParameter* discharg,Encode*code)
{
    unsigned short startSpeed=20;
    signed short   MinSpeed=50;    
    static char    Upstopflag=0;
    static char    Dwonstopflag=0;
    signed short   U_Lowerlimi=0;
    signed short   D_Lowerlimi=0; 
    unsigned short UpTensionPos=0;
    unsigned short DownTensionPos=0;    
    uint8_t Pi=3,ki=2;
    
    if(discharg->devMode!=_SinglAxisDoubleVolumeMode)
    {
        UpTensionPos   =discharg->UpTensionPos;
        DownTensionPos =discharg->DownTensionPos;
        U_Lowerlimi    =discharg->UpLowerlimi;
        D_Lowerlimi    =discharg->DownLowerlimi;
    }
    else
    {
        UpTensionPos   =discharg->DownTensionPos;
        DownTensionPos =discharg->DownTensionPos; 
        U_Lowerlimi    =discharg->DownLowerlimi;
        D_Lowerlimi    =discharg->DownLowerlimi;        
    }  
    set_pid_target(&UpMove_pid,(float)UpTensionPos); 
    set_pid_target(&DownMove_pid,(float)DownTensionPos);     
    
    //dwon 低于张力下限停机
    if(code->DownReal_TimePos<=D_Lowerlimi&&Dwonstopflag!=0)
    {
        Dwonstopflag=0; 
        DualDown_speed=0;
        PID_param_init(2);   
        code->LastPos_D=0;   
        DELTA_Servo_SetTargetSpeed(DOWN_NODE_ID,0);       
    }
    //up 低于张力下限停机
    if(code->UpReal_TimePos<=U_Lowerlimi&&Upstopflag!=0)
    {
        Upstopflag=0;   
        Dualup_speed=0;
        PID_param_init(1);
        code->LastPos_U=0;    
        DELTA_Servo_SetTargetSpeed(UP_NODE_ID,0);  
    }   
    
    if(Discharge.UpMoveEnable&&(Discharge.RunFlag&_Startflag_U))
    {
        if(code->UpReal_TimePos>(U_Lowerlimi+DECEOffSET))
        {
          PID_PercentageSpeedVariationCalculate(&code->UpReal_TimePos,&UpMove_pid); 
          if(Upstopflag==0) 
          {
             Upstopflag=1;
             Dualup_speed=startSpeed; 
             PID_param_init(1); 
          }
        }        
        switch (Upstopflag)
        {
           case 1: 
            if(JudgmentEncodingValue(&code->LastPos_U,&code->UpReal_TimePos))
            {                
                testup_speed=(discharg->UpDir==0?++Dualup_speed:--Dualup_speed)/ki;
                DELTA_Servo_SetTargetSpeed(UP_NODE_ID,Dualup_speed/ki);                           
            }
            if(code->UpReal_TimePos>=UpTensionPos)
            {
                Upstopflag=2;
                PID_set_err(&UpMove_pid,(float)Dualup_speed,(float)(Dualup_speed/ki));  
            }                
            break;
           case 2:
            if(PIDSpeedConversion(2,&Dualup_speed,&code->UpReal_TimePos)){             
                DELTA_Servo_SetTargetSpeed(UP_NODE_ID,Dualup_speed);         
            }//减速      
            if(code->UpReal_TimePos<(UpTensionPos/Pi)){
                Upstopflag=3;
                DELTA_Servo_SetTargetSpeed(UP_NODE_ID,discharg->UpDir==0?MinSpeed:-MinSpeed); 
            }  
            break;
           case 3:
              if(code->UpReal_TimePos>(UpTensionPos/Pi)){
                Dualup_speed=MinSpeed;  
                Upstopflag=1; 
                PID_param_init(1);
              }   
              break;             
        }
    }
    else
    {
        Upstopflag=0;   
        Dualup_speed=0;
        PID_param_init(1);                 
    }
    /***********************************down**************************************/
    if(Discharge.DownMoveEnable&&(Discharge.RunFlag&_Startflag_D))
    {  
       if(code->DownReal_TimePos>(Discharge.DownLowerlimi+DECEOffSET))
       { 
         PID_PercentageSpeedVariationCalculate(&code->DownReal_TimePos,&DownMove_pid);  
         if(Dwonstopflag==0)
         {
             Dwonstopflag=1;
             DualDown_speed=startSpeed;
             PID_param_init(2);           
         }
       }        
       switch (Dwonstopflag)
       {
          case 1:   
              if(JudgmentEncodingValue(&code->LastPos_D,&code->DownReal_TimePos))
              {    
             
                testDown_speed=(discharg->DownDir==0?++DualDown_speed:--DualDown_speed)/ki;
                DELTA_Servo_SetTargetSpeed(DOWN_NODE_ID,DualDown_speed/ki);   
              }                
              if(code->DownReal_TimePos>=DownTensionPos)
              {
                Dwonstopflag=2;  
                PID_set_err(&DownMove_pid,(float)DualDown_speed,(float)DualDown_speed/ki);  
              }               
              break;
          case 2:
              if(PIDSpeedConversion(1,&DualDown_speed,&code->DownReal_TimePos)){ 
                 DELTA_Servo_SetTargetSpeed(DOWN_NODE_ID,DualDown_speed); 
              }//减速      
              if(code->DownReal_TimePos<(DownTensionPos/Pi))
              {
                Dwonstopflag=3; 
                DELTA_Servo_SetTargetSpeed(DOWN_NODE_ID,discharg->DownDir==0?MinSpeed:-MinSpeed);      
              }  
              break;   
          case 3:
              if(code->DownReal_TimePos>(DownTensionPos/Pi))
              {
                DualDown_speed=MinSpeed;  
                Dwonstopflag=1; 
                PID_param_init(2);
              }
              break;  
       }
    }
    else
    {
        Dwonstopflag=0; 
        DualDown_speed=0;
        PID_param_init(2);           
    }
}
void app_DischargeDataParsingTask(char type)
{
    if(type)
    {
        HqSlaveReturnLoopTask();
    }
    else
    {
        CanOpen_SlaveReturnLoopTask();      
    }
}

/*
放料系统运行
*/
int SpeedRequest=0;
void DischargeRunloopTask(void)
{   
    
    app_DischargeDataParsingTask(appDeltServoType);
    BalanceRollEncodeValLoadloopTask(Discharge.devMode);
    if(Discharge.PIDRefreshTime>=EncodeRefreshTime)
    {
        SpeedRequest++; 
        Discharge.PIDRefreshTime=0;
        app_BalanceRoll_ReadEncodeVal(Discharge.devMode);
        MotorDroppedLineDetection_loop(&SpeedRequest);  
        AxisIndependentDischarge(&Discharge,&Encode_handle);        
    }
    
}




