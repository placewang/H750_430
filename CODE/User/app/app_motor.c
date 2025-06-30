#include "bsp_dcan.h"
#include "app_motor.h"

#include <string.h>
#include <stdio.h>

unsigned char G_send_buf[8];


/*****************************************************************
APP_MOTOR_send_get_status:��ȡ�����״̬
	id�������ID
******************************************************************/
void APP_MOTOR_send_get_status(unsigned char id)
{
	G_send_buf[0] = CMD_HQ_MOTOR_GET_STATUS;
	G_send_buf[1] = id;
	
	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 2);
}

/*****************************************************************
APP_MOTOR_poweron:����ϵ�
	id�������ID
******************************************************************/
void APP_MOTOR_poweron(unsigned char id)
{
	G_send_buf[0] = CMD_HQ_MOTOR_ENABLE;
	G_send_buf[1] = id;
	G_send_buf[2] = 0xFF;
	G_send_buf[3] = 0XFF;
	
	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 4);
}

/*****************************************************************
APP_MOTOR_send_get_status:��ȡ�����״̬
	id�������ID
******************************************************************/
void APP_MOTOR_stop(unsigned char id)
{
	G_send_buf[0] = CMD_HQ_MOTOR_MOVE_STOP_NOW;
	G_send_buf[1] = id;
	
	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 2);
}


/*****************************************************************
APP_MOTOR_poweroff:����µ�
	id�������ID
******************************************************************/
void APP_MOTOR_poweroff(unsigned char id)
{
	G_send_buf[0] = CMD_HQ_MOTOR_ENABLE;
	G_send_buf[1] = id;
	G_send_buf[2] = 0x00;
	G_send_buf[3] = 0X00;
	
	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 4);
}


/*****************************************************************
APP_MOTOR_set_code:������õ�ǰ����ֵ��λ�ã�
	id�������ID
	code������ֵ
******************************************************************/
void APP_MOTOR_set_code(unsigned char id, int code)
{
	struct LOOP_MOTOR_MSG_SET_PARAMETER *set_p;
	memset(G_send_buf, 0x00, 8);
	
	set_p = (struct LOOP_MOTOR_MSG_SET_PARAMETER *)G_send_buf;
	set_p->cmd = CMD_HQ_MOTOR_SET_PARAMETER;
	set_p->id = id;
	set_p->parameter_type = MOTOR_PARAMETER_TYPE_SET_CODE;
	set_p->parameter.data_32 = code;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_set_code:������õ�ǰ����ֵ��λ�ã�
	id�������ID
	code������ֵ
******************************************************************/
void APP_MOTOR_set_position(unsigned char id, int position)
{
	struct LOOP_MOTOR_MSG_SET_PARAMETER *set_p;
	memset(G_send_buf, 0x00, 8);
	
	set_p = (struct LOOP_MOTOR_MSG_SET_PARAMETER *)G_send_buf;
	set_p->cmd = CMD_HQ_MOTOR_SET_PARAMETER;
	set_p->id = id;
	set_p->parameter_type = MOTOR_PARAMETER_TYPE_SET_POSITION;
	set_p->parameter.data_32 = position;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_set_zero_offset:�������ײ����ƫ����
	id�������ID
	code������ֵ
******************************************************************/
void APP_MOTOR_set_zero_offset(unsigned char id, int offset)
{
	struct LOOP_MOTOR_MSG_SET_PARAMETER *set_p;
	memset(G_send_buf, 0x00, 8);
	
	set_p = (struct LOOP_MOTOR_MSG_SET_PARAMETER *)G_send_buf;
	set_p->cmd = CMD_HQ_MOTOR_SET_PARAMETER;
	set_p->id = id;
	set_p->parameter_type = MOTOR_PARAMETER_TYPE_SET_OFFSET;
	set_p->parameter.data_32 = offset;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_set_direction:	������÷���
	id�������ID
	dir���˶�������:0˳ʱ�� 1��ʱ��
******************************************************************/
void APP_MOTOR_set_direction(unsigned char id, unsigned char move_dir, unsigned char find_zero_dir)
{
	struct LOOP_MOTOR_MSG_SET_PARAMETER *set_p;
	memset(G_send_buf, 0x00, 8);

	set_p = (struct LOOP_MOTOR_MSG_SET_PARAMETER *)G_send_buf;
	set_p->cmd = CMD_HQ_MOTOR_SET_PARAMETER;
	set_p->id = id;
	set_p->parameter_type = MOTOR_PARAMETER_TYPE_SET_DIR;
	set_p->parameter.data_8[0] = move_dir;
	set_p->parameter.data_8[1] = 0;
	set_p->parameter.data_8[2] = find_zero_dir;
	set_p->parameter.data_8[3] = 0;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_set_code_ratio:�������λ��ֵ����
	id�������ID
	denominator����ĸ
	molecule������
******************************************************************/
void APP_MOTOR_set_code_ratio(unsigned char id, unsigned short denominator, unsigned short molecule)
{
	struct LOOP_MOTOR_MSG_SET_PARAMETER *set_p;
	memset(G_send_buf, 0x00, 8);

	set_p = (struct LOOP_MOTOR_MSG_SET_PARAMETER *)G_send_buf;
	set_p->cmd = CMD_HQ_MOTOR_SET_PARAMETER;
	set_p->id = id;
	set_p->parameter_type = MOTOR_PARAMETER_TYPE_SET_CODE_RATIO;
	set_p->parameter.data_16[0] = denominator;
	set_p->parameter.data_16[1] = molecule;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_set_zero_type:����������㷽ʽ
	id�������ID
	type:Ĭ�ϴ�����ģʽ ��ֵΪ1����ײͣģʽ�����ó�0��
******************************************************************/
void APP_MOTOR_set_zero_type(unsigned char id, unsigned char type)
{
	struct LOOP_MOTOR_MSG_SET_PARAMETER *set_p;
	memset(G_send_buf, 0x00, 8);

	set_p = (struct LOOP_MOTOR_MSG_SET_PARAMETER *)G_send_buf;
	set_p->cmd = CMD_HQ_MOTOR_SET_PARAMETER;
	set_p->id = id;
	set_p->parameter_type = MOTOR_PARAMETER_TYPE_FIND_ZERO_TYPE;
	set_p->parameter.data_8[0] = type;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_set_zero_report:��λ״̬�ϱ�
	id�������ID
	type:Ĭ�ϴ�����ģʽ ��ֵΪ1����ײͣģʽ�����ó�0��
******************************************************************/
void APP_MOTOR_set_zero_report(unsigned char id, unsigned char enable)
{
	struct LOOP_MOTOR_MSG_SET_PARAMETER *set_p;
	memset(G_send_buf, 0x00, 8);

	set_p = (struct LOOP_MOTOR_MSG_SET_PARAMETER *)G_send_buf;
	set_p->cmd = CMD_HQ_MOTOR_SET_PARAMETER;
	set_p->id = id;
	set_p->parameter_type = MOTOR_PARAMETER_TYPE_SET_ZERO_REPORT;
	set_p->parameter.data_8[0] = enable;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}


/*****************************************************************
APP_MOTOR_find_zero:	�����������
	id�������ID
	speed��������ٶȣ�����ٶȰٷֱ�
	torque��Ť��ǧ�ֱ�
******************************************************************/
void APP_MOTOR_find_zero(unsigned char id, unsigned short speed, unsigned short torque)
{
	struct LOOP_MOTOR_MSG_FIND_ZERO *find_zero_p;
	memset(G_send_buf, 0x00, 8);

	find_zero_p = (struct LOOP_MOTOR_MSG_FIND_ZERO *)G_send_buf;
	find_zero_p->cmd = CMD_HQ_MOTOR_FIND_ZERO_BY_SENSOR;
	find_zero_p->id = id;
	find_zero_p->speed = speed;
	find_zero_p->torque = torque;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_bump_zero:	���ײ������
	id�������ID
	speed��������ٶȣ�����ٶȰٷֱ�
	torque��Ť��ǧ�ֱ�
******************************************************************/
void APP_MOTOR_bump_zero(unsigned char id, unsigned short speed, unsigned short torque)
{
	struct LOOP_MOTOR_MSG_FIND_ZERO *find_zero_p;
	memset(G_send_buf, 0x00, 8);

	find_zero_p = (struct LOOP_MOTOR_MSG_FIND_ZERO *)G_send_buf;
	find_zero_p->cmd = CMD_HQ_MOTOR_FIND_ZERO_BY_HIT;
	find_zero_p->id = id;
	find_zero_p->speed = speed;
	find_zero_p->torque = torque;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_set_bump_zero_report:	���ײ������
	id�������ID
******************************************************************/
void APP_MOTOR_set_bump_zero_report(unsigned char id)
{
	struct LOOP_MOTOR_MSG_SET_PARAMETER *zero_report_p;
	memset(G_send_buf, 0x00, 8);

	zero_report_p = (struct LOOP_MOTOR_MSG_SET_PARAMETER *)G_send_buf;
	zero_report_p->cmd = CMD_HQ_MOTOR_SET_ARRIVAL_IND;
	zero_report_p->id = id;
	zero_report_p->parameter_type = 1;
	zero_report_p->parameter.udata_32 = 0;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_move_position:����ƶ���λ��ģʽ��
	id�������ID
	speed��������ٶȣ�Ȧ/��
	position���ƶ���λ��
	ack_flag������Ӧģʽ
******************************************************************/
void APP_MOTOR_move_position(unsigned char id, unsigned short speed, int position, unsigned char ack_flag)
{
	struct LOOP_MOTOR_MSG_MOVE_P *motor_p;
	memset(G_send_buf, 0x00, 8);

	motor_p = (struct LOOP_MOTOR_MSG_MOVE_P *)G_send_buf;
	if (ack_flag)
	{
		motor_p->cmd = CMD_HQ_MOTOR_MOVE_P_WITH_ACK;
	}
	else
	{
		motor_p->cmd = CMD_HQ_MOTOR_MOVE_P_WITHOUT_ACK;
	}
	
	motor_p->id = id;
	motor_p->speed = speed;
	motor_p->position = position;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_move_speed:����ƶ����ٶ�ģʽ��
	id�������ID
	speed��������ٶȣ�Ȧ/��
	position���ƶ���λ��
	ack_flag������Ӧģʽ
******************************************************************/
void APP_MOTOR_move_speed(unsigned char id, short speed, unsigned short torque)
{
	struct LOOP_MOTOR_MSG_MOVE_S *motor_p;
	memset(G_send_buf, 0x00, 8);

	motor_p = (struct LOOP_MOTOR_MSG_MOVE_S *)G_send_buf;
	motor_p->cmd = CMD_HQ_MOTOR_MOVE_S_WITHOUT_ACK;
	motor_p->id = id;
	motor_p->speed = speed;
	motor_p->torque = torque;

	BSP_DCAN_send_msg(MOTOR_SEND_CAN_ID, G_send_buf, 8);
}

/*****************************************************************
APP_MOTOR_init: �������APP��ʼ������
******************************************************************/
void APP_MOTOR_init(void)
{
	
}


extern void APP_HP_msg_deal(unsigned short id, unsigned char *data_p, unsigned short len);
/*****************************************************************
APP_MOTOR_loop_1ms: �������APP��ѯ����
******************************************************************/
void APP_MOTOR_loop_1ms(void)
{
	static unsigned char recv_data[8];
	static unsigned short recv_len;
	static unsigned short recv_id;
	int i;

	for (i = 0; i < 8; i++)
	{
		recv_len = BSP_DCAN_get_msg(&recv_id, recv_data);
		if (recv_len > 0)
		{	
			//printf("APP_MOTOR_loop_1ms: id = %4x, len = %d, data = %02X %02X\r\n", recv_id, recv_len, recv_data[0], recv_data[1]);
			APP_HP_msg_deal(recv_id, recv_data, recv_len);
		}
		else
		{
			break;
		}
	}
}

