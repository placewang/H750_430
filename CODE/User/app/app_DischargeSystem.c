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

//�������в���
RunParameter Discharge={
                        .UpTensionPos=230,
                        .DownTensionPos=230,
                        .UpJogSpeed=100,
                        .DownJogSpeed=100,
                        .UpLowerlimi=10,
                        .DownLowerlimi=10,
                       };        

/*
SDO���豸���ؽ��մ���
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
    //�ϵ�������¼���err��
    if((UP_NODE_ID+Obj_EMCY)==CanOpenID)
    {
       Discharge.AlarmCode_U =*((uint16_t*)(&Dpack.RxData[0]))+1;
    }//�ϵ�������¼���err��
    else if((DOWN_NODE_ID+Obj_EMCY)==CanOpenID)
    {
       Discharge.AlarmCode_D =*((uint16_t*)(&Dpack.RxData[0]))+1;
    }
    //�Ϸ���ʵʱ�ٶȷŻ�
    if((UP_NODE_ID+ObjectID_SDO_Receive)==CanOpenID&&\
        CS==SDOCS_READ1BYTE_ACK&&index==OD_READ_CUR_SPEED&& \
        SubIndex==0)
    {
       Discharge.RevCnt_U=0; 
       upspeed=*((int32_t *)&Dpack.RxData[4]);
       Discharge.UpSpeed=(upspeed>0?upspeed:-upspeed)/10;
    }//�·���ʵʱ�ٶȷŻ�
    else if((DOWN_NODE_ID+ObjectID_SDO_Receive)==CanOpenID&&\
            CS==SDOCS_READ1BYTE_ACK&&index==OD_READ_CUR_SPEED&& \
            SubIndex==0)
    {
        Discharge.RevCnt_D=0;
        downspeed =*((int32_t *)&Dpack.RxData[4]);
        Discharge.DownSpeed=(downspeed>0?downspeed:-downspeed)/10;
    } //�Ϸ��Ͻڵ�����֪ͨ
    else if((UP_NODE_ID+ObjectID_NodeOnline)==CanOpenID)
    {
        CanOpen_BusNodeStart(0,0);
        DELTA_Servo_SetMode(UP_NODE_ID,3);  
        DELTA_Servo_enable(UP_NODE_ID,1);        
    }//�·��Ͻڵ�����֪ͨ
    else if((DOWN_NODE_ID+ObjectID_NodeOnline)==CanOpenID)
    {
        CanOpen_BusNodeStart(0,0);
        DELTA_Servo_SetMode(DOWN_NODE_ID,3);
        DELTA_Servo_enable(DOWN_NODE_ID,1);        
    }    
}
extern void APP_UpgradeBackAnalyze(unsigned char *canData);
//CAN����ID
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
    //�Ϸ���ʵʱ�ٶȷŻ�
    if(Dpack.RxData[0]==Cmd_TypeReadRunPmr&&Dpack.RxData[1]==UP_NODE_ID&&Dpack.RxData[2]==Rspeed)
    {
       Discharge.RevCnt_U=0; 
       upspeed=*((int16_t *)&Dpack.RxData[3]);
       Discharge.UpSpeed=(upspeed>0?upspeed:-upspeed)/10;
    }//�·���ʵʱ�ٶȷŻ�
    else if(Dpack.RxData[0]==Cmd_TypeReadRunPmr&&Dpack.RxData[1]==DOWN_NODE_ID&&Dpack.RxData[2]==Rspeed)
    {
       Discharge.RevCnt_D=0;  
       downspeed =*((int16_t *)&Dpack.RxData[3]);
       Discharge.DownSpeed=(downspeed>0?downspeed:-downspeed)/10;
    } 
    //�Ϸ���ʵʱ�ٶȷŻ�
    else if(Dpack.RxData[0]==Cmd_RunMode&&Dpack.RxData[1]==UP_NODE_ID&&Dpack.RxData[2]==SpeedMode)
    {
       upspeed=*((int16_t *)&Dpack.RxData[3]);
       Discharge.UpSpeed=(upspeed>0?upspeed:-upspeed)/10;
        
    }//�·���ʵʱ�ٶȷŻ�
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
PID�仯�ٶ��ʼ������(��̬����)  
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
�����ٶ�ת������
m_num:��� 1-�·��� 2-�Ϸ���
*PidSpeed:PID�����Ŀ���ٶ�
*pos:  ��ǰ����ֵ
return :1-ת���ٶ�����һ������ 0-����һ���ٶ���ͬ
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
    //����Ŀ��ֵ
    pidval=PID_realize(PID,*pos); 
    //ȡ����ֵ
    *PidSpeed=pidval>0?pidval:-pidval; 
    //����ٶ����� 
    if(*PidSpeed>MaxSpeed)                    
    {
        *PidSpeed=MaxSpeed;
    }             
    //�жϷ��Ϸ���    
    *PidSpeed=Dir==0?*PidSpeed:-(*PidSpeed);  
    //�ٶ���ͬ�ж�
    if(*lastSpeed==*PidSpeed)
    {
        return 0;
    }
    //�ٶȲ�ͬ����¼    
    *lastSpeed=*PidSpeed;
    return 1;
}


/*
count:ʱ�����ֵ
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
�жϱ����Ƿ�����
mrtid: 1-�·��� 2-�Ϸ���
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
˫����ϣ�ÿ������������������һ��һ���ڣ�
Upos:�Ϸ��ϱ��뷴��
Dpos:�·��ϱ��뷴��
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
    
    //dwon ������������ͣ��
    if(code->DownReal_TimePos<=D_Lowerlimi&&Dwonstopflag!=0)
    {
        Dwonstopflag=0; 
        DualDown_speed=0;
        PID_param_init(2);   
        code->LastPos_D=0;   
        DELTA_Servo_SetTargetSpeed(DOWN_NODE_ID,0);       
    }
    //up ������������ͣ��
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
            }//����      
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
              }//����      
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
����ϵͳ����
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




