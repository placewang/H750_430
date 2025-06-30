#include <string.h>
#include <stdio.h>
#include "gpio.h"

#include "app_top.h"
#include "app_handwheel.h"
#include "bsp_encoder.h"
#include "ui_my_deal.h"

unsigned short G_crt_position;
unsigned char G_gear_position;
unsigned char G_last_gear_position;
unsigned char G_last_gear_count;

#define HANDWHEEL_GEAR_NULL		0x00
#define HANDWHEEL_GEAR_X1		0x01
#define HANDWHEEL_GEAR_X10		0x02
#define HANDWHEEL_GEAR_X100		0x03

unsigned char APP_HWHEEL_get_gear_position(void)
{
	GPIO_PinState st;
	
	st = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
	if (st == 0)
	{
		return HANDWHEEL_GEAR_X100;
	}

	
	st = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);
	if (st == 0)
	{
		return HANDWHEEL_GEAR_X10;
	}

	return HANDWHEEL_GEAR_X1;
}

void APP_HWHEEL_gear_sample_loop_1ms(void)
{
	static unsigned short sample_count = 0;
	unsigned char position;
	
	sample_count++;
	if (sample_count > 10)
	{
		sample_count = 0;
		position = APP_HWHEEL_get_gear_position();
		
		if (G_last_gear_position != position)
		{
			G_last_gear_position = position;
			G_last_gear_count = 5;
		}
		else if (G_last_gear_count > 0)
		{
			G_last_gear_count--;
			if (G_last_gear_count == 0)
			{
				G_gear_position = position;
				UI_my_deal_lock_time_feed();
//				if (position == HANDWHEEL_GEAR_X1)
//				{
//					printf("gear position changed to X1\r\n");
//				}
//				else if (position == HANDWHEEL_GEAR_X10)
//				{
//					printf("gear position changed to X10\r\n");
//				}
//				else if (position == HANDWHEEL_GEAR_X100)
//				{
//					printf("gear position changed to X100\r\n");
//				}
//				else
//				{
//					printf("gear position changed to NULL\r\n");
//				}
			}
		}
	}
}


void APP_HWHEEL_init(void)
{
	G_crt_position = BSP_ENCODER_get_counter();
	G_gear_position = HANDWHEEL_GEAR_NULL;
	G_last_gear_position = HANDWHEEL_GEAR_NULL;
	G_last_gear_count = 0;
}

int APP_HWHEEL_get_position_offset(unsigned short last_position, unsigned short crt_position)
{
	int l_p,c_p,o_p;
	l_p = last_position;
	c_p = crt_position;
	
	o_p  = c_p - l_p;
	
	if (l_p > c_p)
	{
		o_p = l_p - c_p;
		if (o_p > 32768)
		{
			c_p += 65535;
		}
	}
	else
	{
		o_p = c_p - l_p;
		if (o_p > 32768)
		{
			l_p += 65535;
		}
	}
	
	return (c_p - l_p);
}

void APP_HWHEEL_position_loop_1ms(void)
{
	static unsigned short position_count = 0;
	unsigned short read_position;
	int offset;
	
	position_count++;
	if (position_count >= 50)
	{
		position_count = 0;
		read_position = BSP_ENCODER_get_counter();
		offset = APP_HWHEEL_get_position_offset(G_crt_position, read_position);
		//printf("offset = %d\r\n", offset);
		if ((offset > 2) || (offset < -2))
		{
			UI_my_deal_lock_time_feed();
			G_crt_position = read_position;
			if (UI_my_deal_if_scr_is_main())
			{
				if (G_gear_position == HANDWHEEL_GEAR_X1)
				{
					APP_TOP_event_deal(HWH_EVENT_MOVE, UI_my_deal_main_get_hp_flag(), offset);
				}
				else if (G_gear_position == HANDWHEEL_GEAR_X10)
				{
					APP_TOP_event_deal(HWH_EVENT_MOVE, UI_my_deal_main_get_hp_flag(), offset * 10);
				}
				else if (G_gear_position == HANDWHEEL_GEAR_X100)
				{
					APP_TOP_event_deal(HWH_EVENT_MOVE, UI_my_deal_main_get_hp_flag(), offset * 10);
				}
			}
			else if (UI_my_deal_if_scr_is_smain())
			{
				//printf("UI_my_deal_if_scr_is_smain = 1\r\n");
				if (G_gear_position == HANDWHEEL_GEAR_X1)
				{
					APP_TOP_event_deal(HWH_EVENT_SINGLE_MOVE, UI_my_deal_smain_get_hp_flag(), offset);
				}
				else if (G_gear_position == HANDWHEEL_GEAR_X10)
				{
					APP_TOP_event_deal(HWH_EVENT_SINGLE_MOVE, UI_my_deal_smain_get_hp_flag(), offset * 10);
				}
				else if (G_gear_position == HANDWHEEL_GEAR_X100)
				{
					APP_TOP_event_deal(HWH_EVENT_SINGLE_MOVE, UI_my_deal_smain_get_hp_flag(), offset * 10);
				}
			}
		}
		G_crt_position = read_position;
	}
}

void APP_HWHEEL_loop_1ms(void)
{
	APP_HWHEEL_gear_sample_loop_1ms();
	APP_HWHEEL_position_loop_1ms();
}
