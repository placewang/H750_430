#ifndef UI_MY_SET_H_
#define UI_MY_SET_H_
#include "ui.h"
typedef struct 
{
  void (*ui_UpdateStartStopTAB)(char flag);  
  void (*ui_RestoreDisplay)(float* pos,int*speed);  
  void (*ui_RealTimeRefreshShow)(int*,int*,int*,float);
  void (*ui_getParameter)(int*ex_speed,float* ex_pos);
  void (*ui_RefreshExeParameters)(int* pos,int*speed,int*tim);
} ViewInterface;


extern ViewInterface View_set;



 

#endif


