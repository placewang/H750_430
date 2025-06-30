#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#define APP_CONFIG_START_PID	0x00000005	// ��ʼPID������Ϊ0x5AA5AA55
#define APP_CONFIG_END_PID		0x00000000	// ����PID������Ϊ��¼������
#define APP_CONFIG_START_FLAG	0x5AA5AA55	// ��ʼPID�ı�־


// ���ϲ����Ӳ���
#define APP_CONFIG_PID_RUN_MODE		            0x1000		//��������ģʽ
#define APP_CONFIG_PID_UP_MOVE_ENAE		        0x1001		//�Ϸ��Ϲ���ʹ��
#define APP_CONFIG_PID_UP_DRI           		0x1002		//�Ϸ��Ϸ���
#define APP_CONFIG_PID_UP_TENSION_POS           0x1003      //�Ϸ�������λ��
#define APP_CONFIG_PID_UP_JOG_SPEED             0x1004      //�Ϸ��ϵ㶯�ٶ�
#define APP_CONFIG_PID_UP_PERlIMIT              0x1009      //�Ϸ�����������λ��
#define APP_CONFIG_PID_UP_LOWERlIMIT            0x100A      //�Ϸ�����������λ�� 
#define APP_CONFIG_PID_UP_ENCODERFDIR            0x100D      //�ϱ������ϵ�Ĭ�Ϸ���

#define APP_CONFIG_PID_DOWN_MOVE_ENAE		    0x1005		//�·��Ϲ���ʹ��
#define APP_CONFIG_PID_DOWN_DRI           		0x1006		//�·��Ϸ���
#define APP_CONFIG_PID_DOWN_TENSION_POS         0x1007      //�·�������λ��
#define APP_CONFIG_PID_DOWN_JOG_SPEED           0x1008      //�·��ϵ㶯�ٶ�
#define APP_CONFIG_PID_DOWN_PERlIMIT            0x100B      //�·�����������λ��
#define APP_CONFIG_PID_DOWN_LOWERlIMIT          0x100C      //�·�����������λ�� 
#define APP_CONFIG_PID_DOWN_ENCODERFDIR          0x100E      //�±������ϵ�Ĭ�Ϸ���
#define APP_CONFIG_PID_LANGUAGE_MODE		    0x1010 

#define APP_CONFIG_PID_DEV_MODE		            0x100F		//�豸ģʽ
#define APP_CONFIG_PID_LANGUAGE_MODE		    0x1010      //�����л�

void APP_CONFIG_load(void);
void APP_CONFIG_save(void);

signed char  APP_AddConfigParameterToFlash(void);

#endif	// _APP_CONFIG_H_



