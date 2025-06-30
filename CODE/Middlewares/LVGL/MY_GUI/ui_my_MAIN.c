#include "main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"
#include "ui_my_config.h"

#include "app_config.h"
#include "app_balanceRoll_code.h"
#include "app_DischargeSystem.h"

#define   SystemCheckTime 5000
#define   SystemStartTime (SystemCheckTime+1000)

uint16_t  LostConnection=10;
int16_t   Negativelimit=-50;
uint16_t  codelimit=1500; 

const char * motorErr[]  ={"电机报警_%X ", "Motor_Alarm_%X"};
const char * motor_lin[] ={"电机掉线 ",    "Motor_Offline"}; 
const char * Encode_lin[]={"编码器掉线 ",  "Encoder_Offline "}; 
const char * Encode_cbt[]={"编码器未校准 ","Encoder_Uncalibrated"};

const char * MotorEn[]   ={"电机使能 ","MTR_ENA"};
const char * MotorDs[]   ={"电机释放 ","MTR_DIS"}; 

const char * txtUStop[]  ={"停止上放料 ","UP_STOP"};
const char * txtUStart[] ={"启动上放料 ","UP_START"};  
const char * txtStop[]   ={"停止下放料 ","DN_STOP"};
const char * txtStart[]  ={"启动下放料 ","DN_START"};
const char * txtSingStart[]  ={"启动放料 ","UNRL_START"};
const char * txtSingStop[]   ={"停止放料 ","UNRL_STOP"};

const char * txtUstateTag[]={"上放料故障 ",   "UP_UNRL_FLT"};
const char * txtDstateTag[]={"下放料故障 ",   "DN_UNRL_FLT"};
const char * txtDsingStateTag[]={"放料故障 ", "UNRL_FLT"};

const char * txtDSingunreeling[]={"放料中 ", "UNRL_RUNING"};
const char * txtmotorEnabled[]={"电机已使能 ",  "MTR_Enabled"};
const char * txtUunreeling[]={"上卷放料中 ", "UP_UNRL_RUNING"};
const char * txtDunreeling[]= {"下卷放料中 ","DN_UNRL_RUNING"};
const char * txtmotorReleased[]= {"电机已释放 ","MTR_Released"};

const char * txtMRTsta[]={"电机","MTR"};
const char * txtMTRL_R[]={"卷料","Roll"};
const char * txtENCsta[]={"编码器 ","Encode"};
const char * txtURoll[] ={"上卷料 ","UP-ROL"};
const char * txtDRoll[] ={"下卷料 ","DWN-ROL"};
const char * txtMEDset[]={"模式设定","Mode Set"};
const char * txtMODE_0[]={"主动放料系统(双放料模式) ",\
                          "UnwindSystem(Dual-Unwnd)"  };
const char * txtMODE_1[]={"主动放料系统(单放料模式) ",\
                          "UnwindSystem(Sgl-Unwnd) "};
const char * txtMODE_2[]={"主动放料系统(单放双卷模式) ",\
                          "UnwindSystem(Sgl_DualRoll) "};


                 
void GUI_SetDefaultState_MAINPage(void)
{
   lv_label_set_text(ui_dwonStaTag,txtmotorReleased[uiCfg_Language]); 
   lv_label_set_text(ui_upStaTag,txtmotorReleased[uiCfg_Language]);  
   lv_label_set_text(ui_Labelmode0,txtMODE_0[uiCfg_Language]);       
   lv_label_set_text(ui_Labelmode1,txtMODE_1[uiCfg_Language]);       
   lv_label_set_text(ui_Labelmode2,txtMODE_2[uiCfg_Language]);       
   lv_label_set_text(ui_Label1, txtMEDset[uiCfg_Language]);        
   lv_label_set_text(ui_Label19,txtENCsta[uiCfg_Language]); 
   lv_label_set_text(ui_Label18,txtMRTsta[uiCfg_Language]);
   lv_label_set_text(ui_UpAxisLock,MotorDs[uiCfg_Language]);
   lv_label_set_text(ui_DownAxisLock,MotorDs[uiCfg_Language]);        
   lv_label_set_text(ui_UpAxisLock,MotorEn[uiCfg_Language]);
   lv_label_set_text(ui_DownAxisLock,MotorEn[uiCfg_Language]);   
   lv_label_set_text(ui_textStatUp, txtUStop[uiCfg_Language]);
   lv_label_set_text(ui_textStatUp, txtUStart[uiCfg_Language]);
   lv_label_set_text(ui_Label28,txtURoll[uiCfg_Language]);
   lv_label_set_text(ui_Label20,Discharge.devMode!=_SingAxleMod?txtDRoll[uiCfg_Language]:txtMTRL_R[uiCfg_Language]);
   lv_label_set_text(ui_textStatDwon, Discharge.devMode!=_SingAxleMod?txtStop[uiCfg_Language]:txtSingStop[uiCfg_Language]);
   lv_label_set_text(ui_textStatDwon, Discharge.devMode!=_SingAxleMod?txtStart[uiCfg_Language]:txtSingStart[uiCfg_Language]);         
}

static void GUI_ShowStatusTagUpdate(uint8_t uStaErr,uint8_t DStaErr,uint8_t S_mode)
{
           
  if(uStaErr==1)
  {      
    lv_label_set_text(ui_upStaTag,txtUstateTag[uiCfg_Language]);
    lv_obj_set_style_text_color(ui_upStaTag, lv_color_hex(0xF6394D), LV_PART_MAIN | LV_STATE_DEFAULT);  
  }
  else if(Discharge.UpMoveEnable==1)
  {
    lv_label_set_text(ui_upStaTag,txtUunreeling[uiCfg_Language]); 
    lv_obj_set_style_text_color(ui_upStaTag, lv_color_hex(0x158630), LV_PART_MAIN | LV_STATE_DEFAULT);      
  }
  else if(!strcmp(lv_label_get_text(ui_UpAxisLock),MotorEn[uiCfg_Language]))
  {
    lv_label_set_text(ui_upStaTag,txtmotorReleased[uiCfg_Language]); 
    lv_obj_set_style_text_color(ui_upStaTag, lv_color_hex(0x158630), LV_PART_MAIN | LV_STATE_DEFAULT);      
  }
  else if(!strcmp(lv_label_get_text(ui_UpAxisLock),MotorDs[uiCfg_Language]))
  {
    lv_label_set_text(ui_upStaTag,txtmotorEnabled[uiCfg_Language]); 
    lv_obj_set_style_text_color(ui_upStaTag, lv_color_hex(0x5218FA), LV_PART_MAIN | LV_STATE_DEFAULT);   
  }
  /**/
  if(DStaErr==1)
  {
    if(Discharge.devMode==_SinglAxisDoubleVolumeMode) 
    {
        lv_label_set_text(ui_upStaTag,txtUstateTag[uiCfg_Language]);
        lv_obj_set_style_text_color(ui_upStaTag, lv_color_hex(0xF6394D), LV_PART_MAIN | LV_STATE_DEFAULT);   
    }     
    S_mode!=_SingAxleMod?lv_label_set_text(ui_dwonStaTag,txtDstateTag[uiCfg_Language]) \
                        :lv_label_set_text(ui_dwonStaTag,txtDsingStateTag[uiCfg_Language]); 
    lv_obj_set_style_text_color(ui_dwonStaTag, lv_color_hex(0xF6394D), LV_PART_MAIN | LV_STATE_DEFAULT);   
  }
  else if(Discharge.DownMoveEnable==1)
  {
    S_mode!=_SingAxleMod?lv_label_set_text(ui_dwonStaTag,txtDunreeling[uiCfg_Language]) \
                        :lv_label_set_text(ui_dwonStaTag,txtDSingunreeling[uiCfg_Language]); 
    lv_obj_set_style_text_color(ui_dwonStaTag, lv_color_hex(0x158630), LV_PART_MAIN | LV_STATE_DEFAULT);     
  }
  else if(!strcmp(lv_label_get_text(ui_DownAxisLock),MotorEn[uiCfg_Language]))
  {
    lv_label_set_text(ui_dwonStaTag,txtmotorReleased[uiCfg_Language]); 
    lv_obj_set_style_text_color(ui_dwonStaTag, lv_color_hex(0x158630), LV_PART_MAIN | LV_STATE_DEFAULT);      
  }
  else if(!strcmp(lv_label_get_text(ui_DownAxisLock),MotorDs[uiCfg_Language]))
  {
    lv_label_set_text(ui_dwonStaTag,txtmotorEnabled[uiCfg_Language]); 
    lv_obj_set_style_text_color(ui_dwonStaTag, lv_color_hex(0x5218FA), LV_PART_MAIN | LV_STATE_DEFAULT);   
  }  
    
}


static uint8_t  U_EncodeErrSta=0;    
static uint8_t  D_EncodeErrSta=0; 
static void GUI_EncodeStatusTagUpdate(Encode*tagEcde,uint8_t E_mode)
{  
    /*Dwon encode err*/
    if(tagEcde->DownReal_TimePos<Negativelimit||tagEcde->DownReal_TimePos>codelimit)
    { 
       D_EncodeErrSta=1; 
       lv_label_set_text(ui_dwonsta,Encode_cbt[uiCfg_Language]);
    }   
    else if(tagEcde->SendRevCnt_D>LostConnection)
    {
       D_EncodeErrSta=1;   
       lv_label_set_text(ui_dwonsta,Encode_lin[uiCfg_Language]);         
    }
    else
    {
       D_EncodeErrSta=0; 
    }
    //state update
    if(lv_obj_get_state(ui_CheckboxC2)&LV_STATE_CHECKED)
    {
      if(D_EncodeErrSta==1)
      {  
        lv_obj_clear_state(ui_CheckboxC2, LV_STATE_CHECKED);  
      }            
    }
    else
    {
      if(D_EncodeErrSta==0)
      {  
          BalanceRollEncodeInit();  
          lv_obj_add_state(ui_CheckboxC2, LV_STATE_CHECKED);  
      }     
    }     
    /*Up encode err*/
    if((tagEcde->UpReal_TimePos<Negativelimit||tagEcde->UpReal_TimePos>codelimit))
    {  
       U_EncodeErrSta=1;  
       lv_label_set_text(ui_upsta,Encode_cbt[uiCfg_Language]);
    }
    else if(tagEcde->SendRevCnt_U>LostConnection)
    { 
        U_EncodeErrSta=1;  
        lv_label_set_text(ui_upsta,Encode_lin[uiCfg_Language]);          
    }      
    else
    {
        U_EncodeErrSta=0;
    }//state update    
    if(lv_obj_get_state(ui_CheckboxC1)&LV_STATE_CHECKED)
    {
        if(U_EncodeErrSta==1)
        {     
          lv_obj_clear_state(ui_CheckboxC1, LV_STATE_CHECKED);  
        }        
    }
    else
    {
        if(U_EncodeErrSta==0)
        {  
          BalanceRollEncodeInit();  
          lv_obj_add_state(ui_CheckboxC1, LV_STATE_CHECKED);  
        }     
    }
}




static uint8_t  U_motorErrSta=0;    
static uint8_t  D_motorErrSta=0;    
static void GUI_MotorStatusTagUpdate(RunParameter *sta,uint8_t M_mode)
{
    char str[20];    
  
    /*下电机 Err */
    if(sta->AlarmCode_D!=0)
    {
       sprintf(str,motorErr[uiCfg_Language],--sta->AlarmCode_D);
       lv_label_set_text(ui_dwonsta,str);              
       sta->AlarmCode_D=0;  
       D_motorErrSta=1;        
    }
    else if(sta->RevCnt_D>LostConnection)
    {
       lv_label_set_text(ui_dwonsta,motor_lin[uiCfg_Language]);
       D_motorErrSta=1;           
    }
    else
    {
        D_motorErrSta=0;
    }//state   
    if(lv_obj_get_state(ui_CheckboxM2)&LV_STATE_CHECKED)
    {
        if(D_motorErrSta==1)
        {     
          lv_obj_clear_state(ui_CheckboxM2, LV_STATE_CHECKED);  
        }        
    }
    else
    {
        if(D_motorErrSta==0)
        {  
          lv_obj_add_state(ui_CheckboxM2, LV_STATE_CHECKED);  
        }     
    }
    /*上电机 Err*/    
    if(sta->AlarmCode_U!=0)
    {    
      sprintf(str,motorErr[uiCfg_Language],--sta->AlarmCode_U);
      lv_label_set_text(ui_upsta,str);        
      sta->AlarmCode_U=0; 
      U_motorErrSta=1;        
    }
    else if(sta->RevCnt_U>LostConnection)
    {
       lv_label_set_text(ui_upsta,motor_lin[uiCfg_Language]); 
       lv_obj_clear_state(ui_CheckboxM1, LV_STATE_CHECKED);  
       U_motorErrSta=1;              
    } 
    else
    {
      U_motorErrSta=0;    
    }//state
    if(lv_obj_get_state(ui_CheckboxM1)&LV_STATE_CHECKED)
    {
        if(U_motorErrSta==1)
        {     
          lv_obj_clear_state(ui_CheckboxM1, LV_STATE_CHECKED);  
        }        
    }
    else
    {
        if(U_motorErrSta==0)
        {  
          lv_obj_add_state(ui_CheckboxM1, LV_STATE_CHECKED);  
        }     
    }   
}




/*
单放料模式页面设
*/
void GUI_SinglefeedMode_MAINPage(void)
{
    lv_obj_set_width(ui_Panel2, 294+20);
    lv_obj_set_height(ui_Panel2, 92+20);    
    lv_obj_set_y(ui_Panel2, 4);
    lv_obj_set_x(ui_dwonStaTag, -168);
    lv_obj_set_y(ui_dwonStaTag, 5);
    lv_obj_set_x(ui_Label20, -130);    
    lv_label_set_text(ui_Label20, txtMTRL_R[uiCfg_Language]);
    lv_label_set_text(ui_textStatDwon,txtSingStart[uiCfg_Language]);
    
   _ui_flag_modify(ui_upStaTag, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);   
   _ui_flag_modify(ui_Panel3,   LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); 
   _ui_flag_modify(ui_CheckboxM1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);   
   _ui_flag_modify(ui_CheckboxC1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);  
   _ui_flag_modify(ui_Labelmode0, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);   
   _ui_flag_modify(ui_Labelmode2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);

   _ui_flag_modify(ui_Labelmode1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);    
}


/*
单放双卷模式页面设置
*/
void GUI_SinglAxisDoubleVolumeMode_MAINPage(void)
{
   _ui_flag_modify(ui_CheckboxC1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);  
   _ui_flag_modify(ui_Labelmode0, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD);   
   _ui_flag_modify(ui_Labelmode1, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_ADD); 
   
   _ui_flag_modify(ui_Labelmode2, LV_OBJ_FLAG_HIDDEN, _UI_MODIFY_FLAG_REMOVE);    
}



/*单放双卷上下电机互斥屏蔽*/
static void GUI_StartMutualExclusionShielding(uint8_t axi,uint8_t flag)
{
    //up
    if(axi==0)
    {
        if(flag==1)
        {
           _ui_state_modify(ui_STARTdwon,LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);  
        }
        else
        {
            _ui_state_modify(ui_STARTdwon,LV_STATE_DISABLED, _UI_MODIFY_STATE_REMOVE);  
        }
    }
    //dwon
    if(axi==1)
    {
        if(flag==1)
        {
           _ui_state_modify(ui_STARTup,LV_STATE_DISABLED, _UI_MODIFY_STATE_ADD);  
        }
        else
        {
            _ui_state_modify(ui_STARTup,LV_STATE_DISABLED, _UI_MODIFY_STATE_REMOVE);  
        }    
    }        
}



/*上电自启动*/
static void GUI_PowerOn_AutoStart(uint8_t uStaErr,uint8_t DStaErr,uint8_t pw_mode)
{    
    if(pw_mode!=_SingAxleMod)
    {
        if(Discharge.UpMoveEnable&&!uStaErr)
        {
            lv_obj_add_state(ui_STARTup, LV_STATE_CHECKED);
            lv_event_send(ui_STARTup,LV_EVENT_VALUE_CHANGED,NULL);
            lv_event_send(ui_STARTup,LV_EVENT_CLICKED,NULL);            
        }
        if(Discharge.DownMoveEnable&&!DStaErr)
        {
            lv_obj_add_state(ui_STARTdwon, LV_STATE_CHECKED);
            lv_event_send(ui_STARTdwon,LV_EVENT_VALUE_CHANGED,NULL);
            lv_event_send(ui_STARTdwon,LV_EVENT_CLICKED,NULL);       
        }         
    }
    else
    {
         if(!DStaErr)
        {
            lv_obj_add_state(ui_STARTdwon, LV_STATE_CHECKED);
            lv_event_send(ui_STARTdwon,LV_EVENT_VALUE_CHANGED,NULL);
            lv_event_send(ui_STARTdwon,LV_EVENT_CLICKED,NULL);       
        }     
    }
           
}
/*
下放料使能
*/
uint8_t UpaxisEnb=0;
uint8_t DownaxisEnb=0;
void GUI_setDown_motorEn(lv_event_t * e)
{
    if (DownaxisEnb)
    {
        DownaxisEnb=!DownaxisEnb;    
        Down_ServoAxisLockOrRelease(0);
        lv_label_set_text(ui_DownAxisLock,MotorEn[uiCfg_Language]);        
    }
    else
    {
        DownaxisEnb=!DownaxisEnb;          
        Down_ServoAxisLockOrRelease(1);
        lv_label_set_text(ui_DownAxisLock,MotorDs[uiCfg_Language]);        
    }	
}


void GUI_setUp_motorEn(lv_event_t * e)
{
	// Your code here
    if (UpaxisEnb)
    {
        UpaxisEnb=!UpaxisEnb;         
        lv_label_set_text(ui_UpAxisLock,MotorEn[uiCfg_Language]);
        Up_ServoAxisLockOrRelease(0);    
    }
    else
    {
        UpaxisEnb=!UpaxisEnb;       
        lv_label_set_text(ui_UpAxisLock,MotorDs[uiCfg_Language]);
        Up_ServoAxisLockOrRelease(1);
    }	 
}


/*
上下轴锁定释放标志跟随

*/
static void GUI_setEnableFollow(uint8_t motor,uint8_t enc)
{
    //up 
    if(motor==0)
    {
      lv_label_set_text(ui_upsta,"");              
      Up_ServoAxisLockOrRelease(enc!=0?1:0);        
    }
    else if(motor==1)
    {
      lv_label_set_text(ui_dwonsta,"");
      Down_ServoAxisLockOrRelease(enc!=0?1:0);        
    }
}

/*
指示灯亮灭
*/
void indicatorlight_onORoff(char M_num,char sta)
{
    //up 
    if(M_num==0)
    {
       HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,sta);
    }
    else if(M_num==1)
    {
      /*等改PCB版*/
    }
}
void GUI_CF_startDwon(lv_event_t * e)
{

    if (lv_obj_has_state(ui_STARTdwon,LV_STATE_CHECKED))
    {
        GUI_setEnableFollow(1,1);
        indicatorlight_onORoff(1,1); 
        Discharge.DownMoveEnable=1;
        Discharge.RunFlag|=_Startflag_D;
        lv_label_set_text(ui_textStatDwon, Discharge.devMode!=_SingAxleMod?txtStop[uiCfg_Language]:txtSingStop[uiCfg_Language]);
        if(Discharge.devMode==_SinglAxisDoubleVolumeMode){
            GUI_StartMutualExclusionShielding(1,1);
        }        
    } 
    else
    {
        
        Down_DischargeStop();
        indicatorlight_onORoff(1,0); 
        GUI_setEnableFollow(1,0);
        Discharge.DownMoveEnable=0;
        lv_label_set_text(ui_textStatDwon, Discharge.devMode!=_SingAxleMod?txtStart[uiCfg_Language]:txtSingStart[uiCfg_Language]);  
        if(Discharge.devMode==_SinglAxisDoubleVolumeMode){
            GUI_StartMutualExclusionShielding(1,0);
        }        
    }
    APP_AddConfigParameterToFlash();
}



void GUI_CF_startUp(lv_event_t * e)
{
    if (lv_obj_has_state(ui_STARTup,LV_STATE_CHECKED))
    {
        GUI_setEnableFollow(0,1);
        indicatorlight_onORoff(0,1);        
        Discharge.UpMoveEnable=1;
        Discharge.RunFlag|=_Startflag_U;
        lv_label_set_text(ui_textStatUp, txtUStop[uiCfg_Language]);
        if(Discharge.devMode==_SinglAxisDoubleVolumeMode){
            GUI_StartMutualExclusionShielding(0,1);
        }  
    } 
    else
    {
        Up_DischargeStop();
        indicatorlight_onORoff(0,0);
        GUI_setEnableFollow(0,0);
        Discharge.UpMoveEnable=0;
        lv_label_set_text(ui_textStatUp, txtUStart[uiCfg_Language]);
        if(Discharge.devMode==_SinglAxisDoubleVolumeMode){
            GUI_StartMutualExclusionShielding(0,0);
        }
    }        
    APP_AddConfigParameterToFlash();
}
/*
24V 电磁阀开关
*/
void  SolenoidValveSwitch(void)
{
    if(!HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)&&
       Discharge.UpMoveEnable==0&&Discharge.DownMoveEnable==0)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,1);
    }
    else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)&&
        (Discharge.UpMoveEnable||Discharge.DownMoveEnable)
    )
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,0);
    }
}
//按钮启停
void PhysicalButtonOperations(void)
{
    char DBT =50;
    static uint8_t  u_lock=0,d_lock;  
    if(lv_scr_act()!=ui_MAIN)
        return;
     if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10))
    {
        Discharge.Upbtn_FlrTime=0;
        u_lock=0;
    }
    if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_13))
    {
        Discharge.Downbtn_FlrTime=0;
        d_lock=0;
    }   
    if(!u_lock&&!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_10)&&Discharge.Upbtn_FlrTime>=DBT)
    {
         u_lock=1;
         Discharge.UpMoveEnable?lv_obj_clear_state(ui_STARTup, LV_STATE_CHECKED):
                                lv_obj_add_state(ui_STARTup, LV_STATE_CHECKED);
         lv_event_send(ui_STARTup,LV_EVENT_VALUE_CHANGED,NULL);
         lv_event_send(ui_STARTup,LV_EVENT_CLICKED,NULL);  
    
    }
    if(!d_lock&&!HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_13)&&Discharge.Downbtn_FlrTime>=DBT)
    {
         d_lock=1;
         Discharge.DownMoveEnable?lv_obj_clear_state(ui_STARTdwon, LV_STATE_CHECKED):
                                lv_obj_add_state(ui_STARTdwon, LV_STATE_CHECKED);
         lv_event_send(ui_STARTdwon,LV_EVENT_VALUE_CHANGED,NULL);
         lv_event_send(ui_STARTdwon,LV_EVENT_CLICKED,NULL);   
    
    }    
    
}//
void  GUI_StatusLabelShow_loop1ms_MAINPage(RunParameter *sta,Encode*code,uint8_t mode)
{   
    
    static uint8_t  Stratlock=0;        
    static uint8_t  checkErrflag=0;
    static uint16_t starttime=0;
    static uint16_t checkTtime=0;
    uint16_t Time=500;
    starttime++;    
    checkTtime++;

    if(starttime>SystemCheckTime&&!checkErrflag)
    {       
        checkTtime=Time+1;
        checkErrflag=1;          
    }
    else if(starttime>SystemStartTime&&!Stratlock)
    {
        Stratlock=1;
        GUI_PowerOn_AutoStart(U_EncodeErrSta|U_motorErrSta,D_EncodeErrSta|D_motorErrSta,mode);        
    }
    if(checkErrflag==1)
    {
       PhysicalButtonOperations();  
    }
    if(checkErrflag==1&&checkTtime>Time)
    {
        checkTtime=0;
        SolenoidValveSwitch();

        GUI_EncodeStatusTagUpdate(code,mode);   
        GUI_MotorStatusTagUpdate(sta,mode);
        GUI_ShowStatusTagUpdate(U_EncodeErrSta|U_motorErrSta,D_EncodeErrSta|D_motorErrSta,mode);
    }        
}



