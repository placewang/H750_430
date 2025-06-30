#include "rtc.h"
#include "bsp_rtc.h"

#include <stdio.h>
#include <string.h>

#define UPGRADE_FLAG	0x5AA5

extern RTC_HandleTypeDef hrtc;

void BSP_RTC_init(void)
{
	
}

void BSP_RTC_upgrade_flag_write(unsigned short flag_data)
{
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, flag_data);
}

unsigned short BSP_RTC_upgrade_flag_read(void)
{
	return HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
}

void BSP_RTC_set_upgrade_flag(void)
{
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, UPGRADE_FLAG);
}

void BSP_RTC_clear_upgrade_flag(void)
{
	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0);
}

int BSP_RTC_if_app_want_upgrade(void)
{
	unsigned short flag;
	
	flag = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1);
	printf("[BOOT] upgrade flag = %04X\r\n", flag);
	if (flag == UPGRADE_FLAG)
	{
		return 1;
	}
	
	return 0;
}



