#include "app_delta_servo.h"
#include "app_sdo.h"
#include "app_HqServoProtocol.h"

char appDeltServoType=1;

/*
���ٶ�
ACCnodeid:�豸id
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
���ü���ʱ��
ACCnodeid:�豸id
time_ms:����ʱ��
*/
void DELTA_Servo_SetAccTime(unsigned char ACCnodeid,unsigned int time_ms) 
{
	CanOpen_SdoWrite(ACCnodeid,SDOCS_WRITE4BYTE,OD_ACC_TIME,0,time_ms);
}

/*
���ü���ʱ��
DECnodeid:�豸id
time_ms:����ʱ��
*/
void DELTA_Servo_SetDecTime(unsigned char DECnodeid,unsigned int time_ms) 
{
	CanOpen_SdoWrite(DECnodeid,SDOCS_WRITE4BYTE,OD_DEC_TIME,0,time_ms) ;
}

/*
����Ŀ���ٶ�
Snodeid:�豸id
speed:�ٶ�
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
�趨����ָ��
  Wnodeid:�豸id
  ctrl_word:����ֵ
*/
void DELTA_Servo_SetCtrlWord(unsigned char Wnodeid, unsigned short ctrl_word) 
{
	CanOpen_SdoWrite(Wnodeid,SDOCS_WRITE2BYTE,OD_CONTROL_WORD,0,ctrl_word) ;
}

/*
����λ��ģʽ��Ŀ��λ��
  Pnodeid:�豸id
  pos:λ��ֵ
*/
void DELTA_Servo_SetTargetPosition(unsigned char Pnodeid,signed int pos) 
{
	CanOpen_SdoWrite(Pnodeid,SDOCS_WRITE4BYTE,OD_SET_TARGET_POSITION,0,pos);
}

/*
����λ��ģʽ����ٶ�����
  Sdnodeid:�豸id
  speed_limit:����ٶ�
*/
void DELTA_Servo_SetSpeedLimit(unsigned char Sdnodeid,unsigned int speed_limit) 
{
	CanOpen_SdoWrite(Sdnodeid,SDOCS_WRITE4BYTE,OD_SET_SPEED_LIMIT,0,speed_limit) ;
}

/*
����ģʽ
  node_id:�豸id
  mode:�豸����ģʽ
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
�ŷ����ʹ�ܲ���
Enode_id:�豸id
on_off:ʹ��/ʧ��
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
            DELTA_Servo_SetCtrlWord(Enode_id,6); //�ر�
            DELTA_Servo_SetCtrlWord(Enode_id,7); //servo-׼��
            DELTA_Servo_SetCtrlWord(Enode_id,15);//servo -on
        }
        else
        {
            DELTA_Servo_SetCtrlWord(Enode_id,6) ; //�ر� 
        }
   }
}
/*
λ��ģʽ����
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
    /*��Ӳ���ӿڷ���*/
   appHqservo.Can1Send(senddata,3,CANSendID);     

    
}
//servo ��ʼ��
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















