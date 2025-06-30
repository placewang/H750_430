#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bsp_lcd.h"

#include "ui_my_deal.h"
#include "ui_my_set.h"
#include "ui_my_mode1.h"
#include "ui_my_MAIN.h"
#include "ui_my_config.h"

#include "app_pid.h"
#include "app_version.h"
#include "app_config.h"


unsigned int G_screen_lock_time;
unsigned int G_screen_backlight_time;


void UI_my_deal_lock_time_feed(void)
{
	//printf("%s\r\n", __FUNCTION__);
	G_screen_lock_time = SCREEN_LOCK_TIME;
	G_screen_backlight_time = SCREEN_BACKLIGHT_CLOSE_TIME;
	BSP_LCD_backlight_enable(1);
}

void UI_my_deal_lock_the_screen(void)
{
    
   lv_obj_set_parent(ui_Panel1,lv_scr_act());
    
   _ui_flag_modify(ui_Panel1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE); 
}


void UI_my_deal_init(void)
{
	char str[16];
    unsigned int version[5];
    
	G_screen_lock_time = SCREEN_LOCK_TIME;
	
	version[0] = APP_VERSION_H;
	version[1] = APP_VERSION_L;
	version[2] = APP_VERSION_M;
	version[3] = APP_VERSION_S;

	sprintf(str, "V.%d.%d.%d.%d",version[0], version[1], version[2], version[3]);
	lv_label_set_text(ui_VERSION, str);
}
//升级重启
void BSP_RTC_set_upgrade_flag(void);
void GUI_ReSet_boot_updata(lv_event_t * e)
{
	BSP_RTC_set_upgrade_flag();
	NVIC_SystemReset(); // 复位	
}
  

/*
上电恢复配置参数及显示状态
*/
void UI_StartRecovery(void)
{
    GUI_RefreshDir_mode1Page();
    GUI_RefreshEncoderDir_setPage();
    GUI_RefreshTensionAndSpeed_setPage();
    
    if(Discharge.devMode==_SingAxleMod)
    {
       GUI_SinglefeedMode_MAINPage(); 
       GUI_SinglefeedMode_SetPage(); 
       GUI_SinglefeedMode_mode1Page();
    }
    else if(Discharge.devMode==_SinglAxisDoubleVolumeMode)
    {
        GUI_SinglAxisDoubleVolumeMode_SetPage();
        GUI_SinglAxisDoubleVolumeMode_MAINPage();
    }
    GUI_SetDefaultState_setPage();
    GUI_SetDefaultState_MAINPage();
    GUI_SetDefaultState_mode1Page();
    GUI_SetDefaultState_configPage();
}



/*
定时任务刷新
*/
void UI_my_deal_loop_1ms(void)
{
	if (G_screen_lock_time)
	{
		G_screen_lock_time--;
		if(G_screen_lock_time == 0)
		{
			UI_my_deal_lock_the_screen();
		}
	}
    GUI_DischargePosShow_1msloop_setPage();
    GUI_StatusLabelShow_loop1ms_MAINPage(&Discharge,&Encode_handle,Discharge.devMode);

}


