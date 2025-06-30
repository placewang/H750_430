#include "bsp_dcan.h"
#include "app_queue.h"
#include "app_balanceRoll_code.h"
#include <string.h>

MotorCmd     MCmd={0x01,0x02,0x07,0x0A,0x8A,0x71,0x72,0x12,0x76,0x75,0x08,0x78,0x73,0x0B};
MotorSubCmd	 MSCmd={0x8001,0x8002,0x8003,0x8004,0x8009,0x800B,0x8008,0x800C,0x8040,0x8041, 0x0201,0x8005,0x800e};
Encode Encode_handle;



/*
	Can数据发送接口/若移植变更
	dataval:要发送的数据
	num:    数据大小或者长度
	Dev_num:设备接收CANID  
*/

signed char EncodeSendCanData(unsigned char * dataval,\
							 const unsigned int num,\
							 const unsigned int Dev_num)
{
    if(dataval==NULL||num==0)
		return -1;
    bsp_Can2_SendPacket(dataval,num,Dev_num);
    return 0;
}
/*
使能/失能
MID：电机ID
Ner: 1:上使能
	 0:下使能	
*/
signed char Motor_EnableOrClear(const unsigned short MID,const unsigned char Ner)
{
		unsigned char   MotorInitVal[8]={MCmd.Enable ,MID,Ner,0x00,0x00,0x00,0x00,0x00};
 
		if(Ner==0||Ner==1)
		{
			EncodeSendCanData(MotorInitVal,MOTORDATALEN,MOTORREVCANID);
		}
		else
			return -1;
		return 0;
}

/*
平衡压辊编码器初始化
*/
void BalanceRollEncodeInit(void)
{
//    uint8_t dir[2]={Encode_handle.EncoderUpDir,Encode_handle.EncoderDownDir};
    unsigned char  ID[2]={Encode_UPID,Encode_BELOWID};
	unsigned char  MotorInitVal[8]={0};
    HAL_Delay(1000);
	for(char i=0;i<2;i++)
    {
        memset(MotorInitVal,0,8);
        MotorInitVal[0]=MCmd.Parameter;
        MotorInitVal[1]=ID[i];
//        MotorInitVal[2]=MSCmd.SetDir&0xff; 
//        MotorInitVal[3]=MSCmd.SetDir>>8;
//        MotorInitVal[4]=dir[i];

//        EncodeSendCanData(MotorInitVal,MOTORDATALEN,MOTORREVCANID);   //设置运动正方向（逆时针）
        MotorInitVal[2]=MSCmd.ElectronicGearRatio&0xff;
        MotorInitVal[3]=MSCmd.ElectronicGearRatio>>8;
        MotorInitVal[4]=0x12;
        MotorInitVal[6]=0x01;
        EncodeSendCanData(MotorInitVal,MOTORDATALEN,MOTORREVCANID);   //766_PCB 电子齿轮比 360个脉冲转一圈
        Motor_EnableOrClear(ID[i],0);
    }
}    

/*
设置编码值器
id:协议id编号
encodeval:编码值
*/

signed char BalanceRoll_SetEncodeVal(const uint8_t id,const int32_t encodeval)
{ 
	unsigned char   MotorInitVal[8]={0};
    
    MotorInitVal[0]=MCmd.Parameter;
    MotorInitVal[1]=id;
    MotorInitVal[2]=MSCmd.SetEncodervalue&0xff;
    MotorInitVal[3]=MSCmd.SetEncodervalue>>8;
    
    MotorInitVal[4]=encodeval&0xff;
    MotorInitVal[5]=(encodeval>>8)&0xff;
    MotorInitVal[6]=(encodeval>>16)&0xff;
    MotorInitVal[7]=(encodeval>>24)&0xff;
    
    EncodeSendCanData(MotorInitVal,MOTORDATALEN,MOTORREVCANID); 
    
    return 0;
}

/*
读双轴压辊编码器值
up_id:上压辊协议id编号
below_id:下压辊协议id编号
*/

signed char app_BalanceRoll_ReadEncodeVal(const uint8_t mode)
{
   unsigned char   MotorInitVal[2]={0};
    
   MotorInitVal[0]=MCmd.GetEncodervalue;
    
   if(mode==0)
   {       
      MotorInitVal[1]=Encode_UPID;
      EncodeSendCanData(MotorInitVal,2,MOTORREVCANID); 
      Encode_handle.SendRevCnt_U==0? \
                    Encode_handle.SendRevCnt_U++:
                    Encode_handle.SendRevCnt_U++;

   }  
   if(mode!=0||mode==0)
   {    
      MotorInitVal[1]=Encode_BELOWID;
      EncodeSendCanData(MotorInitVal,2,MOTORREVCANID);
      Encode_handle.SendRevCnt_D==0? \
                    Encode_handle.SendRevCnt_D++:
                    Encode_handle.SendRevCnt_D++;       
   }    
    return 0;
}

/*
张力反馈值清0
id:0:上放料 1:下放料
*/
void TensionCalibrationValClear(uint8_t id)
{

	unsigned char  MotorInitVal[8]={0};
   
    MotorInitVal[0]=MCmd.Parameter;
    MotorInitVal[2]=MSCmd.SetEncodervalue&0xff;
    MotorInitVal[3]=MSCmd.SetEncodervalue>>8;
    MotorInitVal[4]=0x00;
    MotorInitVal[6]=0x00;
    if(id==0)
       MotorInitVal[1]=Encode_UPID; 
    else if(id==1)
       MotorInitVal[1]=Encode_BELOWID;
    
   EncodeSendCanData(MotorInitVal,MOTORDATALEN,MOTORREVCANID);   //设置编码器值为0 
   Motor_EnableOrClear(MotorInitVal[1],0); 
}


/*
双轴压辊编码器值装载任务
upId:上压辊协议id编号
belowId:下压辊协议id编号
*/
     
signed short  UpPos=0;     
signed short  DownPos=0;

signed char BalanceRollEncodeValLoadloopTask(const uint8_t mode)
{   
//    QUEUE_DATA_T Dpack={0};	   
    CAN_RevTye Dpack={0}; 
    
//    if(!DeQueue(&Can2_RevQueue,&Dpack))
//       return 0;
    if(!CAN2_RevQueue_dequeue(&Can2RevQueue,&Dpack))
       return 0;  
    if(Dpack.StdId==Encode_UP_CANID&&Dpack.RxData[0]==MCmd.GetEncodervalue)
    {
        UpPos=*((int32_t *)&Dpack.RxData[4]);
        Encode_handle.SendRevCnt_U=0;
    }

    if(Dpack.StdId==Encode_BELOW_CANID&&Dpack.RxData[0]==MCmd.GetEncodervalue)
    {
        DownPos=*((int32_t *)&Dpack.RxData[4]);
        Encode_handle.SendRevCnt_D=0;
    }
    //双轴放料模式
    if(mode==0)
    {
        Encode_handle.UpReal_TimePos=UpPos>0?UpPos:-UpPos;
        Encode_handle.DownReal_TimePos=DownPos>0?DownPos:-DownPos;
    } 
    else
    {  
        Encode_handle.UpReal_TimePos=DownPos>0?DownPos:-DownPos;
        Encode_handle.DownReal_TimePos=DownPos>0?DownPos:-DownPos;
    } 
    return 0;
}


