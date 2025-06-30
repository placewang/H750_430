
#ifndef _DISCHARGESYSTEM_ 
#define _DISCHARGESYSTEM_ 

#define  _DoubleAxisMod             0
#define  _SingAxleMod               1
#define  _SinglAxisDoubleVolumeMode 2

#define  _Startflag_U               1
#define  _Startflag_D               2

typedef struct
{
    unsigned char  devMode;        //设备模式   
    unsigned char  mode;           //运行模式    0:双轴放料 1：单轴切换
    unsigned char  RunFlag;        //启停标志    0:停止     1:启动
    unsigned char  UpMoveEnable;   //上放料运动  0:失能     1:使能
    unsigned char  DownMoveEnable; //下放料运动  0:失能     1:使能
    unsigned char  UpDir;          //上放料方向
    unsigned char  DownDir;        //下放料方向 
    unsigned short AlarmCode_U;    //报警号
    unsigned short AlarmCode_D;    //报警号    
    unsigned short RevCnt_U; 
    unsigned short RevCnt_D;     
    signed   short UpSpeed;        //上放料速度
    signed   short DownSpeed;      //下放料速度
    signed int     Down_LastPidspeed; //下放料上一次PID计算速度
    signed int     Up_LastPidspeed;   //上放料上一次PID计算速度
    signed int     Down_LastPos;      //下放料上一次编码位置
    signed int     Up_LastPos;        //上放料上一次编码位置
    unsigned int   PIDRefreshTime;   
    unsigned int   Upbtn_FlrTime;
    unsigned int   Downbtn_FlrTime;     
    unsigned short UpTensionPos;   //上放料张力位置
    unsigned short DownTensionPos; //下放料张力位置    
    unsigned short UpJogSpeed;     //上放料点动速度
    unsigned short DownJogSpeed;   //下放料点动速度  

    signed short UpPerlimit;     //上放料张力上限位置
    signed short UpLowerlimi;    //上放料张力下线位置    
    signed short DownPerlimit;   //下放料张力上限位置
    signed short DownLowerlimi;  //下放料张力下线位置     
    
}RunParameter;



extern RunParameter Discharge;        //放料运行参数


void DischargeRunloopTask(void);
//GUI
void Up_DischargeStop(void);
void Down_DischargeStop(void);
void Up_DischargeJog(unsigned char,unsigned char);
void Down_DischargeJog(unsigned char,unsigned char);
void Up_ServoAxisLockOrRelease(unsigned char enb);
void Down_ServoAxisLockOrRelease(unsigned char enb);


#endif






