#ifndef _APP_TOP_H_
#define _APP_TOP_H_

// 事件
#define KEY_EVENT_DHP_MOVE_F	0x01		// 排刀正转
#define KEY_EVENT_DHP_MOVE_R	0x02		// 排刀反转
#define KEY_EVENT_DHP_FMOVE_F	0x03		// 排刀快速正转
#define KEY_EVENT_DHP_FMOVE_R	0x04		// 排刀快速反转
#define KEY_EVENT_HP_MOVE_F		0x10		// 电机正转
#define KEY_EVENT_HP_MOVE_R		0x11		// 电机反转
#define KEY_EVENT_HP_FMOVE_F	0x12		// 电机快速正转
#define KEY_EVENT_HP_FMOVE_R	0x13		// 电机快速反转
#define HWH_EVENT_MOVE			0x20		// 排刀转动
#define HWH_EVENT_SINGLE_MOVE	0x21		// 电机转动

#define KEY_EVENT_MOVE_STOP		0xF0		// 停止转动
#define KEY_EVENT_TIME_OUT		0xF1		// 超时
#define MOTOR_EVENT_MOVE_SUCC	0xF2		// 电机运行成功

// 状态
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



