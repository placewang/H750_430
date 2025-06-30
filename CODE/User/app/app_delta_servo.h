#ifndef __DELTASERVO_H_
#define __DELTASERVO_H_

#define  UP_NODE_ID    0x02     //�Ϸ��Ͻڵ�ID
#define  DOWN_NODE_ID  0x01     //�·��Ͻڵ�ID


/*
��׼�豸��������ֵ��ж���DS402
*/

#define OD_CONTROL_WORD        0x6040 //����������  bit16
#define OD_STATUS_WORD         0x6041 //������״̬  bit16
#define OD_ACC_TIME            0x6083 //����б�� ms bit32
#define OD_DEC_TIME            0x6084 //����б�� ms bit32
#define OD_SET_TARGET_SPEED    0x60FF //����Ŀ���ٶ�bit32
#define OD_READ_CUR_SPEED      0x606C //��ȡ��ǰ�ٶ�bit32
#define OD_SET_MODE            0x6060 //����ģʽ    bit8
#define OD_SET_TARGET_POSITION 0x607A //����Ŀ��λ��bit32
#define OD_SET_SPEED_LIMIT     0x6081 //����λ��ģʽ�������ٶ� bit32
#define OD_READ_CUR_POSITION   0x6064 //��ȡ��ǰλ�� bit32


/*
�ŷ��������ģʽ
*/
#define    SERVO_POSITION_MODE 1       //λ��ģʽ   
#define    SERVO_SPEED_MODE    3       //�ٶ�ģʽ 

extern char appDeltServoType;

void DELTA_Servo_init(void); 
void DELTA_Set_motorID(unsigned char mID);
void DELTA_Servo_GetSpeed(unsigned char ACCnodeid); 
void DELTA_Servo_SetMode(unsigned char node_id,unsigned char mode); 
void DELTA_Servo_SetTargetSpeed(unsigned char,signed int);
void DELTA_Servo_enable(unsigned char,unsigned char);
#endif


