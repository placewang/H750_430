#include "main.h"
#include "bsp_dcan.h"
#include "app_HqServoProtocol.h"


void CanSend(uint8_t *DataBuf, uint8_t Len, const unsigned int ID)
{
    HAL_Delay(4); 
    bsp_Can1_SendPacket(DataBuf,Len,ID);
}

/*
������
tye:   ��������
parnum:Ŀ��������Cn
ID��   �����
*/
int RunParameter_r(uint8_t tye,uint8_t parnum,uint16_t mID)
{
    unsigned char senddata[8]={0};   
    senddata[0]=tye;
    senddata[1]=mID;
    senddata[2]=parnum;  
      
    /*��Ӳ���ӿڷ���*/
    CanSend(senddata,8,CANSendID);   
    return 0;
}
/*
д��������
parnum:Ŀ��������
data:  д�����
ID��   �����
*/
void WriteWorkParameter(uint16_t parnum,uint32_t data,uint16_t mID)
{
    unsigned char senddata[8]={0};   
    senddata[0]=Cmd_WriteworkPmr;
    senddata[1]=mID;
    senddata[2]=parnum&0xff; 
    senddata[3]=(parnum>>8)&0xff;
    /**/
    senddata[4] =  data&0xff;
    senddata[5] = (data>>8)&0xff;
    senddata[6] = (data>>16)&0xff;
    senddata[7] = (data>>24)&0xff;     
      
    /*��Ӳ���ӿڷ���*/
    CanSend(senddata,8,CANSendID);   
}
/*
дϵͳ����
parnum:Ŀ��������
data:  д�����
ID��   �����
*/
void WriteSystemParameter(uint16_t parnum,uint32_t data,uint16_t mID)
{
    unsigned char senddata[8]={0};   
    senddata[0]=Cmd_WriteSysPmr;
    senddata[1]=mID;
    senddata[2]=parnum&0xff; 
    senddata[3]=(parnum>>8)&0xff;
    /**/
    senddata[4] =  data&0xff;
    senddata[5] = (data>>8)&0xff;
    senddata[6] = (data>>16)&0xff;
    senddata[7] = (data>>24)&0xff;     
      
    /*��Ӳ���ӿڷ���*/
    CanSend(senddata,8,CANSendID);   
}
/*
�ٶ�ģʽ
*/
void SpeedRunCmd(int16_t speed,uint16_t sid)
{
	unsigned char writedata[8]={0};
    writedata[0]=Cmd_RunMode;
    writedata[1]=sid;
    writedata[2]=SpeedMode;
    writedata[3]=speed&0xff;
    writedata[4]=(speed>>8)&0xff;
       
    /*��Ӳ���ӿڷ���*/
    CanSend(writedata,8,CANSendID);      
}
/*
���ʹ��/ʧ��
*/
void EnableOrDisable(uint8_t ble,uint16_t eid)
{
    unsigned char senddata[8]={0};
    senddata[0]=Cmd_Enable;
    senddata[1]=eid;
    senddata[2]=Enable;
    senddata[3]=ble;
     
    /*��Ӳ���ӿڷ���*/
    CanSend(senddata,8,CANSendID);        
}

/*
��������
*/
void upgradeCmd(uint8_t cmd,uint8_t mid, uint8_t * data, uint8_t idx)
{
    unsigned char senddata[8]={0};
    senddata[0]=cmd;
    senddata[1]=mid;
    senddata[2]=data[0];
    senddata[3]=data[1];
    senddata[4]=data[2];
    senddata[5]=data[3];  
    senddata[6]=idx;
    senddata[7]=0;
    /*��Ӳ���ӿڷ���*/
    CanSend(senddata,8,CANSendID);    
}

HqServo appHqservo={
    .Can1Send=CanSend,
    .SetWorkParameter=WriteWorkParameter,
    .SetSysParameter=WriteSystemParameter,
    .EnableDisa  =EnableOrDisable,
    .SpeedModeRun=SpeedRunCmd,
    .ReadRunParameter=RunParameter_r,
    .UpgradeCmdSend=upgradeCmd
};























