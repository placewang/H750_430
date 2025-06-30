#include "main.h"
#include "app_sdo.h"
#include "bsp_dcan.h"

/*
CanOpenSDO数据发送
    COBID:发送ID(objectID+NodeID)
    Cs:sdo命令符 
    index:索引
    subindex:子索引
    Data:待发送数据
*/
signed char CanOpen_Sdo_SendData(unsigned short COB_ID,unsigned char Cs,\
                                 unsigned short index,unsigned char subindex,\
                                 unsigned char * Data)
{
    unsigned char senddata[8]={0};
    /*命令与对象*/
    senddata[0]=Cs;
    senddata[1]=index&0xff;
    senddata[2]=(index>>8)&0xff;
    senddata[3]=subindex;
    /*数据*/
    senddata[4]=Data[0];
    senddata[5]=Data[1];    
    senddata[6]=Data[2];    
    senddata[7]=Data[3];    
    //台达伺服连续发送无法响应需要延迟
    HAL_Delay(1);       
    /*调硬件接口发送*/
    bsp_Can1_SendPacket(senddata,8,COB_ID);
                    
    return 0;
}
/*
CanOpen数据发送
    COBID:发送ID(objectID+NodeID)
    len:  数据长度
    Data:待发送数据
*/
signed char CanOpen_SendData(unsigned short COB_ID,unsigned char len,unsigned char * Data)
{

    unsigned char senddata[8]={0};
    for(char i=0;i<len;i++)
    {
        senddata[i]=Data[i];    
    }   
    //台达伺服连续发送无法响应需要延迟
    HAL_Delay(1);       
    /*调硬件接口发送*/
    bsp_Can1_SendPacket(senddata,len,COB_ID);
  
    return 0;
}

/*
CanOpen总线节点启动
node_id:设备节点ID
*/
void CanOpen_BusNodeStart(unsigned char node_id,unsigned char Cs)
{
		uint8_t busdata[8] ={0} ;
		busdata[0] = Cs;
		busdata[1] = node_id;
        
        CanOpen_SendData(ObjectID_NMT_Service,2,busdata);  
}

/*
CanOpen_SDO 写数据
    node_id:设备节点ID
    wr_num:写字节数(1/2/4) 
    index:索引
    subindex:子索引
    Data:待发送数据
*/

void CanOpen_SdoWrite(unsigned char node_id,unsigned char SDOcs,\
                      unsigned short index,unsigned char subindex,\
                      signed int value)
{

	unsigned char writedata[4]={0};
    
     writedata[0] = value&0xff;
     writedata[1] = (value>>8)&0xff;
     writedata[2] = (value>>16)&0xff;
     writedata[3] = (value>>24)&0xff; 
     
     CanOpen_Sdo_SendData(node_id+ObjectID_SDO_Send,SDOcs,index,subindex,writedata);
}
/*
CanOpen_SDO 读数据
    node_id:设备节点ID 
    index:索引
    subindex:子索引
*/
void CanOpen_SdoRead(unsigned char node_id,unsigned short index,unsigned char subindex)
{
        unsigned char SDOcs=0;
		unsigned char readdata[4] ={0} ;
		
        SDOcs =SDO_READ_CMD; 
        
        CanOpen_Sdo_SendData(node_id+ObjectID_SDO_Send,SDOcs,index,subindex,readdata);
}








