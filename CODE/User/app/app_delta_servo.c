#include "app_delta_servo.h"
#include "app_sdo.h"
#include "app_HqServoProtocol.h"

char appDeltServoType=1;

/*
读速度
ACCnodeid:设备id
*/
void DELTA_Servo_GetSpeed(unsigned char ACCnodeid) 
{
   if(appDeltServoType)
   {
        appHqservo.ReadRunParameter(Cmd_TypeReadRunPmr,Rspeed,ACCnodeid);    
   }
   else
   {       
        CanOpen_SdoRead(ACCnodeid,OD_READ_CUR_SPEED,0);
   }
}


/*
设置加速时间
ACCnodeid:设备id
time_ms:加速时间
*/
void DELTA_Servo_SetAccTime(unsigned char ACCnodeid,unsigned int time_ms) 
{
	CanOpen_SdoWrite(ACCnodeid,SDOCS_WRITE4BYTE,OD_ACC_TIME,0,time_ms);
}

/*
设置减速时间
DECnodeid:设备id
time_ms:加速时间
*/
void DELTA_Servo_SetDecTime(unsigned char DECnodeid,unsigned int time_ms) 
{
	CanOpen_SdoWrite(DECnodeid,SDOCS_WRITE4BYTE,OD_DEC_TIME,0,time_ms) ;
}

/*
设置目标速度
Snodeid:设备id
speed:速度
*/
void DELTA_Servo_SetTargetSpeed(unsigned char Snodeid,signed int speed) 
{
   if(appDeltServoType)
   {
       appHqservo.SpeedModeRun(speed,Snodeid);
   }
   else
   {    
       CanOpen_SdoWrite(Snodeid,SDOCS_WRITE4BYTE,OD_SET_TARGET_SPEED,0,speed) ;
   }
}
/*
设定控制指令
  Wnodeid:设备id
  ctrl_word:配置值
*/
void DELTA_Servo_SetCtrlWord(unsigned char Wnodeid, unsigned short ctrl_word) 
{
	CanOpen_SdoWrite(Wnodeid,SDOCS_WRITE2BYTE,OD_CONTROL_WORD,0,ctrl_word) ;
}

/*
设置位置模式的目标位置
  Pnodeid:设备id
  pos:位置值
*/
void DELTA_Servo_SetTargetPosition(unsigned char Pnodeid,signed int pos) 
{
	CanOpen_SdoWrite(Pnodeid,SDOCS_WRITE4BYTE,OD_SET_TARGET_POSITION,0,pos);
}

/*
设置位置模式最大速度限制
  Sdnodeid:设备id
  speed_limit:最大速度
*/
void DELTA_Servo_SetSpeedLimit(unsigned char Sdnodeid,unsigned int speed_limit) 
{
	CanOpen_SdoWrite(Sdnodeid,SDOCS_WRITE4BYTE,OD_SET_SPEED_LIMIT,0,speed_limit) ;
}

/*
设置模式
  node_id:设备id
  mode:设备运行模式
*/
void DELTA_Servo_SetMode(unsigned char node_id,unsigned char mode) 
{
	CanOpen_SdoWrite(node_id,SDOCS_WRITE1BYTE,OD_SET_MODE,0,mode);
	if(mode ==SERVO_SPEED_MODE)
	{		  
	   DELTA_Servo_SetAccTime(node_id,1000);
	   DELTA_Servo_SetDecTime(node_id,1000);
	   DELTA_Servo_SetTargetSpeed(node_id,0);
	}
	else if(mode ==SERVO_POSITION_MODE)
	{
	   DELTA_Servo_SetAccTime(node_id,1000);
	   DELTA_Servo_SetDecTime(node_id,1000);
	}
}
/*
伺服电机使能操作
Enode_id:设备id
on_off:使能/失能
*/
void DELTA_Servo_enable(unsigned char Enode_id,unsigned char on_off)
{
   if(appDeltServoType)
   {
        appHqservo.EnableDisa(on_off==0?0:1,Enode_id);    
   }
   else
   {
        if(on_off)
        {
            DELTA_Servo_SetCtrlWord(Enode_id,6); //关闭
            DELTA_Servo_SetCtrlWord(Enode_id,7); //servo-准备
            DELTA_Servo_SetCtrlWord(Enode_id,15);//servo -on
        }
        else
        {
            DELTA_Servo_SetCtrlWord(Enode_id,6) ; //关闭 
        }
   }
}
/*
位置模式运行
unsigned char Mnode_id

*/
void DELTA_Servo_PositionRun(unsigned char Mnode_id,signed int pos ,unsigned int speed) 
{
	 DELTA_Servo_SetCtrlWord(Mnode_id,15);//servo -on
	 DELTA_Servo_SetTargetPosition(Mnode_id,pos) ;
	 DELTA_Servo_SetSpeedLimit(Mnode_id,speed) ;
	 DELTA_Servo_SetCtrlWord(Mnode_id,31);
}

void DELTA_Set_motorID(unsigned char mID)
{
    
    appHqservo.SetSysParameter(SysIdset,mID,0);

}

void DELTA_AlarmOperation(unsigned char prm,unsigned char aid)
{
     unsigned char senddata[4]={0};
    senddata[0]=Cmd_Alarm ;
    senddata[1]=aid;
    senddata[2]=prm;      
    /*调硬件接口发送*/
   appHqservo.Can1Send(senddata,3,CANSendID);     

    
}
//servo 初始化
void DELTA_Servo_init(void) 
{
   if(appDeltServoType)
   {
        DELTA_AlarmOperation(alarmEnable,UP_NODE_ID);
        DELTA_AlarmOperation(alarmEnable,DOWN_NODE_ID);
        appHqservo.SetWorkParameter(wkDecRatio_M,0x0a,UP_NODE_ID);   
        appHqservo.SetWorkParameter(wkDecRatio_M,0x0a,DOWN_NODE_ID); 
        appHqservo.SetWorkParameter(wkDenominator,0x01,UP_NODE_ID);   
        appHqservo.SetWorkParameter(wkDenominator,0x01,DOWN_NODE_ID); 
        appHqservo.SetWorkParameter(wkAcceleration,2000,UP_NODE_ID);   
        appHqservo.SetWorkParameter(wkAcceleration,2000,DOWN_NODE_ID);     
        appHqservo.SetWorkParameter(wkResponsiveness,5000,UP_NODE_ID);   
        appHqservo.SetWorkParameter(wkResponsiveness,5000,DOWN_NODE_ID);      
        DELTA_Servo_enable(UP_NODE_ID,1);
        DELTA_Servo_enable(DOWN_NODE_ID,1);       
   }
   else
   {    
        CanOpen_BusNodeStart(0,0);
        DELTA_Servo_SetMode(UP_NODE_ID,SERVO_SPEED_MODE);
        DELTA_Servo_SetMode(DOWN_NODE_ID,SERVO_SPEED_MODE);
   }
}















