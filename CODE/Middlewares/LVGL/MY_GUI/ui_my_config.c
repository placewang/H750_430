#include <stdio.h>
#include "main.h"

#include "ui.h"
#include "ui_my_config.h"
#include "ui_my_set.h"
#include "ui_my_mode1.h"
#include "ui_my_MAIN.h"

#include "app_config.h"
#include "app_balanceRoll_code.h"
#include "app_DischargeSystem.h"

const char *U_shor[] ={"上编码ID配置完成 ","U-ENC ID Config OK"};
const char *D_shor[] ={"下编码ID配置完成 ","D-ENC ID Config OK"};

const char *srt[]={"OK",""};  

const char * txtSingleAxisMode[] ={"单轴模式","SingAxis"};
const char * txtDualAxisMode[]   ={"双轴模式","DualAxis"};
const char * txtSingleDualRoll[] ={"单放双卷模式","SingDualRoll"};

const char * txtUnlock[]         ={"解锁","Unlock"};
const char * txtLowerEncoderCfg[]={"下编码ID配置","L-ENC ID CFG"};
const char * txtUpperEncoderCfg[]={"上编码ID配置","U-ENC ID CFG"};

unsigned char  uiCfg_Language=0;

void GUI_SetDefaultState_configPage(void)
{
    lv_label_set_text(ui_Label4, txtSingleAxisMode[uiCfg_Language]);
    lv_label_set_text(ui_Label9, txtDualAxisMode[uiCfg_Language]);
    lv_label_set_text(ui_Label11,txtSingleDualRoll[uiCfg_Language]);

    lv_label_set_text(ui_Label13,txtLowerEncoderCfg[uiCfg_Language]);
    lv_label_set_text(ui_Label12,txtUpperEncoderCfg[uiCfg_Language]);
    lv_label_set_text(ui_Label6, txtUnlock[uiCfg_Language]);
}


lv_obj_t* IDcgf_mbox_p = NULL;
/**************************************************************************************************/
void UI_IDcgf_event(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED)
	{	
		if (strcmp(lv_msgbox_get_active_btn_text(IDcgf_mbox_p),"清除") == 0)
		{
		}
		lv_msgbox_close(IDcgf_mbox_p);
		IDcgf_mbox_p = NULL;
    }
}


void UI_IDcgf_show(const char* IDlog_str,const char** str)
{
	static const char *btn_str[]={NULL,NULL,""};
    btn_str[0]=str[0];
    btn_str[1]=str[1];
	
    if (IDcgf_mbox_p != NULL)
	{
		lv_msgbox_close(IDcgf_mbox_p);
	}
	
	IDcgf_mbox_p = lv_msgbox_create(lv_scr_act(), "ID_CONFIG", IDlog_str, btn_str, false);
	lv_obj_align(IDcgf_mbox_p, LV_ALIGN_CENTER, 0, 0);
	
	lv_obj_set_style_text_font(IDcgf_mbox_p, &ui_font_CN20, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(IDcgf_mbox_p, lv_color_hex(0xe7400a), LV_STATE_DEFAULT);
	lv_obj_add_event_cb(IDcgf_mbox_p, UI_IDcgf_event, LV_EVENT_ALL, NULL);
	lv_obj_set_style_border_color(IDcgf_mbox_p, lv_color_hex(0x2095F6), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void GUI_Language_mode(lv_event_t * e)
{
	uiCfg_Language=!uiCfg_Language;
    
    APP_AddConfigParameterToFlash();
    NVIC_SystemReset(); // 复位	     
}

//单轴模式设置
void GUI_SetDevSingAxleMod(lv_event_t * e)
{
    Discharge.DownMoveEnable=1;
    Discharge.UpMoveEnable  =0;    
	Discharge.devMode=_SingAxleMod;
    APP_AddConfigParameterToFlash();
    NVIC_SystemReset(); // 复位	    
}

/*
双轴模式
*/
void GUI_SetDevdoubleAxisMod(lv_event_t * e)
{
    Discharge.DownMoveEnable=0;
    Discharge.UpMoveEnable  =0;
	Discharge.devMode=_DoubleAxisMod;    
    APP_AddConfigParameterToFlash();
    NVIC_SystemReset(); // 复位	 	
}

/*
单放双卷模式设置
*/
void GUI_SetSinglAxisDoubleVolumeMode(lv_event_t * e)
{
    Discharge.DownMoveEnable=0;
    Discharge.UpMoveEnable  =0;    
    Discharge.devMode=_SinglAxisDoubleVolumeMode;
    APP_AddConfigParameterToFlash();
    NVIC_SystemReset(); // 复位	
}


void GUI_UPsetEncodeID(lv_event_t * e)
{
    unsigned char IDVal[8]={0};
    IDVal[0]=0x02;
    IDVal[1]=0xff;
    IDVal[2]=0x00;
    IDVal[4]=Encode_UPID; 
    EncodeSendCanData(IDVal,8,MOTORREVCANID); 
    UI_IDcgf_show(U_shor[uiCfg_Language],srt);    
}
void GUI_DWsetEncodeID(lv_event_t * e)
{
    unsigned char IDVal[8]={0};
    IDVal[0]=0x02;
    IDVal[1]=0xff;
    IDVal[2]=0x00;
    IDVal[4]=Encode_BELOWID; 
    EncodeSendCanData(IDVal,8,MOTORREVCANID); 
    UI_IDcgf_show(D_shor[uiCfg_Language],srt); 
}



