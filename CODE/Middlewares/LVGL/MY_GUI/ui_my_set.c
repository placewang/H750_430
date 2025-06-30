#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"
#include "ui_my_config.h"

#include "app_config.h"
#include "app_balanceRoll_code.h"
#include "app_DischargeSystem.h"

const char * txtEnterUpdate[]    ={"进入升级","Update"};
const char * txtEcnDir_P[]={"编码正向","ENC FWD"};
const char * txtEcnDir_R[]={"编码反向","ENC REV"};
const char * txtEcnZero[]={"编码归零","ENC ZERO"};
const char * txtURLL[]={"上放料 ","UNLOAD UP"};
const char * txtDRLL[]={"下放料 ","UNLOAD DOWN"};
const char * txtTage[]={"浮辊位置\n\n\n点动速度\n\n\n编码位置",\
                        "DancerPos\n\n\nJog Speed\n\n\nENC Pos"};
/*
张力速度参数上电恢复配置
*/
void GUI_RefreshTensionAndSpeed_setPage(void)
{
    char upstr[8]={0};
    char downstr[8]={0};
    //上放料张力位置
    snprintf(upstr,sizeof(upstr),"%d",Discharge.UpTensionPos);
    lv_textarea_set_text(ui_upTensionpos,upstr);
    memset(upstr,0,sizeof(upstr));
    //点动速度
    snprintf(upstr,sizeof(upstr),"%d",Discharge.UpJogSpeed/10);
    lv_textarea_set_text(ui_upjogspeed,upstr);    
    
    //下放料张力位置
    snprintf(downstr,sizeof(downstr),"%d",Discharge.DownTensionPos);
    lv_textarea_set_text(ui_DownTensionpos,downstr);
    memset(downstr,0,sizeof(downstr));
    //点动速度
    snprintf(downstr,sizeof(downstr),"%d",Discharge.DownJogSpeed/10);
    lv_textarea_set_text(ui_Downjogspeed,downstr);      
}
/*
编码方向字体恢复
*/
void GUI_RefreshEncoderDir_setPage(void)
{
        
    
     
}
void GUI_SetDefaultState_setPage(void)
{
    lv_label_set_text(ui_Label15, txtEnterUpdate[uiCfg_Language]);
    lv_label_set_text(ui_Label10,txtDRLL[uiCfg_Language]);
    lv_label_set_text(ui_Label7, txtURLL[uiCfg_Language]);
    lv_label_set_text(ui_Label5, txtTage[uiCfg_Language]);
    lv_label_set_text(ui_DownAxisLock3, txtEcnZero[uiCfg_Language]);   
    lv_label_set_text(ui_DownAxisLock2, txtEcnZero[uiCfg_Language]);
}


/*
双轴模式页面设置
*/
void GUI_DoubleAxisMode_SetPage(void)
{
    
  lv_obj_set_width(ui_Button14, 100);
  lv_obj_set_height(ui_Button14, 50);
  lv_obj_set_x(ui_Button14, -13);
  lv_obj_set_y(ui_Button14, 24);
  _ui_flag_modify(ui_Label10, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE); 
  _ui_flag_modify(ui_upjogspeed, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);    
  _ui_flag_modify(ui_upTensionpos, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);
  _ui_flag_modify(ui_TTT4, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);  
  _ui_flag_modify(ui_Button13, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);    
}
/*
单放料模式页面设置
*/
void GUI_SinglefeedMode_SetPage(void)
{
  _ui_flag_modify(ui_Label7, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);   
  _ui_flag_modify(ui_Label10, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); 
  _ui_flag_modify(ui_upjogspeed, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);    
  _ui_flag_modify(ui_upTensionpos, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);
  _ui_flag_modify(ui_TTT4, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);  
  _ui_flag_modify(ui_Button13, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); 

  //
//  lv_obj_set_width(ui_Button14, 100+10);
//  lv_obj_set_height(ui_Button14, 50+10);
  lv_obj_set_x(ui_Button14, -13+50);
//  lv_obj_set_y(ui_Button14, 24+20);    
}
/*
单放双卷模式页面设置
*/
void GUI_SinglAxisDoubleVolumeMode_SetPage(void)
{
  
   _ui_flag_modify(ui_TTT4, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);    
   _ui_state_modify(ui_upTensionpos, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);   
   _ui_state_modify(ui_Button13, LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);    
}


/*
放料电机张力位置显示
time:刷新时间
count:时间计数值
*/
void GUI_DischargePosShow_1msloop_setPage(void)
{
    char upstr[8]={0};
    char downstr[8]={0};
    static uint8_t time=0;
    time++;
    if(time>(EncodeRefreshTime*5))
    {
        time=0;
        //上放料
        snprintf(upstr,sizeof(upstr),"%d",Encode_handle.UpReal_TimePos);
        lv_label_set_text(ui_TTT4,upstr);
        //下放料
        snprintf(downstr,sizeof(downstr),"%d",Encode_handle.DownReal_TimePos);
        lv_label_set_text(ui_TTT3,downstr);     
    }
}

/*
修改键盘1位置
*/
void GUI_keyPos(lv_event_t * e)
{
    lv_obj_set_x(ui_Keyboard1, -89);
    lv_obj_set_y(ui_Keyboard1, 13);
}
/*
放料配置参数
*/
void GUI_CF_set_UpParameter(lv_event_t * e)
{
    const char * val_char; 
    unsigned  short pos=0,speed=0;
    //修改键盘1位置
    lv_obj_set_x(ui_Keyboard1, 101);
    lv_obj_set_y(ui_Keyboard1, 0);     
    /*********************读取上放料配置参数***************/
	//张力位置
    val_char=lv_textarea_get_text(ui_upTensionpos);
    pos=atoi(val_char);
    if(pos>1200)
        pos=1200;
    Discharge.UpTensionPos=pos;
    //点动速度
    val_char=lv_textarea_get_text(ui_upjogspeed);
    speed=atoi(val_char);
    if(speed>1500)
         speed=1500;   
    Discharge.UpJogSpeed=speed*10;
    /***********************读取下放料配置参数*************/
	//张力位置
    val_char=lv_textarea_get_text(ui_DownTensionpos);
    pos=atoi(val_char);
    if(pos>1200)
        pos=1200;    
    Discharge.DownTensionPos=pos;
    //点动速度
    val_char=lv_textarea_get_text(ui_Downjogspeed);
    speed=atoi(val_char);  
    if(speed>1500)
         speed=1500;     
    Discharge.DownJogSpeed=speed*10;
    
    APP_AddConfigParameterToFlash();
}

//上放料张力反馈校准
void GUI_UP_TensionCalibration(lv_event_t * e)
{
	TensionCalibrationValClear(0);
}
//下放料张力反馈校准
void GUI_Dwon_TensionCalibration(lv_event_t * e)
{
	TensionCalibrationValClear(1);
}
/*
上放料运动使能选择
*/
void GUI_UpMotorSelection(lv_event_t * e)
{

}
/*
下放料运动使能选择
*/
void GUI_DownMotorSelection(lv_event_t * e)
{

}
//下放料张力反馈换向
void GUI_DwonEncoderDirSwitch(lv_event_t * e)
{
//	unsigned char  MotorInitVal[8]={0};    
//    MotorInitVal[0]=MCmd.Parameter;
//    MotorInitVal[1]=Encode_BELOWID;
//    MotorInitVal[2]=MSCmd.SetDir&0xff; 
//    MotorInitVal[3]=MSCmd.SetDir>>8;
//    MotorInitVal[4]=!Encode_handle.EncoderDownDir;
//    EncodeSendCanData(MotorInitVal,MOTORDATALEN,MOTORREVCANID); 
//    Encode_handle.EncoderDownDir=!Encode_handle.EncoderDownDir;
//    APP_AddConfigParameterToFlash();
//    if(Encode_handle.EncoderDownDir)
//    {
//       lv_label_set_text(ui_DownEncode,txtEcnDir_R[uiCfg_Language]);
//    }
//    else
//    {
//       lv_label_set_text(ui_DownEncode,txtEcnDir_P[uiCfg_Language]); 
//    }    
}
//上放料张力反馈换向
void GUI_UpEncoderDirSwitch(lv_event_t * e)
{
//    unsigned char  MotorInitVal[8]={0};
//    MotorInitVal[0]=MCmd.Parameter;
//    MotorInitVal[1]=Encode_UPID;
//    MotorInitVal[2]=MSCmd.SetDir&0xff; 
//    MotorInitVal[3]=MSCmd.SetDir>>8;
//    MotorInitVal[4]=!Encode_handle.EncoderUpDir;
//    EncodeSendCanData(MotorInitVal,MOTORDATALEN,MOTORREVCANID); 
//    Encode_handle.EncoderUpDir=!Encode_handle.EncoderUpDir;
//    APP_AddConfigParameterToFlash(); 
//    if(Encode_handle.EncoderUpDir)
//    {
//        lv_label_set_text(ui_UpEncode,txtEcnDir_R[uiCfg_Language]);
//    }
//    else
//    {
//       lv_label_set_text(ui_UpEncode,txtEcnDir_P[uiCfg_Language]); 
//    }  
}



