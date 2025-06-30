#include "main.h"
#include "ui.h"
#include "ui_my_config.h"

#include "app_config.h"
#include "app_DischargeSystem.h"

const char * txtMRTDIRTage[]={"电机方向","MRT DIR"};
const char * txtMRTDir_P[]={"正向","FWD"};
const char * txtMRTDir_R[]={"反向","REV"};
const char * txtUROLL[]={"上放料 ","UNLOAD UP"};
const char * txtDROLL[]={"下放料 ","UNLOAD DOWN"};
const char * txtJOG_P[]={"点动正转","JOG FWD"};
const char * txtJOG_R[]={"点动反转","JOG REV"};  


/*
方向上电恢复配置
*/

void GUI_RefreshDir_mode1Page(void)
{
    if(Discharge.UpDir==0)
    {  
        lv_obj_clear_state(ui_MODE5,LV_STATE_CHECKED);
        lv_label_set_text(ui_Label3,txtMRTDir_P[uiCfg_Language]);
    }
    else
    {
        lv_obj_add_state(ui_MODE5, LV_STATE_CHECKED);
        lv_label_set_text(ui_Label3,txtMRTDir_R[uiCfg_Language]);
    }
    if(Discharge.DownDir==0)
    {
        lv_label_set_text(ui_Label32,txtMRTDir_P[uiCfg_Language]);
        lv_obj_clear_state(ui_MODE7, LV_STATE_CHECKED);
    }
    else  
    {
        lv_label_set_text(ui_Label32,txtMRTDir_R[uiCfg_Language]);
        lv_obj_add_state(ui_MODE7, LV_STATE_CHECKED);
    }    
}

void GUI_SetDefaultState_mode1Page(void)
{    
    lv_label_set_text(ui_Label21,txtJOG_R[uiCfg_Language]);
    lv_label_set_text(ui_Label34,txtJOG_R[uiCfg_Language]);
    lv_label_set_text(ui_Label23,txtJOG_P[uiCfg_Language]);
    lv_label_set_text(ui_Label35,txtJOG_P[uiCfg_Language]);
    lv_label_set_text(ui_Label29,txtMRTDIRTage[uiCfg_Language]);
    lv_label_set_text(ui_Label30,txtDROLL[uiCfg_Language]);
    lv_label_set_text(ui_Label14, txtUROLL[uiCfg_Language]);
}


/*
单轴模式
*/
void GUI_SinglefeedMode_mode1Page(void)
{
    lv_obj_set_width(ui_JOGCCWDW, 100+20);
    lv_obj_set_height(ui_JOGCCWDW, 58+10);
    lv_obj_set_x(ui_JOGCCWDW, -130);

    lv_obj_set_width(ui_JOGCCDW, 100+20);
    lv_obj_set_height(ui_JOGCCDW, 58+10);
    lv_obj_set_x(ui_JOGCCDW, 100);
   
  _ui_flag_modify(ui_Label30, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);   
  _ui_flag_modify(ui_Label14, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); 
  _ui_flag_modify(ui_MODE5, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);    
  _ui_flag_modify(ui_JOGCCUP, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
  _ui_flag_modify(ui_JOGCCWUP, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);     
    
}



/*
上放料 方向设置
*/
void GUI_CF_setDir_Up(lv_event_t * e)
{
   
    if (lv_obj_has_state(ui_MODE5,LV_STATE_CHECKED))
    {
        lv_label_set_text(ui_Label3,txtMRTDir_R[uiCfg_Language]);
        Discharge.UpDir=1;         
    }
    else
    {
        lv_label_set_text(ui_Label3,txtMRTDir_P[uiCfg_Language]);
        Discharge.UpDir=0;
    }
    APP_AddConfigParameterToFlash();
}

/*
下放料 方向设置
*/
void GUI_CF_setDir_down_mode2(lv_event_t * e)
{
    if (lv_obj_has_state(ui_MODE7,LV_STATE_CHECKED))
    {
        lv_label_set_text(ui_Label32,txtMRTDir_R[uiCfg_Language]);
        Discharge.DownDir=1;       
    }
    else
    {
        lv_label_set_text(ui_Label32,txtMRTDir_P[uiCfg_Language]);
        Discharge.DownDir=0;
    } 
    APP_AddConfigParameterToFlash();
}

/*
上放料点动正转启动
*/
void GUI_Upmotor_Jog_CC(lv_event_t * e)
{
    Up_ServoAxisLockOrRelease(1); 
    HAL_Delay(5);
	Up_DischargeJog(0,1);
}
/*
上放料 点动正转停止
*/
void GUI_Upmotor_Jog_CCstop(lv_event_t * e)
{
	Up_DischargeJog(0,0);
    HAL_Delay(5);
    Up_ServoAxisLockOrRelease(0); 
}
/*
上放料 点动反转启动
*/
void GUI_Upmotor_Jog_Cw(lv_event_t * e)
{
    Up_ServoAxisLockOrRelease(1); 
    HAL_Delay(5);
	Up_DischargeJog(1,1);
}
/*
上放料 点动反转停止
*/
void GUI_Upmotor_Jog_CwStop(lv_event_t * e)
{
	Up_DischargeJog(0,0);
    HAL_Delay(5);
    Up_ServoAxisLockOrRelease(0);
}
/*
下放料点动正转启动
*/
void GUI_Downmotor_Jog_CC(lv_event_t * e)
{
    Down_ServoAxisLockOrRelease(1); 
    HAL_Delay(5);
	Down_DischargeJog(0,1);
}
/*
下放料 点动正转停止
*/
void GUI_Downmotor_Jog_CCstop(lv_event_t * e)
{
	Down_DischargeJog(0,0);
    HAL_Delay(5);
    Down_ServoAxisLockOrRelease(0); 
}
/*
下放料 点动反转启动
*/
void GUI_Downmotor_Jog_Cw(lv_event_t * e)
{
    Down_ServoAxisLockOrRelease(1); 
    HAL_Delay(5);
	Down_DischargeJog(1,1);
}
/*
下放料 点动反转停止
*/
void GUI_Downmotor_Jog_Cwstop(lv_event_t * e)
{
	Down_DischargeJog(0,0);
    HAL_Delay(5);
    Down_ServoAxisLockOrRelease(0); 
}








