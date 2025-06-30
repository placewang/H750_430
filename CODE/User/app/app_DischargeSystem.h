
#ifndef _DISCHARGESYSTEM_ 
#define _DISCHARGESYSTEM_ 

#define  _DoubleAxisMod             0
#define  _SingAxleMod               1
#define  _SinglAxisDoubleVolumeMode 2

#define  _Startflag_U               1
#define  _Startflag_D               2

typedef struct
{
    unsigned char  devMode;        //�豸ģʽ   
    unsigned char  mode;           //����ģʽ    0:˫����� 1�������л�
    unsigned char  RunFlag;        //��ͣ��־    0:ֹͣ     1:����
    unsigned char  UpMoveEnable;   //�Ϸ����˶�  0:ʧ��     1:ʹ��
    unsigned char  DownMoveEnable; //�·����˶�  0:ʧ��     1:ʹ��
    unsigned char  UpDir;          //�Ϸ��Ϸ���
    unsigned char  DownDir;        //�·��Ϸ��� 
    unsigned short AlarmCode_U;    //������
    unsigned short AlarmCode_D;    //������    
    unsigned short RevCnt_U; 
    unsigned short RevCnt_D;     
    signed   short UpSpeed;        //�Ϸ����ٶ�
    signed   short DownSpeed;      //�·����ٶ�
    signed int     Down_LastPidspeed; //�·�����һ��PID�����ٶ�
    signed int     Up_LastPidspeed;   //�Ϸ�����һ��PID�����ٶ�
    signed int     Down_LastPos;      //�·�����һ�α���λ��
    signed int     Up_LastPos;        //�Ϸ�����һ�α���λ��
    unsigned int   PIDRefreshTime;   
    unsigned int   Upbtn_FlrTime;
    unsigned int   Downbtn_FlrTime;     
    unsigned short UpTensionPos;   //�Ϸ�������λ��
    unsigned short DownTensionPos; //�·�������λ��    
    unsigned short UpJogSpeed;     //�Ϸ��ϵ㶯�ٶ�
    unsigned short DownJogSpeed;   //�·��ϵ㶯�ٶ�  

    signed short UpPerlimit;     //�Ϸ�����������λ��
    signed short UpLowerlimi;    //�Ϸ�����������λ��    
    signed short DownPerlimit;   //�·�����������λ��
    signed short DownLowerlimi;  //�·�����������λ��     
    
}RunParameter;



extern RunParameter Discharge;        //�������в���


void DischargeRunloopTask(void);
//GUI
void Up_DischargeStop(void);
void Down_DischargeStop(void);
void Up_DischargeJog(unsigned char,unsigned char);
void Down_DischargeJog(unsigned char,unsigned char);
void Up_ServoAxisLockOrRelease(unsigned char enb);
void Down_ServoAxisLockOrRelease(unsigned char enb);


#endif






