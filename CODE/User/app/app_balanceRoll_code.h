#ifndef __BALANCEROLL_H_
#define __BALANCEROLL_H_

#define  EncodeRefreshTime            10
#define  MOTORDATALEN                 8
#define  MOTORREVCANID                0x01  

#define  Encode_UP_CANID              0x102          //上压辊编码器发送ID         
#define  Encode_UPID                  0x02           //上压辊编码器协议ID
#define  Encode_BELOW_CANID           0x101          //下压辊编码器发送ID         
#define  Encode_BELOWID               0x01          //下压辊编码器协议ID


//电机主命令
typedef struct {
	unsigned char ZeroHit;								//撞零
	unsigned char Parameter;							//参数设置一级指令			
	unsigned char Enable;							    //电机使能/失能
	unsigned char MoveWthout;							//位置模式运动
	unsigned char MoveWith;								//位置模式运动完成后返回当前位置	
	unsigned char GetEncodervalue;						//读当前编码器位置	
	unsigned char Read;									//读电机一级指令
	unsigned char Limit;								//运动极限设置
	unsigned char Torque;                               //读转矩
	unsigned char RotatingSpeed;                        //读转速
	unsigned char ClearALarm;                           //清报警
	unsigned char MovezeroRev;                          //归零完成后自动回复
	unsigned char OriginalEncodedVal;                   //读原始编码器值
	unsigned char SeepMode;                             //速度模式	
}MotorCmd;

//电机子命令
typedef struct {
	unsigned short SetEncodervalue;                      //设置编码器值                         
	unsigned short SetDir;	                             //设置电机运转正方向
	unsigned short SetReturnZeroOffset;                  //设置电机撞停的偏移量
	unsigned short SetHitstopPosValue;		             //设置电机撞停后编码器值
	unsigned short SetFindZeroMode; 			         //设置电机回零方式
	unsigned short SetMaxSpeed;						     //设置电机最高转速
	unsigned short SetPosvalue;						     //设置电机位置
	unsigned short SetBaud;							     //设置通讯波特率
	unsigned short SetStartStiffness;                    //设置电机启动刚性
	unsigned short SetStopStiffness;	                 //设置电机停止刚性
	unsigned short SetLimit;                             //设置电机运动极限（大小）
	unsigned short ElectronicGearRatio;                  //设置电机电子齿轮比
    unsigned short VersionSwitch;                        //协议版本切换
}MotorSubCmd;


extern MotorCmd     MCmd;
extern MotorSubCmd	 MSCmd;
        

typedef struct
{
   unsigned char random[4];    
   signed char  IDcfg;  
   signed char  CodeRefreshMark_UP;  //上放料编码刷新标记
   signed char  CodeRefreshMark_DW;  //下放料编码刷新标记
   signed char  EncoderUpDir;        //上电默认方向
   signed char  EncoderDownDir;      //上电默认方向
    
   unsigned short SendRevCnt_U; 
   unsigned short SendRevCnt_D;
   signed short   LastPos_U;         //上一次编码位置
   signed short   LastPos_D;         //上一次编码位置     
   signed short UpReal_TimePos;      //上放料编码器实时位置
   signed short DownReal_TimePos;    //下放料编码器实时位置
   unsigned int ReadIntervalTime;       
    
}Encode;
                              


extern Encode Encode_handle;


void BalanceRollEncodeInit(void);
signed char BalanceRollEncodeValLoadloopTask(const unsigned char);
signed char EncodeSendCanData(unsigned char * dataval,\
							  const unsigned int num,\
							  const unsigned int Dev_num);
void TensionCalibrationValClear(unsigned char id);
signed char app_BalanceRoll_ReadEncodeVal(const unsigned char);
#endif

