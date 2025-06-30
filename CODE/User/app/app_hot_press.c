#include "app_hot_press.h"
#include "app_motor.h"
//#include "App_top.h"

#include <string.h>
#include <stdio.h>

#include "ui_my_deal.h"

//#define SCAN_USING_STEP_MOTOR		1


// ������ú�����״̬
//struct HOT_PRESS_MOTOR G_hot_press_motor[HOT_PRESS_MOTOR_MAX_NUM];
struct HOT_PRESS_EVENT G_motor_event[HOT_PRESS_EVENT_MAX_NUM];
unsigned char G_motor_event_wr = 0;
unsigned char G_motor_event_rd = 0;

unsigned char  G_motor_get_position_time_enable = 0;
unsigned short G_motor_get_position_time_count;

unsigned short G_motor_delay_startup_time = 0;


unsigned char G_vertical_use_num = 0;
struct HOT_PRESS_MOTOR G_vertical_motor[HOT_PRESS_MOTOR_VERTICAL_MAX_NUM];
unsigned char G_vertical_bed_use_num = 0;
struct HOT_PRESS_MOTOR G_vertical_bed_motor[HOT_PRESS_MOTOR_VERTICAL_MAX_NUM];

/*****************************************************************
APP_HP_get_motor_status: ���õ�ǰʹ�õĸ����͵����״̬
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
	index��������
����ֵ��
	0���ر�
	1������
	2������
	3������
******************************************************************/
unsigned char APP_HP_get_motor_status(unsigned char type, unsigned char index)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point(type, index);
	if (motor_p == NULL)
	{
		return 0;
	}
	
	if (motor_p->enable == 0)
	{
		return 0;
	}
	
	if (motor_p->status == HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		return 2;
	}
	
	if (motor_p->status > HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		return 3;
	}
	
	return 1;
}


void APP_HP_move_init(void)
{
	struct HOT_PRESS_MOTOR *motor_p;
	unsigned char motor_index;
	
	if (G_vertical_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_use_num; motor_index++)
		{
			motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_motor[motor_index];
			motor_p->target_position = motor_p->crt_position;
		}
	}

	if (G_vertical_bed_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_bed_use_num; motor_index++)
		{
			motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_bed_motor[motor_index];
			motor_p->target_position = motor_p->crt_position;
		}
	}
	
	
	
	
	
}

/*****************************************************************
APP_HP_set_motor_use_num: ���õ�ǰʹ�õĸ����͵������
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// ����
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// ����
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// ���
	num���������
******************************************************************/
void APP_HP_set_motor_use_num(unsigned char motor_type, unsigned char num)
{
	switch (motor_type)
	{
		case HOT_PRESS_MOTOR_TYPE_NULL:					// ��
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL:				// ���̣����̣�
			if (num <= HOT_PRESS_MOTOR_VERTICAL_MAX_NUM)
			{
				G_vertical_use_num = num;
			}
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL_BED:			// ���̴������̣�
			if (num <= HOT_PRESS_MOTOR_VERTICAL_MAX_NUM)
			{
				G_vertical_bed_use_num = num;
			}
			break;
		
		default:
			break;
	}
}

/*****************************************************************
APP_HP_set_use_num: ���õ�ǰʹ�õĸ����͵������
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// ����
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// ����
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// ���
����ֵ����ǰʹ�õĵ����
******************************************************************/
unsigned char APP_HP_get_motor_use_num(unsigned char motor_type)
{
	if (motor_type == HOT_PRESS_MOTOR_TYPE_VERTICAL)
	{
		return G_vertical_use_num;
	}
	else if (motor_type == HOT_PRESS_MOTOR_TYPE_VERTICAL_BED)
	{
		return G_vertical_bed_use_num;
	}
	
	return 0;
}


/*****************************************************************
APP_HP_get_motor_id: �������ͺ������ţ���ȡ��������Ľṹ��ָ��
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// ����
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// ����
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// ���
	motor_index���������
******************************************************************/
unsigned char APP_HP_get_motor_id(unsigned char motor_type, unsigned char motor_index)
{
	switch (motor_type)
	{
		case HOT_PRESS_MOTOR_TYPE_NULL:					// ��
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL:				// ���̣����̣�
			if (motor_index < G_vertical_use_num)
			{
				return G_vertical_motor[motor_index].id;
			}
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL_BED:			// ���̴������̣�
			if (motor_index < G_vertical_bed_use_num)
			{
				return G_vertical_bed_motor[motor_index].id;
			}
			break;
		
		default:
			break;
	}
	
	return 0xFE;
}


/*****************************************************************
APP_HP_get_motor_point: �������ͺ������ţ���ȡ��������Ľṹ��ָ��
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// ����
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// ����
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// ���
	motor_index���������
******************************************************************/
struct HOT_PRESS_MOTOR *APP_HP_get_motor_point(unsigned char motor_type, unsigned char motor_index)
{
	switch (motor_type)
	{
		case HOT_PRESS_MOTOR_TYPE_NULL:					// ��
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL:				// ���̣����̣�
			if (motor_index <= HOT_PRESS_MOTOR_VERTICAL_MAX_NUM)
			{
				return &G_vertical_motor[motor_index];
			}
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL_BED:			// ���̴������̣�
			if (motor_index <= HOT_PRESS_MOTOR_VERTICAL_MAX_NUM)
			{
				return &G_vertical_bed_motor[motor_index];
			}
			break;
			
		default:
			break;
	}
	
	return NULL;
}

/*****************************************************************
APP_HP_get_motor_point_by_id: �������ͺ������ţ���ȡ��������Ľṹ��ָ��
	id�����ID
******************************************************************/
struct HOT_PRESS_MOTOR *APP_HP_get_motor_point_by_id(unsigned char id)
{
	unsigned char motor_index;
	
	
	if (G_vertical_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_use_num; motor_index++)
		{
			if (G_vertical_motor[motor_index].id == id)
			{
				return &G_vertical_motor[motor_index];
			}
		}
	}

	if (G_vertical_bed_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_bed_use_num; motor_index++)
		{
			if (G_vertical_bed_motor[motor_index].id == id)
			{
				return &G_vertical_bed_motor[motor_index];
			}
		}
	}
	
	return NULL;
}


/*****************************************************************
APP_HP_set_motor_id: ���õ����ID
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
	motor_index���������
	id�����õĵ��ID
******************************************************************/
void APP_HP_set_motor_id(unsigned char motor_type, unsigned char motor_index, unsigned char id)
{
	struct HOT_PRESS_MOTOR *motor_p = NULL;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	
	if (motor_p != NULL)
	{
		motor_p->id = id;
		motor_p->type = motor_type;
	}
}

/*****************************************************************
APP_HP_set_motor_move_direction: ���õ�����˶�����
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
	motor_index���������
	move_dir���˶�����
******************************************************************/
void APP_HP_set_motor_move_direction(unsigned char motor_type, unsigned char motor_index, unsigned char move_dir)
{
	struct HOT_PRESS_MOTOR *motor_p = NULL;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	
	if (motor_p != NULL)
	{
		motor_p->move_direction = move_dir;
	}
}

/*****************************************************************
APP_HP_set_motor_ratio_denominator: ���õ����λ�ñ���ֵ�ķ�ĸ
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// ����
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// ����
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// ���
	motor_index���������
	denominator����ĸֵ
******************************************************************/
void APP_HP_set_motor_ratio_denominator(unsigned char motor_type, unsigned char motor_index, unsigned short denominator)
{
	struct HOT_PRESS_MOTOR *motor_p = NULL;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	
	if (motor_p != NULL)
	{
		motor_p->denominator = denominator;
	}
}

/*****************************************************************
APP_HP_set_motor_ratio_molecule: ���õ����λ�ñ���ֵ�ķ���
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// ����
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// ����
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// ���
	motor_index���������
	molecule������ֵ
******************************************************************/
void APP_HP_set_motor_ratio_molecule(unsigned char motor_type, unsigned char motor_index, unsigned short molecule)
{
	struct HOT_PRESS_MOTOR *motor_p = NULL;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	
	if (motor_p != NULL)
	{
		motor_p->molecule = molecule;
	}
}

/*****************************************************************
APP_HP_set_motor_trim_speed: ���õ����΢���ٶ�
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// ����
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// ����
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// ���
	motor_index���������
	speed���ٶ�
******************************************************************/
void APP_HP_set_motor_trim_speed(unsigned char motor_type, unsigned char motor_index, unsigned short speed)
{
	struct HOT_PRESS_MOTOR *motor_p = NULL;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	
	if (motor_p != NULL)
	{
		motor_p->speed_mode_trim_speed = speed;
	}
}

/*****************************************************************
APP_HP_get_motor_current_position: ��ȡ��ǰ�ĵ��λ��
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
	motor_index���������
******************************************************************/
int APP_HP_get_motor_current_position(unsigned char motor_type, unsigned char motor_index)
{
	struct HOT_PRESS_MOTOR *motor_p = NULL;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	
	if (motor_p != NULL)
	{
		return motor_p->crt_position;
	}
	return 0;
}

/*****************************************************************
APP_HP_get_motor_current_position_by_id: ��ȡ��ǰ�ĵ��λ��
	motor_id�����ID
******************************************************************/
int APP_HP_get_motor_current_position_by_id(unsigned char motor_id)
{
	struct HOT_PRESS_MOTOR *motor_p = NULL;
	
	motor_p = APP_HP_get_motor_point_by_id(motor_id);
	
	if (motor_p != NULL)
	{
		return motor_p->crt_position;
	}
	return 0;
}

/*****************************************************************
APP_HP_get_motor_current_position: ��ȡ��ǰ�ĵ��λ��
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// ����
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// ����
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// ���
	motor_index���������
******************************************************************/
int APP_HP_get_motor_target_position(unsigned char motor_type, unsigned char motor_index)
{
	struct HOT_PRESS_MOTOR *motor_p = NULL;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	
	if (motor_p != NULL)
	{
		return motor_p->target_position;
	}
	return 0;
}

/*****************************************************************
APP_HP_get_motor_current_position_by_id: ��ȡ��ǰ�ĵ��λ��
	motor_id�����ID
******************************************************************/
int APP_HP_get_motor_target_position_by_id(unsigned char motor_id)
{
	struct HOT_PRESS_MOTOR *motor_p = NULL;
	
	motor_p = APP_HP_get_motor_point_by_id(motor_id);
	
	if (motor_p != NULL)
	{
		return motor_p->target_position;
	}
	return 0;
}

/*****************************************************************
APP_HP_print_motor_status: ��ӡ��ǰ�ĵ��״̬
******************************************************************/
void APP_HP_print_motor_status(void)
{
	unsigned char i;
	unsigned char zero_input;
	struct HOT_PRESS_MOTOR *motor_p;
	
	printf("vertical motor: %u\r\n", G_vertical_use_num);
	printf("  |SQ|ID    |ST|IN|IS|POSITION\r\n");
	printf("  |--+--+--+--+--+--------\r\n");
	if (G_vertical_use_num > 0)
	{
		for (i = 0; i < G_vertical_use_num; i++)
		{
			motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_motor[i];
			zero_input = 0;
			printf("  |%02X|%02X/%03d|%02X|%02x|%02x|%d\r\n", i, motor_p->id, motor_p->id, motor_p->status, zero_input, motor_p->motor_signal_status, motor_p->crt_position);
		}
	}
	printf("------------------------------------------\r\n");
	
	printf("vertical bed motor: %u\r\n", G_vertical_bed_use_num);
	printf("  |SQ|ID    |ST|IN|IS|POSITION\r\n");
	printf("  |--+--+--+--+--+--------\r\n");
	if (G_vertical_bed_use_num > 0)
	{
		for (i = 0; i < G_vertical_bed_use_num; i++)
		{
			motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_bed_motor[i];
			zero_input = 0;
			//printf("  |%02X|%02X|%02X|%02x|%d\r\n", i, motor_p->id, motor_p->status, zero_input, motor_p->crt_position);
			printf("  |%02X|%02X/%03d|%02X|%02x|%02x|%d\r\n", i, motor_p->id, motor_p->id, motor_p->status, zero_input, motor_p->motor_signal_status, motor_p->crt_position);
		}
	}
	printf("------------------------------------------\r\n");

}

/*****************************************************************
APP_HP_add_event: ��ӵ�������¼�
	motor_p����������ṹ��ָ��
	event���¼�����
******************************************************************/
static void APP_HP_add_event(struct HOT_PRESS_MOTOR *motor_p, unsigned char event)
{
	if (motor_p != NULL)
	{
		G_motor_event[G_motor_event_wr].motor_p = motor_p;
		G_motor_event[G_motor_event_wr].event = event;
		G_motor_event_wr++;
	}
}

/*****************************************************************
APP_HP_set_expected_reply: �����ڴ��յ��Ļظ���������ó�ʱʱ��
	motor_p����������ṹ��ָ��
	reply_command���ڴ��ظ�������
	time_count����ʱʱ��
******************************************************************/
static void APP_HP_set_expected_reply(struct HOT_PRESS_MOTOR *motor_p, unsigned char reply_command, unsigned short time_count)
{
	if (motor_p != NULL)
	{
		motor_p->expected_reply = reply_command;
		motor_p->time_count = time_count;
	}
}

/*****************************************************************
APP_HP_set_timer: ���ó�ʱʱ��
	time_count����ʱʱ��
******************************************************************/
static void APP_HP_set_timer(struct HOT_PRESS_MOTOR *motor_p, unsigned short time_count)
{
	if (motor_p != NULL)
	{
		motor_p->time_count = time_count;
	}
}


/*****************************************************************
APP_HP_motor_init: �����ʼ�������е�������ʼ������
******************************************************************/
void APP_HP_motor_init(void)
{
	struct HOT_PRESS_MOTOR *motor_p;
	unsigned char motor_index;
	
	if (G_vertical_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_use_num; motor_index++)
		{
			motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_motor[motor_index];
			if (motor_p->enable)
			{
				APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_GET_STATUS, 1000);
				motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S1;
			}
		}
	}

	if (G_vertical_bed_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_bed_use_num; motor_index++)
		{
			motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_bed_motor[motor_index];
			if (motor_p->enable)
			{
				APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_GET_STATUS, 1000);
				motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S1;
			}
		}
	}

	APP_MOTOR_send_get_status(0xFF);	// ���͹㲥��״̬����
	G_motor_get_position_time_enable = 1;
}

/*****************************************************************
APP_HP_send_status_alarm: ���͵���ĵ�ǰ״̬�澯
******************************************************************/
void APP_HP_send_status_alarm(struct HOT_PRESS_MOTOR *motor_p)
{
	if (motor_p == NULL)
	{
		return;
	}
	
	if (motor_p->status == HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		//APP_ALARM_send_motor_alarm(APP_COMM_MSG_ALARM_PID_FAULT, motor_p->type, motor_p->index, ERROR_ID_OFFLINE);
	}
	else if (motor_p->status == HOT_PRESS_MOTOR_STATUS_FAILED_1)
	{
		//APP_ALARM_send_motor_alarm(APP_COMM_MSG_ALARM_PID_FAULT, motor_p->type, motor_p->index, ERROR_ID_NOT_READY);
	}
	else if (motor_p->status == HOT_PRESS_MOTOR_STATUS_FAILED_2)
	{
		//APP_ALARM_send_motor_alarm(APP_COMM_MSG_ALARM_PID_FAULT, motor_p->type, motor_p->index, ERROR_ID_FIND_ZERO_FAILED);
	}
	else if (motor_p->status == HOT_PRESS_MOTOR_STATUS_FAILED_3)
	{
		//APP_ALARM_send_motor_alarm(APP_COMM_MSG_ALARM_PID_FAULT, motor_p->type, motor_p->index, ERROR_ID_MOTOR_MOVE_FUALED);
	}
	else if (motor_p->status == HOT_PRESS_MOTOR_STATUS_FAILED_4)
	{
		//APP_ALARM_send_motor_alarm(APP_COMM_MSG_ALARM_PID_FAULT, motor_p->type, motor_p->index, ERROR_ID_INPUT_SIGNAL_ABNORMAL);
	}
}


/*****************************************************************
APP_HP_start_speed_move: �����ٶ�ģʽ
	motor_type���������
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// ���̣����̣�
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// ���̴������̣�
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// ����
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// ����
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// ���
	motor_index�������
	speed�������ٶȵķ��� 1���� 0��ֹͣ�� -1����
����ֵ��0������
		1����������
		2������쳣
		3��״̬����
******************************************************************/
unsigned char APP_HP_speed_move(unsigned char motor_type, unsigned char motor_index, short speed_flag)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	if (motor_p == NULL)
	{
		return 1;
	}
	

	if (motor_p->status >= HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{// ״̬����
		APP_HP_send_status_alarm(motor_p);
		return 2;
	}
	
	if (speed_flag == 0)
	{// �ٶ�Ϊ0���ر�
		motor_p->running_speed = 0;
		APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_STOP_SPEED_MOVE);
		return 0;
	}
	else if (motor_p->status != HOT_PRESS_MOTOR_STATUS_RUNNING)
	{// æ
		return 3;
	}

	motor_p->running_speed = motor_p->speed_mode_move_speed * speed_flag;
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_START_SPEED_MOVE);
	return 0;
}

/*****************************************************************
APP_HP_speed_move_by_id: ͨ��ID�������ٶ�ģʽ
	id�����ID
	speed�������ٶȵķ��� 1���� 0��ֹͣ�� -1����
����ֵ��0������
		1����������
		2������쳣
		3��״̬����
******************************************************************/
unsigned char APP_HP_speed_move_by_id(unsigned char id, short speed_flag)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point_by_id(id);
	if (motor_p == NULL)
	{
		return 1;
	}
	

	if (motor_p->status >= HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		APP_HP_send_status_alarm(motor_p);
		return 2;
	}
	
	if (speed_flag == 0)
	{// �ٶ�Ϊ0���ر�
		motor_p->running_speed = 0;
		APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_STOP_SPEED_MOVE);
		return 0;
	}
	else if (motor_p->status != HOT_PRESS_MOTOR_STATUS_RUNNING)
	{
		return 3;
	}

	motor_p->running_speed = motor_p->speed_mode_move_speed * speed_flag;
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_START_SPEED_MOVE);
	return 0;
}

/*****************************************************************
APP_HP_trim_move_by_id: ͨ��ID������΢��ģʽ
	id�����ID
	speed�������ٶȵķ��� 1���� 0��ֹͣ�� -1����
����ֵ��0������
		1����������
		2������쳣
		3��״̬����
******************************************************************/
unsigned char APP_HP_trim_move_by_id(unsigned char id, short speed_flag)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point_by_id(id);
	if (motor_p == NULL)
	{
		return 1;
	}

	if (motor_p->status >= HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		APP_HP_send_status_alarm(motor_p);
		return 2;
	}
	
	if (speed_flag == 0)
	{// �ٶ�Ϊ0���ر�
		motor_p->running_speed = 0;
		APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_STOP_SPEED_MOVE);
		return 0;
	}
	else if (motor_p->status != HOT_PRESS_MOTOR_STATUS_RUNNING)
	{
		return 3;
	}

	motor_p->running_speed = motor_p->speed_mode_trim_speed * speed_flag;
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_START_SPEED_MOVE);
	return 0;
}

/*****************************************************************
APP_HP_position_offset_move: �����ٶ�ģʽ
	motor_index�������
	speed�������ٶ�
����ֵ��0������
		1����������
		2������쳣
		3��״̬����
******************************************************************/
unsigned char APP_HP_position_offset_move(unsigned char motor_type, unsigned char motor_index, int offset)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	if (motor_p == NULL)
	{
		return 1;
	}
	
	if (motor_p->status >= HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		APP_HP_send_status_alarm(motor_p);
		return 2;
	}
	
	else if (motor_p->status != HOT_PRESS_MOTOR_STATUS_RUNNING)
	{
		return 3;
	}

	motor_p->running_position_offset = offset;
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_START_OFFSET_MOVE);
	return 0;
}

/*****************************************************************
APP_HP_position_offset_move: �����ٶ�ģʽ
	motor_index�������
	speed�������ٶ�
����ֵ��0������
		1����������
		2������쳣
		3��״̬����
******************************************************************/
unsigned char APP_HP_position_offset_move_by_id(unsigned char id, int offset)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point_by_id(id);
	if (motor_p == NULL)
	{
		return 1;
	}
	
	if (motor_p->status >= HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		APP_HP_send_status_alarm(motor_p);
		return 2;
	}
	
	else if (motor_p->status != HOT_PRESS_MOTOR_STATUS_RUNNING)
	{
		return 3;
	}

	motor_p->running_position_offset = offset;
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_START_OFFSET_MOVE);
	return 0;
}

/*****************************************************************
APP_HP_position_move_by_id: �����ٶ�ģʽ
	motor_index�������
	speed�������ٶ�
����ֵ��0������
		1����������
		2������쳣
		3��״̬����
******************************************************************/
unsigned char APP_HP_position_move_by_id(unsigned char id, int position)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point_by_id(id);
	if (motor_p == NULL)
	{
		return 1;
	}
	
	if (motor_p->status >= HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		APP_HP_send_status_alarm(motor_p);
		return 2;
	}
	
	else if (motor_p->status != HOT_PRESS_MOTOR_STATUS_RUNNING)
	{
		return 3;
	}

	motor_p->running_position = position;
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_START_POSIT_MOVE);
	return 0;
}



/*****************************************************************
APP_HP_position_trim_by_id: ����λ��΢��ģʽ��ֱ�ӷ������ظ���λ���ƶ���
******************************************************************/
unsigned char APP_HP_position_trim_by_id(unsigned char id, int offset)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point_by_id(id);
	if (motor_p == NULL)
	{
		return 1;
	}
	
	if (motor_p->status >= HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		APP_HP_send_status_alarm(motor_p);
		return 2;
	}
	else if ((motor_p->status != HOT_PRESS_MOTOR_STATUS_RUNNING) && (motor_p->status != HOT_PRESS_MOTOR_STATUS_MOVE_TRIM_S1))
	{
		return 3;
	}

	motor_p->running_position_offset = offset;
	if (offset > 0)
	{
		motor_p->running_dir = 1;
	}
	else
	{
		motor_p->running_dir = 0;
	}
	motor_p->fast_update_position_enable = 1;
	motor_p->fast_update_position_time = 150;
	
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_START_TRIM_MOVE);
	return 0;
}

/*****************************************************************
APP_HP_position_follow_by_id: ����λ�ø���ģʽ��ֱ�ӷ������ظ���λ���ƶ���
******************************************************************/
unsigned char APP_HP_position_follow_by_id(unsigned char id, int position)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point_by_id(id);
	if (motor_p == NULL)
	{
		return 1;
	}
	
	if (motor_p->status >= HOT_PRESS_MOTOR_STATUS_FAILED_0)
	{
		APP_HP_send_status_alarm(motor_p);
		return 2;
	}
	else if ((motor_p->status != HOT_PRESS_MOTOR_STATUS_RUNNING) && (motor_p->status != HOT_PRESS_MOTOR_STATUS_MOVE_FOLLOW_S1))
	{
		return 3;
	}

	motor_p->running_position = position;

//	motor_p->fast_update_position_enable = 1;
//	motor_p->fast_update_position_delay_disable = 0;
//	motor_p->fast_update_position_time = 150;
	//APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed, motor_p->running_position, 0);
	//APP_MOTOR_move_position(motor_p->id, motor_p->position_mode_speed, motor_p->running_position, 0);
	
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_START_FOLLOW_MOVE);
	return 0;
}

/*****************************************************************
APP_HP_set_current_position: ���õ�ǰλ��
	motor_type���������
	motor_index�������
	posiition���趨��λ��
******************************************************************/
void APP_HP_set_current_position(unsigned char motor_type, unsigned char motor_index, int position)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	if (motor_p == NULL)
	{
		return;
	}
	
	APP_MOTOR_set_code(motor_p->id, position);
}


unsigned char APP_HP_motor_stop_by_id(unsigned char id)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point_by_id(id);
	if (motor_p == NULL)
	{
		return 1;
	}
	
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_MOTOR_STOP);
	return 0;
}

unsigned char APP_HP_motor_stop(unsigned char motor_type, unsigned char motor_index)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point(motor_type, motor_index);
	if (motor_p == NULL)
	{
		return 1;
	}
	
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_MOTOR_STOP);
	return 0;
}

/*****************************************************************
APP_HP_status_check: ���״̬���
����ֵ��0��״̬������1��״̬����
******************************************************************/
unsigned char APP_HP_status_check(int *motor_type_p, int *motor_index_p)
{
	unsigned char motor_index;
	
	if (G_vertical_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_use_num; motor_index++)
		{
			if ((G_vertical_motor[motor_index].status >= HOT_PRESS_MOTOR_STATUS_FAILED_0) || 
				(G_vertical_motor[motor_index].status < HOT_PRESS_MOTOR_STATUS_RUNNING))
			{
				if (motor_type_p != NULL)
					*motor_type_p = HOT_PRESS_MOTOR_TYPE_VERTICAL;
				if (motor_index_p != NULL)
					*motor_index_p = motor_index;
				return 1;
			}
		}
	}

	if (G_vertical_bed_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_bed_use_num; motor_index++)
		{
			if ((G_vertical_bed_motor[motor_index].status >= HOT_PRESS_MOTOR_STATUS_FAILED_0) || 
				(G_vertical_bed_motor[motor_index].status < HOT_PRESS_MOTOR_STATUS_RUNNING))
			{
				if (motor_type_p != NULL)
					*motor_type_p = HOT_PRESS_MOTOR_TYPE_VERTICAL_BED;
				if (motor_index_p != NULL)
					*motor_index_p = motor_index;
				return 1;
			}
		}
	}
	
	return 0;
}

/*****************************************************************
APP_HP_init: ��ѹ�ŵ���ʼ��
******************************************************************/
void APP_HP_init(void)
{
	struct HOT_PRESS_MOTOR *motor_p;
	unsigned char motor_index;
	
	G_motor_event_wr = 0;
	G_motor_event_rd = 0;
	G_motor_get_position_time_count = 0;
	G_vertical_use_num = HOT_PRESS_MOTOR_VERTICAL_MAX_NUM;
	G_vertical_bed_use_num = HOT_PRESS_MOTOR_VERTICAL_MAX_NUM;
	
	for (motor_index = 0; motor_index < HOT_PRESS_MOTOR_VERTICAL_MAX_NUM; motor_index++)
	{
		motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_motor[motor_index];
		
		motor_p->enable = 0;
		motor_p->id = 0x00;
		motor_p->type = HOT_PRESS_MOTOR_TYPE_VERTICAL;
		motor_p->index = motor_index;
		motor_p->status = HOT_PRESS_MOTOR_STATUS_IDLE;
		motor_p->move_direction = 0;	// �˶�����
		motor_p->denominator = 1;
		motor_p->molecule = 1;
		motor_p->motor_signal_status = 0;
		motor_p->motor_signal2_status = 0;
		
		motor_p->reply_flag = 0;
		motor_p->expected_reply = 0;
		
		motor_p->time_count = 0;
		motor_p->exist_count = 0;
		
		motor_p->set_position = 0;
		motor_p->crt_position = 0;
		motor_p->target_position = 0;

		motor_p->move_speed = 100;
		motor_p->speed_mode_move_speed = HOT_PRESS_MOTOR_SPEED_MOVE_DEFAULT_SPEED;
		motor_p->speed_mode_move_torque = HOT_PRESS_MOTOR_SPEED_MOVE_DEFAULT_TORQUE;
		motor_p->speed_mode_trim_speed = HOT_PRESS_MOTOR_SPEED_TRIM_DEFAULT_SPEED;
		motor_p->speed_mode_trim_torque = HOT_PRESS_MOTOR_SPEED_TRIM_DEFAULT_TORQUE;
		motor_p->position_mode_speed = HOT_PRESS_MOTOR_POSITION_MOVE_DEFAULT_SPEED;
		
		motor_p->running_speed = 0;
		motor_p->running_position_offset = 0;
		
		motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_bed_motor[motor_index];
		
		motor_p->enable = 0;
		motor_p->id = 0x00;
		motor_p->type = HOT_PRESS_MOTOR_TYPE_VERTICAL_BED;
		motor_p->index = motor_index;
		motor_p->status = HOT_PRESS_MOTOR_STATUS_IDLE;
		motor_p->move_direction = 0;	// �˶�����
		motor_p->denominator = 1;
		motor_p->molecule = 1;
		motor_p->motor_signal_status = 0;
		motor_p->motor_signal2_status = 0;
		
		motor_p->reply_flag = 0;
		motor_p->expected_reply = 0;
		
		motor_p->time_count = 0;
		motor_p->exist_count = 0;
		
		motor_p->set_position = 0;
		motor_p->crt_position = 0;
		motor_p->target_position = 0;

		motor_p->move_speed = 100;
		motor_p->speed_mode_move_speed = HOT_PRESS_MOTOR_SPEED_MOVE_DEFAULT_SPEED;
		motor_p->speed_mode_move_torque = HOT_PRESS_MOTOR_SPEED_MOVE_DEFAULT_TORQUE;
		motor_p->speed_mode_trim_speed = HOT_PRESS_MOTOR_SPEED_TRIM_DEFAULT_SPEED;
		motor_p->speed_mode_trim_torque = HOT_PRESS_MOTOR_SPEED_TRIM_DEFAULT_TORQUE;
		motor_p->position_mode_speed = HOT_PRESS_MOTOR_POSITION_MOVE_DEFAULT_SPEED;
		
		motor_p->running_speed = 0;
		motor_p->running_position_offset = 0;
	}
	
	G_motor_delay_startup_time = 3000;
}

/*****************************************************************
APP_HP_timer_1ms: 1ms�Ķ�ʱ������
******************************************************************/
static void APP_HP_timer_1ms(void)
{
	struct HOT_PRESS_MOTOR *motor_p;
	unsigned char motor_index;
	
	if (G_vertical_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_use_num; motor_index++)
		{
			motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_motor[motor_index];
			if (motor_p->time_count)
			{
				motor_p->time_count--;
				if (motor_p->time_count == 0)
				{
					APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_TIMEOUT);
				}
			}
			
			if (motor_p->exist_count)
			{
				motor_p->exist_count--;
				if (motor_p->exist_count == 0)
				{
					motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;	// �������λ
					APP_HP_send_status_alarm(motor_p);
					printf("GXD:%s...%d\r\n", __FILE__, __LINE__);
				}
			}
			
			if (motor_p->fast_update_position_enable)
			{
				motor_p->fast_update_position_time++;
				if (motor_p->fast_update_position_time >= HOT_PRESS_MOTOR_FAST_UPDATE_POSITION_TIME)
				{
					motor_p->fast_update_position_time = 0;
					APP_MOTOR_send_get_status(motor_p->id);
				}
			}
			
		}
	}

	if (G_vertical_bed_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_bed_use_num; motor_index++)
		{
			motor_p = (struct HOT_PRESS_MOTOR *)&G_vertical_bed_motor[motor_index];
			if (motor_p->time_count)
			{
				motor_p->time_count--;
				if (motor_p->time_count == 0)
				{
					APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_TIMEOUT);
				}
			}
			
			if (motor_p->exist_count)
			{
				motor_p->exist_count--;
				if (motor_p->exist_count == 0)
				{
					motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;	// �������λ
					APP_HP_send_status_alarm(motor_p);
					printf("GXD:%s...%d\r\n", __FILE__, __LINE__);
				}
			}
			
			if (motor_p->fast_update_position_enable)
			{
				motor_p->fast_update_position_time++;
				if (motor_p->fast_update_position_time >= HOT_PRESS_MOTOR_FAST_UPDATE_POSITION_TIME)
				{
					motor_p->fast_update_position_time = 0;
					APP_MOTOR_send_get_status(motor_p->id);
				}
			}
			
		}
	}
}

/*****************************************************************
APP_HP_find_motor_by_id: ͨ��id�ҵ���Ӧ�ĵ���ṹ��ָ��
	motor_id�������ID
����ֵ������ṹ��ָ��
******************************************************************/
struct HOT_PRESS_MOTOR *APP_HP_find_motor_by_id(unsigned char motor_id)
{
	unsigned char motor_index;
	
	if (G_vertical_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_use_num; motor_index++)
		{
			if (motor_id == G_vertical_motor[motor_index].id)
			{
				return &G_vertical_motor[motor_index];
			}
		}
	}

	if (G_vertical_bed_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_bed_use_num; motor_index++)
		{
			if (motor_id == G_vertical_bed_motor[motor_index].id)
			{
				return &G_vertical_bed_motor[motor_index];
			}
		}
	}
	
	return NULL;
}

/*****************************************************************
APP_HP_find_motor_by_id: ͨ��id�ҵ���Ӧ�ĵ���ṹ��ָ��
	motor_id�������ID
����ֵ������ṹ��ָ��
******************************************************************/
unsigned char APP_HP_check_all_motor_idle(void)
{
	unsigned char motor_index;
	
	if (G_vertical_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_use_num; motor_index++)
		{
			if (G_vertical_motor[motor_index].enable)
			{
				if (G_vertical_motor[motor_index].status != HOT_PRESS_MOTOR_STATUS_RUNNING)
				{
					return 1;
				}
			}
		}
	}

	if (G_vertical_bed_use_num > 0)
	{
		for (motor_index = 0; motor_index < G_vertical_bed_use_num; motor_index++)
		{
			if (G_vertical_motor[motor_index].enable)
			{
				if (G_vertical_bed_motor[motor_index].status != HOT_PRESS_MOTOR_STATUS_RUNNING)
				{
					return 1;
				}
			}
		}
	}
	
	return 0;
}


/*****************************************************************
APP_HP_msg_deal: �յ������Ϣ����
	id���յ���CAN ID
	data_p���յ�������
	len�����ݲ��
******************************************************************/
void APP_HP_msg_deal(unsigned short id, unsigned char *data_p, unsigned short len)
{
	unsigned char command;
	unsigned char motor_id;
	unsigned char motor_signal_status;
	struct HOT_PRESS_MOTOR *motor_p;
	struct LOOP_MOTOR_ACK_GET_STATUS *ack_get_status_p;
	
	if ((id >= 0x100) && (id < 0x200))
	{
		command = data_p[0];
		motor_id = data_p[1];
		
		motor_p = APP_HP_find_motor_by_id(motor_id);
		if (motor_p == NULL)
		{
			return;
		}
		
		motor_p->exist_count = HOT_PRESS_MOTOR_EXIST_LIVE_TIME;
		
		if (command == CMD_HQ_MOTOR_GET_STATUS)
		{
			ack_get_status_p = (struct LOOP_MOTOR_ACK_GET_STATUS *)data_p;
			motor_p->crt_position = ack_get_status_p->position;
			
			motor_signal_status = (ack_get_status_p->signal >> 5) & 0x01;
			if (motor_signal_status != motor_p->motor_signal_status)
			{
				// �ź�1
				
				motor_p->motor_signal_status = motor_signal_status;
			}
			
			motor_signal_status = (ack_get_status_p->signal >> 6) & 0x01;
			if (motor_signal_status != motor_p->motor_signal2_status)
			{
				// �ź�2
				motor_p->motor_signal2_status = motor_signal_status;
			}
		}
		
		if (motor_p->expected_reply == command)
		{// �յ��ڴ�����Ϣ�������¼�
			APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS);
		}
	}
}

static void APP_HP_event_deal(void)
{
	struct HOT_PRESS_MOTOR *motor_p;
	unsigned char event;
	int offset;
//	int count;
	
	//for (count = 0; count < 8; count++)
	{
		if (G_motor_event_wr == G_motor_event_rd)
		{
			return;
		}
		
		motor_p = G_motor_event[G_motor_event_rd].motor_p;
		event = G_motor_event[G_motor_event_rd].event;
		G_motor_event_rd++;
		
		if (motor_p == NULL)
		{
			return;
		}
		

		switch (motor_p->status)
		{
			case HOT_PRESS_MOTOR_STATUS_IDLE:			// �ϵ�״̬����
				 break;
			
			case HOT_PRESS_MOTOR_STATUS_INIT_S1:		// ��ʼ�����裬�жϵ���Ƿ���ڣ�����71����
				if (event == HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS)
				{// �ظ��ɹ��������µ�
					APP_MOTOR_poweroff(motor_p->id);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_ENABLE, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S2;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ������������
					motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;
					APP_HP_send_status_alarm(motor_p);
					printf("GXD:%s...%d\r\n", __FILE__, __LINE__);
				}
				break;
			
			case HOT_PRESS_MOTOR_STATUS_INIT_S2:		// ��ʼ�����裬�µ�
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ�
					APP_MOTOR_set_zero_type(motor_p->id, 1);
					APP_HP_set_timer(motor_p, 100);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_SET_PARAMETER, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S3;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_INIT_S3:		// ��ʼ�����裬���ù��㷽ʽ
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ�
					APP_MOTOR_set_direction(motor_p->id, motor_p->move_direction, 0);
					APP_HP_set_timer(motor_p, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S4;
				}
				break;

			case HOT_PRESS_MOTOR_STATUS_INIT_S4:		// ��ʼ�����裬���÷���
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ�
					APP_MOTOR_set_code_ratio(motor_p->id, motor_p->denominator, motor_p->molecule);
					APP_HP_set_timer(motor_p, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S5;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_INIT_S5:		// ��ʼ�����裬���ñ���������.
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ�
					APP_MOTOR_set_zero_report(motor_p->id, 1);
					APP_HP_set_timer(motor_p, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S5_1;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_INIT_S5_1:		// ��ʼ�����裬���ñ���������.
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ�
					APP_MOTOR_set_code(motor_p->id, 0);
					APP_HP_set_timer(motor_p, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S5_2;
				}
				break;
			
			case HOT_PRESS_MOTOR_STATUS_INIT_S5_2:
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ�
					APP_MOTOR_poweron(motor_p->id);
					APP_HP_set_timer(motor_p, 1000);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S6;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_INIT_S6:		// ��ʼ�����裬���
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ�
					//APP_MOTOR_poweroff(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				break;
				

			case HOT_PRESS_MOTOR_STATUS_RUNNING:			// ������
				if (event == HOT_PRESS_MOTOR_EVENT_START_SPEED_MOVE)
				{
					motor_p->retry_count = 3;
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_GET_STATUS, 3000);
					APP_MOTOR_send_get_status(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S1;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_START_OFFSET_MOVE)
				{
					motor_p->retry_count = 3;
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_GET_STATUS, 3000);
					APP_MOTOR_send_get_status(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S1;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_START_POSIT_MOVE)
				{
					motor_p->retry_count = 3;
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_GET_STATUS, 3000);
					APP_MOTOR_send_get_status(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S1;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_START_TRIM_MOVE)
				{
					if (motor_p->running_position_offset > 500)
					{
						motor_p->running_position_offset = 500;
					}
					
					if (motor_p->running_position_offset < -500)
					{
						motor_p->running_position_offset = -500;
					}
					
					motor_p->running_position = motor_p->target_position + motor_p->running_position_offset;
					
					if (motor_p->running_position > motor_p->crt_position)
					{
						offset = motor_p->running_position - motor_p->crt_position;
					}
					else
					{
						offset = motor_p->crt_position - motor_p->running_position;
					}
					
					if (offset < 10)
					{
						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed, motor_p->running_position, 1);
					}
					else
					{
						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed * offset / 10, motor_p->running_position, 1);
					}
					
					
					motor_p->target_position = motor_p->running_position;
					
					//printf("F---APP_MOTOR_move_position(%d)--offset = %d\r\n", motor_p->running_position, offset);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_MOVE_ARRIVAL_IND, 5000);
					
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_TRIM_S1;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_START_FOLLOW_MOVE)
				{
					//motor_p->running_position_offset = motor_p->running_position - motor_p->crt_position;
					
					if (motor_p->running_position > motor_p->crt_position)
					{
						offset = motor_p->running_position - motor_p->crt_position;
					}
					else
					{
						offset = motor_p->crt_position - motor_p->running_position;
					}
					
					if (offset < 10)
					{
						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed, motor_p->running_position, 1);
					}
					else
					{
						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed * offset / 10, motor_p->running_position, 1);
					}
					motor_p->target_position = motor_p->running_position;
					//printf("F---APP_MOTOR_move_position(%d)--offset = %d\r\n", motor_p->running_position, offset);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_MOVE_ARRIVAL_IND, 5000);
					
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_FOLLOW_S1;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S1:
				if (event == HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS)
				{// �ظ��ɹ��������µ�
					APP_MOTOR_move_speed(motor_p->id, motor_p->running_speed, motor_p->speed_mode_move_torque);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S2;
					//APP_HP_start_move_check(motor_index);	// �����ƶ����
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_STOP_SPEED_MOVE)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ������������
					if (motor_p->retry_count)
					{
						motor_p->retry_count--;
						APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_GET_STATUS, 3000);
						APP_MOTOR_send_get_status(motor_p->id);
					}
					else
					{
						motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;
//						APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_FAIL, motor_p->type, motor_p->index, motor_p->status);
						APP_HP_send_status_alarm(motor_p);
						printf("GXD:%s...%d\r\n", __FILE__, __LINE__);
					}
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_MOTOR_STOP)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S2:
				if (event == HOT_PRESS_MOTOR_EVENT_STOP_SPEED_MOVE)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
					
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_MOTOR_MOVE_ERROR)
				{// ����ƶ��쳣
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_3;
					APP_HP_send_status_alarm(motor_p);
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_MOTOR_STOP)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S1:
				if (event == HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS)
				{// �ظ��ɹ�
					motor_p->set_position = motor_p->crt_position + motor_p->running_position_offset;
					if ((motor_p->running_position_offset < 10) && (motor_p->running_position_offset > -10))
					{
						APP_MOTOR_move_position(motor_p->id, 10, motor_p->set_position, 1);
					}
					else if ((motor_p->running_position_offset < 100) && (motor_p->running_position_offset > -100))
					{
						APP_MOTOR_move_position(motor_p->id, 30, motor_p->set_position, 1);
					}
					else if ((motor_p->running_position_offset < 500) && (motor_p->running_position_offset > -500))
					{
						APP_MOTOR_move_position(motor_p->id, 100, motor_p->set_position, 1);
					}
					else
					{
						APP_MOTOR_move_position(motor_p->id, 200, motor_p->set_position, 1);
					}
					
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_MOVE_ARRIVAL_IND, 30000);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S2;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ������������
					if (motor_p->retry_count)
					{
						motor_p->retry_count--;
						APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_GET_STATUS, 3000);
						APP_MOTOR_send_get_status(motor_p->id);
					}
					else
					{
						motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;
//						APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_FAIL, motor_p->type, motor_p->index, motor_p->status);
						APP_HP_send_status_alarm(motor_p);
						printf("GXD:%s...%d\r\n", __FILE__, __LINE__);
					}
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_MOTOR_STOP)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				break;

			case HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S2:
				if (event == HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS)
				{// �ظ��ɹ�
					APP_MOTOR_send_get_status(motor_p->id);
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ������������
					motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_FAIL, motor_p->type, motor_p->index, motor_p->status);
					APP_HP_send_status_alarm(motor_p);
					printf("GXD:%s...%d\r\n", __FILE__, __LINE__);
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_MOTOR_STOP)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				break;
				
//			case HOT_PRESS_MOTOR_STATUS_MOVE_OFFSET_S3:
//				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
//				{
//					APP_MOTOR_poweron(motor_p->id);
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
//					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
//				}
//				break;
			
			case HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S1:
				if (event == HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS)
				{// �ظ��ɹ�
					motor_p->set_position = motor_p->running_position;
					APP_MOTOR_move_position(motor_p->id, motor_p->position_mode_speed, motor_p->set_position, 1);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_MOVE_ARRIVAL_IND, 60000);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S2;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ������������
					if (motor_p->retry_count)
					{
						motor_p->retry_count--;
						APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_GET_STATUS, 3000);
						APP_MOTOR_send_get_status(motor_p->id);
					}
					else
					{
						motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;
//						APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_FAIL, motor_p->type, motor_p->index, motor_p->status);
						APP_HP_send_status_alarm(motor_p);
						printf("GXD:%s...%d\r\n", __FILE__, __LINE__);
					}
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_MOTOR_STOP)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				break;
			
			case HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S2:
				if (event == HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS)
				{// �ظ��ɹ�
					APP_MOTOR_send_get_status(motor_p->id);
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ������������
					motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_FAIL, motor_p->type, motor_p->index, motor_p->status);
					APP_HP_send_status_alarm(motor_p);
					printf("GXD:%s...%d\r\n", __FILE__, __LINE__);
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_MOTOR_STOP)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				break;
			
			case  HOT_PRESS_MOTOR_STATUS_MOVE_TRIM_S1:
				if (event == HOT_PRESS_MOTOR_EVENT_MOTOR_STOP)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->fast_update_position_enable = 0;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS)
				{// �ظ��ɹ�
					APP_MOTOR_send_get_status(motor_p->id);
					motor_p->fast_update_position_enable = 0;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ������������
					APP_MOTOR_send_get_status(motor_p->id);
					motor_p->fast_update_position_enable = 0;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_START_TRIM_MOVE)
				{
					motor_p->running_position = motor_p->target_position + motor_p->running_position_offset;
					
					if (motor_p->running_position > motor_p->crt_position)
					{
						if ((motor_p->running_position - motor_p->crt_position) > 500)
						{
							motor_p->running_position = motor_p->crt_position + 500;
						}
						
						offset = motor_p->running_position - motor_p->crt_position;
					}
					else
					{
						if ((motor_p->running_position - motor_p->crt_position) < -500)
						{
							motor_p->running_position = motor_p->crt_position - 500;
						}
						
						offset = motor_p->crt_position - motor_p->running_position;
					}
					
					if (offset < 10)
					{
						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed, motor_p->running_position, 1);
					}
					else
					{
						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed * offset / 10, motor_p->running_position, 1);
					}
					
					motor_p->target_position = motor_p->running_position;
//					else if (offset < 100)
//					{
//						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed * 10, motor_p->running_position, 1);
//					}
//					else
//					{
//						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed * 100, motor_p->running_position, 1);
//					}

					//printf("APP_MOTOR_move_position(%d)--offset = %d\r\n", motor_p->running_position, offset);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_MOVE_ARRIVAL_IND, 5000);

					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_TRIM_S1;
				}
				break;

			case HOT_PRESS_MOTOR_STATUS_MOVE_FOLLOW_S1:
				if (event == HOT_PRESS_MOTOR_EVENT_MOTOR_STOP)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->fast_update_position_enable = 1;
					motor_p->fast_update_position_delay_disable = 10;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else 
				if (event == HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS)
				{// �ظ��ɹ�
					APP_MOTOR_send_get_status(motor_p->id);
					motor_p->fast_update_position_enable = 1;
					motor_p->fast_update_position_delay_disable = 10;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// δ�յ��ظ������������
					APP_MOTOR_send_get_status(motor_p->id);
					motor_p->fast_update_position_enable = 1;
					motor_p->fast_update_position_delay_disable = 10;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_START_FOLLOW_MOVE)
				{
					if (motor_p->running_position > motor_p->crt_position)
					{
						offset = motor_p->running_position - motor_p->crt_position;
					}
					else
					{
						offset = motor_p->crt_position - motor_p->running_position;
					}
					
					if (offset < 10)
					{
						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed, motor_p->running_position, 1);
					}
					else
					{
						APP_MOTOR_move_position(motor_p->id, motor_p->speed_mode_trim_speed * offset / 10, motor_p->running_position, 1);
					}
					motor_p->target_position = motor_p->running_position;
					//printf("F---APP_MOTOR_move_position(%d)--offset = %d\r\n", motor_p->running_position, offset);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_MOVE_ARRIVAL_IND, 5000);
					
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_FOLLOW_S1;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_FAILED_0:		// ���������
			case HOT_PRESS_MOTOR_STATUS_FAILED_1:		// ��ʼ��ʧ��
			case HOT_PRESS_MOTOR_STATUS_FAILED_2:		// ����ʧ��.
			case HOT_PRESS_MOTOR_STATUS_FAILED_3:		// ����ƶ��쳣
				break;
			
			default:
				break;
		}
	}
}

void APP_HP_motor_loop(void)
{
	static unsigned char type = HOT_PRESS_MOTOR_TYPE_VERTICAL;
	static unsigned char index = 0;
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point(type, index);
	if (motor_p != NULL)
	{
		if ((motor_p->enable) && (motor_p->id < 0xFE))
		{
			APP_MOTOR_send_get_status(motor_p->id);
		}
	}
	
	index++;
	if (index >= HOT_PRESS_MOTOR_VERTICAL_MAX_NUM)
	{
		index = 0;
		if (type == HOT_PRESS_MOTOR_TYPE_VERTICAL)
		{
			type = HOT_PRESS_MOTOR_TYPE_VERTICAL_BED;
		}
		else
		{
			type = HOT_PRESS_MOTOR_TYPE_VERTICAL;
		}
	}
}


/*****************************************************************
APP_HP_loop_1ms: ��ѹ�ŵ�APP��ѯ����
******************************************************************/
void APP_HP_loop_1ms(void)
{
	APP_HP_timer_1ms();
	//APP_HP_move_check_deal();
	APP_HP_event_deal();

	// ��ʱ���͵��״̬��ѯ
	if (G_motor_get_position_time_enable)
	{
		G_motor_get_position_time_count++;
		if (G_motor_get_position_time_count > HOT_PRESS_MOTOR_GET_POSITION_TIME)
		{
			G_motor_get_position_time_count = 0;
			//APP_MOTOR_send_get_status(0xFF);
			APP_HP_motor_loop();
		}
	}
	
	
	if (G_motor_delay_startup_time)
	{
		G_motor_delay_startup_time--;
		if (G_motor_delay_startup_time == 0)
		{
			APP_HP_motor_init();
			UI_my_deal_system_startup_complete();
		}
	}
	
}

