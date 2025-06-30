#ifndef _APP_TOP_H_
#define _APP_TOP_H_

// �¼�
#define KEY_EVENT_DHP_MOVE_F	0x01		// �ŵ���ת
#define KEY_EVENT_DHP_MOVE_R	0x02		// �ŵ���ת
#define KEY_EVENT_DHP_FMOVE_F	0x03		// �ŵ�������ת
#define KEY_EVENT_DHP_FMOVE_R	0x04		// �ŵ����ٷ�ת
#define KEY_EVENT_HP_MOVE_F		0x10		// �����ת
#define KEY_EVENT_HP_MOVE_R		0x11		// �����ת
#define KEY_EVENT_HP_FMOVE_F	0x12		// ���������ת
#define KEY_EVENT_HP_FMOVE_R	0x13		// ������ٷ�ת
#define HWH_EVENT_MOVE			0x20		// �ŵ�ת��
#define HWH_EVENT_SINGLE_MOVE	0x21		// ���ת��

#define KEY_EVENT_MOVE_STOP		0xF0		// ֹͣת��
#define KEY_EVENT_TIME_OUT		0xF1		// ��ʱ
#define MOTOR_EVENT_MOVE_SUCC	0xF2		// ������гɹ�

// ״̬
#define TOP_STATUS_IDLE				0
#define TOP_STATUS_DHP_MOVE_F		1
#define TOP_STATUS_DHP_MOVE_R		2
#define TOP_STATUS_DHP_FMOVE_F		3
#define TOP_STATUS_DHP_FMOVE_R		4
#define TOP_STATUS_HP_MOVE_F		5
#define TOP_STATUS_HP_MOVE_R		6
#define TOP_STATUS_HP_FMOVE_F		7
#define TOP_STATUS_HP_FMOVE_R		8
#define TOP_STATUS_HWH_MOVE			9
#define TOP_STATUS_HWH_SINGLE_MOVE	10

void APP_TOP_init(void);
void APP_TOP_event_deal(unsigned char event, unsigned char flag, int data);
void APP_TOP_loop_1ms(void);

#endif	// _APP_TOP_H_



