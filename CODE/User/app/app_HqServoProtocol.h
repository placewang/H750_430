#ifndef _HQ_SERVO_H_ 
#define _HQ_SERVO_H_ 
#define CANSendID          0x356     //CAN����ID
#define Cmd_TypeReadRunPmr 0xab
#define Cmd_WriteworkPmr   0xa9
#define Cmd_WriteSysPmr    0xa5
#define Cmd_RunMode        0xa0
#define Cmd_Enable         0xa0 
#define Cmd_Alarm          0xb3 
#define Cmd_upgStart       0x74
#define Cmd_upgNulock      0x75
#define Cmd_upgErase       0x76
#define Cmd_upgRqstaddr    0x77
#define Cmd_upgShortData   0x78
#define Cmd_upgLongData    0x79    
#define Cmd_upgEnd         0x7A  

//����(0xa0)������
enum
{
    Enable   =0x00,
    SpeedMode=0x01, 
};
//��������(0xa9)������
enum
{
    wkDecRatio_M=2,        //���ٱȷ���
    wkDenominator=3,       //���ٱȷ�ĸ  
    wkAcceleration=0x0b,   //�ٶ�ģʽ���ٶ�  
    wkResponsiveness=0x0c, //�ٶ�ģʽ������
};
//��������(0xa5)������
enum
{
    SysIdset=0, 
        
};
//���в���(0xab)������
enum
{
   Rspeed= 0x01,
};
//���в���(0xb3)������
enum
{
   alarmClear = 0x00,
   alarmEnable= 0x01, 
   alarmDisab = 0x02 
};


typedef struct
{
  void (*Can1Send)(unsigned char *, unsigned char, const unsigned int ID);
  void (*SetWorkParameter)(unsigned short parnum,unsigned int data,unsigned short eid);
  void (*SetSysParameter)(unsigned short parnum,unsigned int data,unsigned short eid);     
  void (*EnableDisa)(unsigned char ble,unsigned short eid);
  void (*SpeedModeRun)(short speed,unsigned short sid);    
  int  (*ReadRunParameter)(unsigned char tye,unsigned char parnum,unsigned short mID); 
  void (*UpgradeCmdSend)(unsigned char,unsigned char, unsigned char *,unsigned char);  
}HqServo;

extern HqServo appHqservo;




#endif








