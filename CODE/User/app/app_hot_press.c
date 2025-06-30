#include "app_hot_press.h"
#include "app_motor.h"
//#include "App_top.h"

#include <string.h>
#include <stdio.h>

#include "ui_my_deal.h"

//#define SCAN_USING_STEP_MOTOR		1


// 电机配置和运行状态
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
APP_HP_get_motor_status: 配置当前使用的各类型电机的状态
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
	index：电机序号
返回值：
	0：关闭
	1：运行
	2：掉线
	3：错误
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
APP_HP_set_motor_use_num: 配置当前使用的各类型电机数量
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// 横烫
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// 冷排
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// 冲孔
	num：电机数量
******************************************************************/
void APP_HP_set_motor_use_num(unsigned char motor_type, unsigned char num)
{
	switch (motor_type)
	{
		case HOT_PRESS_MOTOR_TYPE_NULL:					// 无
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL:				// 竖烫（纵烫）
			if (num <= HOT_PRESS_MOTOR_VERTICAL_MAX_NUM)
			{
				G_vertical_use_num = num;
			}
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL_BED:			// 竖烫床（纵烫）
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
APP_HP_set_use_num: 配置当前使用的各类型电机数量
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// 横烫
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// 冷排
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// 冲孔
返回值：当前使用的电机数
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
APP_HP_get_motor_id: 根据类型和索引号，获取电机参数的结构体指针
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// 横烫
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// 冷排
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// 冲孔
	motor_index：电机索引
******************************************************************/
unsigned char APP_HP_get_motor_id(unsigned char motor_type, unsigned char motor_index)
{
	switch (motor_type)
	{
		case HOT_PRESS_MOTOR_TYPE_NULL:					// 无
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL:				// 竖烫（纵烫）
			if (motor_index < G_vertical_use_num)
			{
				return G_vertical_motor[motor_index].id;
			}
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL_BED:			// 竖烫床（纵烫）
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
APP_HP_get_motor_point: 根据类型和索引号，获取电机参数的结构体指针
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// 横烫
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// 冷排
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// 冲孔
	motor_index：电机索引
******************************************************************/
struct HOT_PRESS_MOTOR *APP_HP_get_motor_point(unsigned char motor_type, unsigned char motor_index)
{
	switch (motor_type)
	{
		case HOT_PRESS_MOTOR_TYPE_NULL:					// 无
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL:				// 竖烫（纵烫）
			if (motor_index <= HOT_PRESS_MOTOR_VERTICAL_MAX_NUM)
			{
				return &G_vertical_motor[motor_index];
			}
			break;
		
		case HOT_PRESS_MOTOR_TYPE_VERTICAL_BED:			// 竖烫床（纵烫）
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
APP_HP_get_motor_point_by_id: 根据类型和索引号，获取电机参数的结构体指针
	id：电机ID
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
APP_HP_set_motor_id: 配置电机的ID
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
	motor_index：电机索引
	id：设置的电机ID
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
APP_HP_set_motor_move_direction: 配置电机的运动方向
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
	motor_index：电机索引
	move_dir：运动方向
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
APP_HP_set_motor_ratio_denominator: 配置电机的位置比例值的分母
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// 横烫
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// 冷排
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// 冲孔
	motor_index：电机索引
	denominator：分母值
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
APP_HP_set_motor_ratio_molecule: 配置电机的位置比例值的分子
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// 横烫
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// 冷排
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// 冲孔
	motor_index：电机索引
	molecule：分子值
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
APP_HP_set_motor_trim_speed: 配置电机的微调速度
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// 横烫
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// 冷排
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// 冲孔
	motor_index：电机索引
	speed：速度
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
APP_HP_get_motor_current_position: 获取当前的电机位置
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
	motor_index：电机索引
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
APP_HP_get_motor_current_position_by_id: 获取当前的电机位置
	motor_id：电机ID
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
APP_HP_get_motor_current_position: 获取当前的电机位置
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// 横烫
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// 冷排
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// 冲孔
	motor_index：电机索引
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
APP_HP_get_motor_current_position_by_id: 获取当前的电机位置
	motor_id：电机ID
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
APP_HP_print_motor_status: 打印当前的电机状态
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
APP_HP_add_event: 添加电机处理事件
	motor_p：电机参数结构体指针
	event：事件类型
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
APP_HP_set_expected_reply: 设置期待收到的回复命令，并设置超时时间
	motor_p：电机参数结构体指针
	reply_command：期待回复的命令
	time_count：超时时间
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
APP_HP_set_timer: 设置超时时间
	time_count：超时时间
******************************************************************/
static void APP_HP_set_timer(struct HOT_PRESS_MOTOR *motor_p, unsigned short time_count)
{
	if (motor_p != NULL)
	{
		motor_p->time_count = time_count;
	}
}


/*****************************************************************
APP_HP_motor_init: 电机初始化，所有电机进入初始化流程
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

	APP_MOTOR_send_get_status(0xFF);	// 发送广播的状态请求
	G_motor_get_position_time_enable = 1;
}

/*****************************************************************
APP_HP_send_status_alarm: 发送电机的当前状态告警
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
APP_HP_start_speed_move: 启动速度模式
	motor_type：电机类型
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL			0x01	// 竖烫（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_VERTICAL_BED		0x02	// 竖烫床（纵烫）
		#define HOT_PRESS_MOTOR_TYPE_TRANSVERSE			0x03	// 横烫
		#define HOT_PRESS_MOTOR_TYPE_COLD				0x04	// 冷排
		#define HOT_PRESS_MOTOR_TYPE_PUNCH				0x05	// 冲孔
	motor_index：电机号
	speed：运行速度的方向 1正向， 0，停止， -1反向
返回值：0：正常
		1：参数错误
		2：电机异常
		3：状态错误
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
	{// 状态错误
		APP_HP_send_status_alarm(motor_p);
		return 2;
	}
	
	if (speed_flag == 0)
	{// 速度为0，关闭
		motor_p->running_speed = 0;
		APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_STOP_SPEED_MOVE);
		return 0;
	}
	else if (motor_p->status != HOT_PRESS_MOTOR_STATUS_RUNNING)
	{// 忙
		return 3;
	}

	motor_p->running_speed = motor_p->speed_mode_move_speed * speed_flag;
	APP_HP_add_event(motor_p, HOT_PRESS_MOTOR_EVENT_START_SPEED_MOVE);
	return 0;
}

/*****************************************************************
APP_HP_speed_move_by_id: 通过ID来启动速度模式
	id：电机ID
	speed：运行速度的方向 1正向， 0，停止， -1反向
返回值：0：正常
		1：参数错误
		2：电机异常
		3：状态错误
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
	{// 速度为0，关闭
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
APP_HP_trim_move_by_id: 通过ID来启动微调模式
	id：电机ID
	speed：运行速度的方向 1正向， 0，停止， -1反向
返回值：0：正常
		1：参数错误
		2：电机异常
		3：状态错误
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
	{// 速度为0，关闭
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
APP_HP_position_offset_move: 启动速度模式
	motor_index：电机号
	speed：运行速度
返回值：0：正常
		1：参数错误
		2：电机异常
		3：状态错误
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
APP_HP_position_offset_move: 启动速度模式
	motor_index：电机号
	speed：运行速度
返回值：0：正常
		1：参数错误
		2：电机异常
		3：状态错误
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
APP_HP_position_move_by_id: 启动速度模式
	motor_index：电机号
	speed：运行速度
返回值：0：正常
		1：参数错误
		2：电机异常
		3：状态错误
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
APP_HP_position_trim_by_id: 启动位置微调模式，直接发不带回复的位置移动包
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
APP_HP_position_follow_by_id: 启动位置跟随模式，直接发不带回复的位置移动包
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
APP_HP_set_current_position: 设置当前位置
	motor_type：电机类型
	motor_index：电机号
	posiition：设定的位置
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
APP_HP_status_check: 电机状态检查
返回值：0：状态正常；1：状态错误
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
APP_HP_init: 烫压排刀初始化
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
		motor_p->move_direction = 0;	// 运动正向
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
		motor_p->move_direction = 0;	// 运动正向
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
APP_HP_timer_1ms: 1ms的定时器处理
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
					motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;	// 电机不在位
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
					motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;	// 电机不在位
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
APP_HP_find_motor_by_id: 通过id找到对应的电机结构体指针
	motor_id：电机的ID
返回值：电机结构体指针
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
APP_HP_find_motor_by_id: 通过id找到对应的电机结构体指针
	motor_id：电机的ID
返回值：电机结构体指针
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
APP_HP_msg_deal: 收到电机消息处理
	id：收到的CAN ID
	data_p：收到的数据
	len：数据差昂度
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
				// 信号1
				
				motor_p->motor_signal_status = motor_signal_status;
			}
			
			motor_signal_status = (ack_get_status_p->signal >> 6) & 0x01;
			if (motor_signal_status != motor_p->motor_signal2_status)
			{
				// 信号2
				motor_p->motor_signal2_status = motor_signal_status;
			}
		}
		
		if (motor_p->expected_reply == command)
		{// 收到期待的消息，生成事件
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
			case HOT_PRESS_MOTOR_STATUS_IDLE:			// 上电状态，空
				 break;
			
			case HOT_PRESS_MOTOR_STATUS_INIT_S1:		// 初始化步骤，判断电机是否存在，发送71命令
				if (event == HOT_PRESS_MOTOR_EVENT_REPLY_SUCCESS)
				{// 回复成功，发送下电
					APP_MOTOR_poweroff(motor_p->id);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_ENABLE, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S2;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复，电机不存在
					motor_p->status = HOT_PRESS_MOTOR_STATUS_FAILED_0;
					APP_HP_send_status_alarm(motor_p);
					printf("GXD:%s...%d\r\n", __FILE__, __LINE__);
				}
				break;
			
			case HOT_PRESS_MOTOR_STATUS_INIT_S2:		// 初始化步骤，下电
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复
					APP_MOTOR_set_zero_type(motor_p->id, 1);
					APP_HP_set_timer(motor_p, 100);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_SET_PARAMETER, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S3;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_INIT_S3:		// 初始化步骤，设置归零方式
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复
					APP_MOTOR_set_direction(motor_p->id, motor_p->move_direction, 0);
					APP_HP_set_timer(motor_p, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S4;
				}
				break;

			case HOT_PRESS_MOTOR_STATUS_INIT_S4:		// 初始化步骤，设置方向
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复
					APP_MOTOR_set_code_ratio(motor_p->id, motor_p->denominator, motor_p->molecule);
					APP_HP_set_timer(motor_p, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S5;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_INIT_S5:		// 初始化步骤，设置编码器比例.
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复
					APP_MOTOR_set_zero_report(motor_p->id, 1);
					APP_HP_set_timer(motor_p, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S5_1;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_INIT_S5_1:		// 初始化步骤，设置编码器比例.
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复
					APP_MOTOR_set_code(motor_p->id, 0);
					APP_HP_set_timer(motor_p, 100);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S5_2;
				}
				break;
			
			case HOT_PRESS_MOTOR_STATUS_INIT_S5_2:
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复
					APP_MOTOR_poweron(motor_p->id);
					APP_HP_set_timer(motor_p, 1000);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_INIT_S6;
				}
				break;
				
			case HOT_PRESS_MOTOR_STATUS_INIT_S6:		// 初始化步骤，完成
				if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复
					//APP_MOTOR_poweroff(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				break;
				

			case HOT_PRESS_MOTOR_STATUS_RUNNING:			// 运行中
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
				{// 回复成功，发送下电
					APP_MOTOR_move_speed(motor_p->id, motor_p->running_speed, motor_p->speed_mode_move_torque);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_SPEED_S2;
					//APP_HP_start_move_check(motor_index);	// 启动移动检测
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_STOP_SPEED_MOVE)
				{
					APP_MOTOR_stop(motor_p->id);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复，电机不存在
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
				{// 电机移动异常
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
				{// 回复成功
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
				{// 未收到回复，电机不存在
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
				{// 回复成功
					APP_MOTOR_send_get_status(motor_p->id);
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复，电机不存在
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
				{// 回复成功
					motor_p->set_position = motor_p->running_position;
					APP_MOTOR_move_position(motor_p->id, motor_p->position_mode_speed, motor_p->set_position, 1);
					APP_HP_set_expected_reply(motor_p, CMD_HQ_MOTOR_MOVE_ARRIVAL_IND, 60000);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_MOVE_POSIT_S2;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复，电机不存在
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
				{// 回复成功
					APP_MOTOR_send_get_status(motor_p->id);
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复，电机不存在
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
				{// 回复成功
					APP_MOTOR_send_get_status(motor_p->id);
					motor_p->fast_update_position_enable = 0;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复，电机不存在
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
				{// 回复成功
					APP_MOTOR_send_get_status(motor_p->id);
					motor_p->fast_update_position_enable = 1;
					motor_p->fast_update_position_delay_disable = 10;
//					APP_TOP_add_ind(TOP_EVENT_IND_MOTOR_POSITION_SUCC, motor_p->type, motor_p->index, 0);
					motor_p->status = HOT_PRESS_MOTOR_STATUS_RUNNING;
				}
				else if (event == HOT_PRESS_MOTOR_EVENT_TIMEOUT)
				{// 未收到回复，电机不存在
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
				
			case HOT_PRESS_MOTOR_STATUS_FAILED_0:		// 电机不存在
			case HOT_PRESS_MOTOR_STATUS_FAILED_1:		// 初始化失败
			case HOT_PRESS_MOTOR_STATUS_FAILED_2:		// 找零失败.
			case HOT_PRESS_MOTOR_STATUS_FAILED_3:		// 电机移动异常
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
APP_HP_loop_1ms: 烫压排刀APP轮询函数
******************************************************************/
void APP_HP_loop_1ms(void)
{
	APP_HP_timer_1ms();
	//APP_HP_move_check_deal();
	APP_HP_event_deal();

	// 定时发送电机状态轮询
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

