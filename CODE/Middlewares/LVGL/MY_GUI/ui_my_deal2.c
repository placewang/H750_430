#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ui_my_deal.h"
#include "app_config.h"
#include "main.h"

void UI_my_deal_show_motor_param(int index)
{
	char str[64];
	struct HOT_PRESS_MOTOR *motor_p;
	
	if (index >= HOT_PRESS_MOTOR_VERTICAL_MAX_NUM)
	{
		return;
	}
	
	motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, index);
	if (motor_p != NULL)
	{
		if (motor_p->enable)
		{
			lv_obj_add_state(ui_UPENABLE, LV_STATE_CHECKED);
		}
		else
		{
			lv_obj_clear_state(ui_UPENABLE, LV_STATE_CHECKED);
		}
		
		sprintf(str, "%d", motor_p->id);
		lv_textarea_set_text(ui_UPID, str);
		
		if (motor_p->move_direction == 0)
		{// 正转
			lv_dropdown_set_selected(ui_UPDIR, 0);
		}
		else
		{// 反转
			lv_dropdown_set_selected(ui_UPDIR, 1);
		}
	}
	
	motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, index);
	if (motor_p != NULL)
	{
		if (motor_p->enable)
		{
			lv_obj_add_state(ui_LOENABLE, LV_STATE_CHECKED);
		}
		else
		{
			lv_obj_clear_state(ui_LOENABLE, LV_STATE_CHECKED);
		}
		
		sprintf(str, "%d", motor_p->id);
		lv_textarea_set_text(ui_LOID, str);
		
		if (motor_p->move_direction == 0)
		{// 正转
			lv_dropdown_set_selected(ui_LODIR, 0);
		}
		else
		{// 反转
			lv_dropdown_set_selected(ui_LODIR, 1);
		}
	}
}

void UI_my_deal_init(void)
{
	char str[64];
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, 0);
	if (motor_p != NULL)
	{
		sprintf(str, "%d", motor_p->move_speed);
		lv_textarea_set_text(ui_MOVESPEED, str);
	}
	
	UI_my_deal_show_motor_param(0);
}


void GUI_CF_motor_select_pd1(lv_event_t * e)
{
	UI_my_deal_show_motor_param(0);
}

void GUI_CF_motor_select_pd2(lv_event_t * e)
{
	UI_my_deal_show_motor_param(1);
}

void GUI_CF_motor_select_pd3(lv_event_t * e)
{
	UI_my_deal_show_motor_param(2);
}

void GUI_CF_motor_select_pd4(lv_event_t * e)
{
	UI_my_deal_show_motor_param(3);
}

void GUI_CF_set_speed(lv_event_t * e)
{
	char *str_p;
	int value;
	int motor_index;
	struct HOT_PRESS_MOTOR *motor_p;
	
	str_p = (char *)lv_textarea_get_text(ui_MOVESPEED);
	value = atoi(str_p);
	
	for (motor_index = 0; motor_index < HOT_PRESS_MOTOR_VERTICAL_MAX_NUM; motor_index++)
	{
		motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, motor_index);
		if (motor_p != NULL)
		{
			motor_p->move_speed = value;
		}
		
		motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, motor_index);
		if (motor_p != NULL)
		{
			motor_p->move_speed = value;
		}
	}
	
	
	APP_CONFIG_save();
}

int UI_deal_get_motor_index(void)
{
	if (lv_obj_has_state(ui_CHP1, LV_STATE_CHECKED) == true)
	{
		return 0;
	}
	
	if (lv_obj_has_state(ui_CHP2, LV_STATE_CHECKED) == true)
	{
		return 1;
	}
	
	if (lv_obj_has_state(ui_CHP3, LV_STATE_CHECKED) == true)
	{
		return 2;
	}
	
	if (lv_obj_has_state(ui_CHP4, LV_STATE_CHECKED) == true)
	{
		return 3;
	}
	
	return 100;
}

void GUI_CF_set_upper_motor_enable(lv_event_t * e)
{
	int motor_index;
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_index = UI_deal_get_motor_index();
	motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, motor_index);
	if (motor_p != NULL)
	{
		if (lv_obj_has_state(ui_UPENABLE, LV_STATE_CHECKED) == true)
		{
			motor_p->enable = 1;
		}
		else
		{
			motor_p->enable = 0;
		}
	}
	
}

void GUI_CF_set_lower_motor_enable(lv_event_t * e)
{
	int motor_index;
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_index = UI_deal_get_motor_index();
	motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, motor_index);
	if (motor_p != NULL)
	{
		if (lv_obj_has_state(ui_LOENABLE, LV_STATE_CHECKED) == true)
		{
			motor_p->enable = 1;
		}
		else
		{
			motor_p->enable = 0;
		}
	}
}

void GUI_CF_set_upper_motor_id(lv_event_t * e)
{
	int motor_index;
	char *str_p;
	int value;
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_index = UI_deal_get_motor_index();
	motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, motor_index);
	if (motor_p != NULL)
	{
		str_p = (char *)lv_textarea_get_text(ui_UPID);
		value = atoi(str_p);
		if (value > 255)
		{
			value = 255;
			lv_textarea_set_text(ui_UPID, "255");
		}
		motor_p->id = value;
	}
}

void GUI_CF_set_lower_motor_id(lv_event_t * e)
{
	int motor_index;
	char *str_p;
	int value;
	struct HOT_PRESS_MOTOR *motor_p;
	
	motor_index = UI_deal_get_motor_index();
	motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, motor_index);
	if (motor_p != NULL)
	{
		str_p = (char *)lv_textarea_get_text(ui_LOID);
		value = atoi(str_p);
		if (value > 255)
		{
			value = 255;
			lv_textarea_set_text(ui_LOID, "255");
		}
		motor_p->id = value;
	}
}

void GUI_CF_set_upper_dir(lv_event_t * e)
{
	int motor_index;
	int opt_num;
	struct HOT_PRESS_MOTOR *motor_p;
	
	printf("GUI_CF_set_upper_dir....\r\n");
	motor_index = UI_deal_get_motor_index();
	motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL, motor_index);
	if (motor_p != NULL)
	{
		opt_num = lv_dropdown_get_selected(ui_UPDIR);
		if (opt_num == 0)
		{
			motor_p->move_direction = 0;
		}
		else
		{
			motor_p->move_direction = 1;
		}
	}
}

void GUI_CF_set_lower_dir(lv_event_t * e)
{
	int motor_index;
	int opt_num;
	struct HOT_PRESS_MOTOR *motor_p;
	
	printf("GUI_CF_set_lower_dir....\r\n");
	motor_index = UI_deal_get_motor_index();
	motor_p = APP_HP_get_motor_point(HOT_PRESS_MOTOR_TYPE_VERTICAL_BED, motor_index);
	if (motor_p != NULL)
	{
		opt_num = lv_dropdown_get_selected(ui_LODIR);
		if (opt_num == 0)
		{
			motor_p->move_direction = 0;
		}
		else
		{
			motor_p->move_direction = 1;
		}
	}
}

static unsigned char G_confir_flag = 0;
void GUI_CF_reboot_system(lv_event_t * e)
{
	G_confir_flag = 1;
	lv_label_set_text(ui_SURETEXT,"Confirm restart");
}

void GYUI_CF_save_config(lv_event_t * e)
{
	G_confir_flag = 2;
	lv_label_set_text(ui_SURETEXT,"确认保存");
}

void GUI_CF_config_confirm(lv_event_t * e)
{
	if (G_confir_flag == 1)
	{
		NVIC_SystemReset(); // 复位
	}
	else if (G_confir_flag == 2)
	{
		APP_CONFIG_save();
	}
}
