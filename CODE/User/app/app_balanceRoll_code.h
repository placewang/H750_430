#ifndef __BALANCEROLL_H_
#define __BALANCEROLL_H_

#define  EncodeRefreshTime            10
#define  MOTORDATALEN                 8
#define  MOTORREVCANID                0x01  

#define  Encode_UP_CANID              0x102          //��ѹ������������ID         
#define  Encode_UPID                  0x02           //��ѹ��������Э��ID
#define  Encode_BELOW_CANID           0x101          //��ѹ������������ID         
#define  Encode_BELOWID               0x01          //��ѹ��������Э��ID


//���������
typedef struct {
	unsigned char ZeroHit;								//ײ��
	unsigned char Parameter;							//��������һ��ָ��			
	unsigned char Enable;							    //���ʹ��/ʧ��
	unsigned char MoveWthout;							//λ��ģʽ�˶�
	unsigned char MoveWith;								//λ��ģʽ�˶���ɺ󷵻ص�ǰλ��	
	unsigned char GetEncodervalue;						//����ǰ������λ��	
	unsigned char Read;									//�����һ��ָ��
	unsigned char Limit;								//�˶���������
	unsigned char Torque;                               //��ת��
	unsigned char RotatingSpeed;                        //��ת��
	unsigned char ClearALarm;                           //�屨��
	unsigned char MovezeroRev;                          //������ɺ��Զ��ظ�
	unsigned char OriginalEncodedVal;                   //��ԭʼ������ֵ
	unsigned char SeepMode;                             //�ٶ�ģʽ	
}MotorCmd;

//���������
typedef struct {
	unsigned short SetEncodervalue;                      //���ñ�����ֵ                         
	unsigned short SetDir;	                             //���õ����ת������
	unsigned short SetReturnZeroOffset;                  //���õ��ײͣ��ƫ����
	unsigned short SetHitstopPosValue;		             //���õ��ײͣ�������ֵ
	unsigned short SetFindZeroMode; 			         //���õ�����㷽ʽ
	unsigned short SetMaxSpeed;						     //���õ�����ת��
	unsigned short SetPosvalue;						     //���õ��λ��
	unsigned short SetBaud;							     //����ͨѶ������
	unsigned short SetStartStiffness;                    //���õ����������
	unsigned short SetStopStiffness;	                 //���õ��ֹͣ����
	unsigned short SetLimit;                             //���õ���˶����ޣ���С��
	unsigned short ElectronicGearRatio;                  //���õ�����ӳ��ֱ�
    unsigned short VersionSwitch;                        //Э��汾�л�
}MotorSubCmd;


extern MotorCmd     MCmd;
extern MotorSubCmd	 MSCmd;
        

typedef struct
{
   unsigned char random[4];    
   signed char  IDcfg;  
   signed char  CodeRefreshMark_UP;  //�Ϸ��ϱ���ˢ�±��
   signed char  CodeRefreshMark_DW;  //�·��ϱ���ˢ�±��
   signed char  EncoderUpDir;        //�ϵ�Ĭ�Ϸ���
   signed char  EncoderDownDir;      //�ϵ�Ĭ�Ϸ���
    
   unsigned short SendRevCnt_U; 
   unsigned short SendRevCnt_D;
   signed short   LastPos_U;         //��һ�α���λ��
   signed short   LastPos_D;         //��һ�α���λ��     
   signed short UpReal_TimePos;      //�Ϸ��ϱ�����ʵʱλ��
   signed short DownReal_TimePos;    //�·��ϱ�����ʵʱλ��
   unsigned int ReadIntervalTime;       
    
}Encode;
                              


extern Encode Encode_handle;


void BalanceRollEncodeInit(void);
signed char BalanceRollEncodeValLoadloopTask(const unsigned char);
signed char EncodeSendCanData(unsigned char * dataval,\
							  const unsigned int num,\
							  const unsigned int Dev_num);
void TensionCalibrationValClear(unsigned char id);
signed char app_BalanceRoll_ReadEncodeVal(const unsigned char);
#endif

