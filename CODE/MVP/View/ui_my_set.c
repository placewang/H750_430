#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"
#include "ui_my_set.h"
#include "ui_my_config.h"

#include "app_config.h"
#include "app_balanceRoll_code.h"
#include "app_DischargeSystem.h"

const char * txtEnterUpdate[]    ={"进入升级","Update"};
const char * txtEcnDir_P[]={"编码正向","ENC FWD"};
const char * txtEcnDir_R[]={"编码反向","ENC REV"};
const char * txtEcnZero[]={"编码归零","ENC ZERO"};

const char * txtTage[]={"浮辊位置\n\n\n点动速度\n\n\n编码位置",\
                        "DancerPos\n\n\nJog Speed\n\n\nENC Pos"};

                        
/*
更新启停标签                        
*/
static void ui_UpdateStartStopTAB(char flag)
{
    if(flag)
    {
      lv_label_set_text(ui_Label17,"停止");  
    }
    else
    {
      lv_label_set_text(ui_Label17,"启动");  
    }
}
                        
/*
参数上电恢复配置
*/
static void ui_RestoreDisplay(float* pos,int*speed)
{
    char upstr[8]={0};
    char downstr[8]={0};       
    //下放料张力位置
    snprintf(downstr,sizeof(downstr),"%.1f",*pos);
    lv_textarea_set_text(ui_DownTensionpos,downstr);
    memset(downstr,0,sizeof(downstr));
    //点动速度
    snprintf(downstr,sizeof(downstr),"%d",(*speed)/10);
    lv_textarea_set_text(ui_Downjogspeed,downstr);      
}
                        
/*
更新实时显示
*/
static void ui_RealTimeRefreshShow(int* pos,int*speed,int*encde,float mdec)
{
    char posStr[8]={0};
    char speedStr[8]={0};
    char encdeStr[8]={0};
    char mDecStr[8]={0};
    snprintf(posStr,sizeof(posStr),"%d",*pos);
    lv_label_set_text(ui_upTensionpos,posStr);  
    
    snprintf(speedStr,sizeof(speedStr),"%d",*speed);
    lv_label_set_text(ui_upjogspeed,speedStr); 
    
    snprintf(encdeStr,sizeof(encdeStr),"%d",*encde);
    lv_label_set_text(ui_TTT3,encdeStr); 
    
    snprintf(mDecStr,sizeof(mDecStr),"%.1f",mdec);
    lv_label_set_text(ui_dec,mDecStr);     
    
}
static void ui_RefreshExeParameters(int* e_time,int*speed,int*tim)
{
    char ex_posStr[8]={0};
    char ex_speedStr[8]={0};
    char ex_encdeStr[8]={0};
    
//    snprintf(ex_posStr,sizeof(ex_posStr),"%d",*pos);
//    lv_textarea_set_text(ui_DownTensionpos,ex_posStr);  
    
//    snprintf(ex_speedStr,sizeof(ex_speedStr),"%d",*speed);
//    lv_textarea_set_text(ui_Downjogspeed,ex_speedStr); 
    
    snprintf(ex_encdeStr,sizeof(ex_encdeStr),"%d",*tim);
    lv_label_set_text(ui_timLab,ex_encdeStr); 
    
}
/*
获取配置参数
*/
static void ui_getParameter(int*ex_speed,float* ex_pos)
{
    const char * val_char; 
    char *ptr[32];
    float pos=0;
    int speed=0;
	//张力位置
    val_char=lv_textarea_get_text(ui_DownTensionpos);
    pos=strtof(val_char,ptr);
//    pos=atoi(val_char); 
    *ex_pos=pos;   
    //速度
    val_char=lv_textarea_get_text(ui_Downjogspeed);
    speed=atoi(val_char);  
    *ex_speed=speed*10;

}


ViewInterface View_set={
    .ui_UpdateStartStopTAB=ui_UpdateStartStopTAB,
    .ui_RestoreDisplay=ui_RestoreDisplay,
                        .ui_RefreshExeParameters=ui_RefreshExeParameters,
                        .ui_RealTimeRefreshShow=ui_RealTimeRefreshShow,
                        .ui_getParameter=ui_getParameter,

                        };                          











