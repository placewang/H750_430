#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "m_set.h"
#include "ui_my_set.h"
#include "p_set.h"

typedef struct {
    Model_set     *model;
    ViewInterface *view;
    unsigned int Timeing;
} Presenter;




Presenter Preter_set={
                        .model=&m_set,
                        .view=&View_set,
                        };

void Pretr_setInit(void)
{
    float f_pos;
    int e_pos,e_speed,time;
    Preter_set.model->RowKnifeMotorIint();
    Preter_set.model->getMovePos(&e_pos);
    Preter_set.model->getMoveSpeed(&e_speed);
    f_pos=(float)e_pos*Preter_set.model->MinimumPrecision;   
    Preter_set.view->ui_RestoreDisplay(&f_pos,&e_speed);
    
}    
void Pretr_setParameterCallback(lv_event_t * e)
{
    float expos;
    int exsped;
    Preter_set.view->ui_getParameter(&exsped,&expos);
    Preter_set.model->setMovePos(&expos);
    Preter_set.model->setMoveSpeed(&exsped);   
} 
//
void Pretr_claerEncdeCallback(lv_event_t * e)
{
	Preter_set.model->setEncdeVale(0);
}
//
void Pretr_JogForwardCallback(lv_event_t * e)
{ 
  int mp_pos;
  Preter_set.model->getMovePos(&mp_pos);  
  Preter_set.model->MotorMovePos(1,mp_pos); 
}
//
void Pretr_JogReverseCallback(lv_event_t * e)
{
  int mn_pos;
  Preter_set.model->getMovePos(&mn_pos);  
  Preter_set.model->MotorMovePos(0,mn_pos); 
}

void Pretr_MoveStartCallback(lv_event_t * e)
{
    static char flag=0;
    flag=!flag;
//    Preter_set.view->ui_UpdateStartStopTAB(flag);
//    Preter_set.model->MotorStart(flag);
//    if(flag)
//        DELTA_Servo_SetTargetSpeed(0x01,500);
//    else 
//        DELTA_Servo_SetTargetSpeed(0x01,0);        
}
//
void Pretr_EncdeCorrelationCallback(lv_event_t * e)
{
	// Your code here
}
/*
Ë¢ÐÂ¼ÆÊ±                        
*/                        
void Pretr_RealTimeing_setPage(void)
{
    Preter_set.Timeing++;
}    
void Pretr_RealTimeRefreshShowloop_setPage(void)
{
    int pos,speed,Encde,time;
    float m_dec;
    time=Preter_set.model->getExeTime();
    Preter_set.model->getRealTimePos(&pos);
    Preter_set.model->getRealTimeSpeed(&speed);
    Preter_set.model->getEncde(&Encde);
    Preter_set.model->getMoveDistance(&m_dec);
    if(Preter_set.Timeing>200)
    {
        Preter_set.view->ui_RefreshExeParameters(0,0,&time);
        Preter_set.view->ui_RealTimeRefreshShow(&pos,&speed,&Encde,m_dec);
        Preter_set.Timeing=0;
    }

}    
                        
                        