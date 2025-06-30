#include <string.h>
#include <stdio.h>

#include "app_top.h"
#include "app_hot_press.h"

int G_key_timer_count;

// 主界面排刀选择状态
unsigned char G_hp_select_flag;		// 选择排刀,BIT0-BIT3指示排刀1-4

unsigned char G_top_status;			
unsigned char G_up_select;
unsigned char G_move_index;

void APP_TOP_set_timer(int count)
{
	G_key_timer_count = count;
}

/*****************************************************************
APP_TOP_check_limit: 检查限位，只检查纵烫座的传感器
	motor_type：电机类型
	motor_index：电机号
	move_dir：运动方向  0：正向：1：反向
******************************************************************/
int APP_TOP_if_limit(unsigned char type, unsigned char index, unsigned char move_dir)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	if (move_dir == 0)
	{
		motor_p = APP_HP_get_motor_point(type, index);
		if ((motor_p != NULL) && (motor_p->enable))
		{
			if ((motor_p->f_limit_enable) && (motor_p->motor_signal_status))
			{
				return 1;
			}
		}

		if (index < 3)
		{
			index++;
			motor_p = APP_HP_get_motor_point(type, index);
			if ((motor_p != NULL) && (motor_p->enable))
			{
				if ((motor_p->r_limit_enable) && (motor_p->motor_signal2_status))
				{
					return 1;
				}
			}
		}
	}
	else
	{
		motor_p = APP_HP_get_motor_point(type, index);
		if ((motor_p != NULL) && (motor_p->enable))
		{
			if ((motor_p->r_limit_enable) && (motor_p->motor_signal2_status))
			{
				return 1;
			}
		}
		
		if (index > 0)
		{
			index--;
			motor_p = APP_HP_get_motor_point(type, index);
			if ((motor_p != NULL) && (motor_p->enable))
			{
				if ((motor_p->f_limit_enable) && (motor_p->motor_signal_status))
				{
					return 1;
				}
			}
		}
	}
	
	return 0;
	
}


void APP_TOP_event_deal(unsigned char event, unsigned char flag, int data)
{
	struct HOT_PRESS_MOTOR *motor_p;
	
	if (event == KEY_EVENT_MOVE_STOP)
	{
		G_key_timer_count = 0;
		G_top_status = TOP_STATUS_IDLE;
		return;
	}
	
	switch (G_top_status)
	{
		case TOP_STATUS_IDLE:
			if (event == KEY_EVENT_DHP_MOVE_F)
			{
				G_hp_select_flag = flag;
				APP_TOP_set_timer(1);
				G_top_status = TOP_STATUS_DHP_MOVE_F;
			}
			else if (event == KEY_EVENT_DHP_MOVE_R)
			{
				G_hp_select_flag = flag;
				APP_TOP_set_timer(1);
				G_top_status = TOP_STATUS_DHP_MOVE_R;
			}
			else if (event == KEY_EVENT_DHP_FMOVE_F)
			{
				G_hp_select_flag = flag;
				APP_TOP_set_timer(1);
				G_top_status = TOP_STATUS_DHP_FMOVE_F;
			}
			else if (event == KEY_EVENT_DHP_FMOVE_R)
			{
				G_hp_select_flag = flag;
				APP_TOP_set_timer(1);
				G_top_status = TOP_STATUS_DHP_FMOVE_R;
			}
			else if (event == KEY_EVENT_HP_MOVE_F)
			{
				G_hp_select_flag = flag;
				APP_TOP_set_timer(1);
				G_top_status = TOP_STATUS_HP_MOVE_F;
			}
			else if (event == KEY_EVENT_HP_MOVE_R)
			{
				G_hp_select_flag = flag;
				APP_TOP_set_timer(1);
				G_top_status = TOP_STATUS_HP_MOVE_R;
			}
			else if (event == KEY_EVENT_HP_FMOVE_F)
			{
				G_hp_select_flag = flag;
				APP_TOP_set_timer(1);
				G_top_status = TOP_STATUS_HP_FMOVE_F;
			}
			else if (event == KEY_EVENT_HP_FMOVE_R)
			{
				G_hp_select_flag = flag;
				APP_TOP_set_timer(1);
				G_top_status = TOP_STATUS_HP_FMOVE_R;
			}
			else if (event == HWH_EVENT_MOVE)
			{
				G_hp_select_flag = flag;
				
				if (data > 0)
				{
					if (G_hp_select_flag & 0x01)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0, 0) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0, 0))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x02)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1, 0) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1, 0))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x04)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2, 0) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2, 0))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x08)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3, 0) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3, 0))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
				}
				else
				{
					if (G_hp_select_flag & 0x01)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0, 1) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0, 1))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x02)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1, 1) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1, 1))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x04)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2, 1) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2, 1))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x08)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3, 1) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3, 1))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
				}
				
				if (G_hp_select_flag & 0x01)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				APP_TOP_set_timer(500);
				G_top_status = TOP_STATUS_HWH_MOVE;
			}
			else if (event == HWH_EVENT_SINGLE_MOVE)
			{
				G_hp_select_flag = flag;
				if ((G_hp_select_flag & 0x10) == 0x00)
				{
					if (data > 0)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F, 0))
						{
							break;
						}
					}
					else
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F, 1))
						{
							break;
						}
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				else
				{
					if (data > 0)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F, 0))
						{
							break;
						}
					}
					else
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F, 1))
						{
							break;
						}
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				APP_TOP_set_timer(500);
				G_top_status = TOP_STATUS_HWH_SINGLE_MOVE;
			}
			break;
			
		case TOP_STATUS_DHP_MOVE_F:
			if (event == KEY_EVENT_TIME_OUT)
			{
				if (G_hp_select_flag & 0x01)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0, 0) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1, 0) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2, 0) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3, 0) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x01)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
				}
				
				APP_TOP_set_timer(50);
			}
			break;
		
		case TOP_STATUS_DHP_MOVE_R:
			if (event == KEY_EVENT_TIME_OUT)
			{
				if (G_hp_select_flag & 0x01)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0, 1) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1, 1) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2, 1) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3, 1) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x01)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
				}
				
				APP_TOP_set_timer(50);
			}
			break;
		
		case TOP_STATUS_DHP_FMOVE_F:
			if (event == KEY_EVENT_TIME_OUT)
			{
				if (G_hp_select_flag & 0x01)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0, 0) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1, 0) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2, 0) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3, 0) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x01)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
				}
				
				APP_TOP_set_timer(50);
			}
			break;
		
		case TOP_STATUS_DHP_FMOVE_R:		
			if (event == KEY_EVENT_TIME_OUT)
			{
				if (G_hp_select_flag & 0x01)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0, 1) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1, 1) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2, 1) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3, 1) ||
						APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
				}
				
				if (G_hp_select_flag & 0x01)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
				}
				
				APP_TOP_set_timer(50);
			}
			break;
		
		case TOP_STATUS_HP_MOVE_F:
			if (event == KEY_EVENT_TIME_OUT)
			{
				if ((G_hp_select_flag & 0x10) == 0x00)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}

					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
				}
				else
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed / 10);
					}
				}
				APP_TOP_set_timer(50);
			}
			break;
		
		case TOP_STATUS_HP_MOVE_R:
			if (event == KEY_EVENT_TIME_OUT)
			{
				if ((G_hp_select_flag & 0x10) == 0x00)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
				}
				else
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1) / 10);
					}
				}
				APP_TOP_set_timer(50);
			}
			break;
			
		case TOP_STATUS_HP_FMOVE_F:
			if (event == KEY_EVENT_TIME_OUT)
			{
				if ((G_hp_select_flag & 0x10) == 0x00)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
				}
				else
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F, 0))
					{
						APP_TOP_set_timer(50);
						break;
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed);
					}
				}
				APP_TOP_set_timer(50);
			}
			break;
		
		case TOP_STATUS_HP_FMOVE_R:
			if (event == KEY_EVENT_TIME_OUT)
			{
				if ((G_hp_select_flag & 0x10) == 0x00)
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
				}
				else
				{
					if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F, 1))
					{
						APP_TOP_set_timer(50);
						break;
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, motor_p->move_speed * (-1));
					}
				}
				APP_TOP_set_timer(50);
			}
			break;
		
		case TOP_STATUS_HWH_MOVE:
			if (event == HWH_EVENT_MOVE)
			{
				G_hp_select_flag = flag;
				
				if (data > 0)
				{
					if (G_hp_select_flag & 0x01)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0, 0) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0, 0))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x02)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1, 0) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1, 0))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x04)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2, 0) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2, 0))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x08)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3, 0) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3, 0))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
				}
				else
				{
					if (G_hp_select_flag & 0x01)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0, 1) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0, 1))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x02)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1, 1) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1, 1))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x04)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2, 1) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2, 1))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
					
					if (G_hp_select_flag & 0x08)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3, 1) ||
							APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3, 1))
						{
							APP_TOP_set_timer(50);
							break;
						}
					}
				}
				
				if (G_hp_select_flag & 0x01)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 0);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				
				if (G_hp_select_flag & 0x02)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 1);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				
				if (G_hp_select_flag & 0x04)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 2);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				
				if (G_hp_select_flag & 0x08)
				{
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, 3);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				G_top_status = TOP_STATUS_HWH_MOVE;
				APP_TOP_set_timer(500);
			}
			else if (event == KEY_EVENT_TIME_OUT)
			{
				G_top_status = TOP_STATUS_IDLE;
			}
			break;
		
		case TOP_STATUS_HWH_SINGLE_MOVE:
			if (event == HWH_EVENT_SINGLE_MOVE)
			{
				G_hp_select_flag = flag;
				if ((G_hp_select_flag & 0x10) == 0x00)
				{
					if (data > 0)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F, 0))
						{
							break;
						}
					}
					else
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F, 1))
						{
							break;
						}
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				else
				{
					if (data > 0)
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F, 0))
						{
							break;
						}
					}
					else
					{
						if (APP_TOP_if_limit(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F, 1))
						{
							break;
						}
					}
					
					motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, G_hp_select_flag & 0x0F);
					if ((motor_p != NULL) && (motor_p->enable))
					{
						APP_HP_position_trim_by_id(motor_p->id, data);
					}
				}
				G_top_status = TOP_STATUS_HWH_SINGLE_MOVE;
				APP_TOP_set_timer(500);
			}
			else if (event == KEY_EVENT_TIME_OUT)
			{
				G_top_status = TOP_STATUS_IDLE;
			}
			break;
			
		default:
			break;
	}
}

/*****************************************************************
APP_TOP_init:配置参数导入
******************************************************************/
void APP_TOP_init(void)
{
	G_hp_select_flag = 0;
	G_up_select = 0;
	G_move_index = 0;
	G_top_status = TOP_STATUS_IDLE;
	G_key_timer_count = 0;
}

/*****************************************************************
APP_TOP_loop_1ms:
******************************************************************/
void APP_TOP_loop_1ms(void)
{
	if (G_key_timer_count)
	{
		G_key_timer_count--;
		if (G_key_timer_count == 0)
		{
			APP_TOP_event_deal(KEY_EVENT_TIME_OUT, 0, 0);
		}
	}
}
