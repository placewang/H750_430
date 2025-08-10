
#ifndef _ROWKNIFESYSTEM_ 
#define _ROWKNIFESYSTEM_ 
#define _ReductionRatio  35
#define _MinPrecision 0.002692f
#define _EndeMinPrecision 0.036f

#include "app_Rwkf_pid.h"

// S曲线运动规划器状态
typedef enum {
    MOTOR_STATE_IDLE,
    MOTOR_STATE_ACCEL,
    MOTOR_STATE_CRUISE,
    MOTOR_STATE_DECEL,
    MOTOR_STATE_STOP
} MotorState;
// 主控制器结构体
typedef struct 
{
    // 多组PID参数
    PID_Params high_speed_pid;  // >70%速度
    PID_Params mid_speed_pid;   // 30%-70%速度
    PID_Params low_speed_pid;   // <30%速度
    // 运动参数
    float target_position;
    float current_position;
    float current_velocity;
    uint16_t max_speed;
    
    // 系统状态
    uint8_t enabled;
    uint8_t fault;
    uint16_t EndLast; 
    uint8_t is_small_move;
    uint32_t move_timeout;
    uint32_t start_time;


}MotorController;


typedef struct 
{
    char CabtFlag;
    char MoveFlag;
    char StartFlag;
    char ReturnFlag;
    char MoveDir;
    char LastMoveDir;
    char AlarmCode;
    unsigned short EndLast; 
    unsigned short EndValue;
    unsigned short EndActualPos;  
    unsigned short speed;    
    unsigned short speed_1; 
    unsigned short speed_2;     
    unsigned short Actual_1; 
    unsigned short Actual_2;      
    float    MoveDistance;    
    int RevCnt;
    int MovePos;
    int MovePos_2;
    int RealTimePos;
    int MoveSpeed;
    int RealTimeSpeed;
    int Exetime; 
    int Cabttime;
    unsigned int time;  
   
}RowknifeServo;

extern RowknifeServo Rowknife_S;  
extern MotorController motor_ctrl;
void appRowKnifeMovePos(char cmd ,int m_pos);
void appRowKnifeRealTimeing(void);
void appRowKnifeSystemTask_loop(void);
void appRowKnifeMotorController_Init(MotorController *ctrl);
#endif



///*
//位置模式
//*/
//void PosRun(char cmd,unsigned short pos,unsigned short  speed,unsigned short sid)
//{
//	unsigned char writedata[8]={0};
//    writedata[0]=cmd;
//    writedata[1]=sid;
//    writedata[2]=pos&0xff;
//    writedata[3]=(pos>>8)&0xff;
//    writedata[4]=speed&0xff;
//    writedata[5]=(speed>>8)&0xff;       
//    /*调硬件接口发送*/
//    bsp_Can1_SendPacket(writedata,8,YC_CANSendID);      
//}
//void appRowKnifeStop(unsigned short sid)
//{
//    short speed=0;
//    short pos=0;
// 	unsigned char writedata[8]={0};
//    
//    writedata[0]=0x20;
//    writedata[1]=sid;
//    writedata[2]=pos&0xff;
//    writedata[3]=(pos>>8)&0xff;
//    writedata[4]=speed&0xff;
//    writedata[5]=(speed>>8)&0xff;      
//    /*调硬件接口发送*/
//    bsp_Can1_SendPacket(writedata,8,YC_CANSendID);     
//}

//void appRowKnifeMovePos_cabt(char cmd ,int m_pos)
//{
////  Rowknife_S.EndLast=BSP_getENCODER();
//  PosRun(cmd!=0?0x20:0x21,m_pos,500,DOWN_NODE_ID); 
////  Rowknife_S.MoveDir=cmd;    
////  Rowknife_S.MoveFlag=1;
////  Rowknife_S.Exetime=0; 
//  Rowknife_S.ReturnFlag=0;  
////  Rowknife_S.CabtFlag=0;    
//}


