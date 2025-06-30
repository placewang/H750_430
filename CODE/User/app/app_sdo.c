#include "main.h"
#include "app_sdo.h"
#include "bsp_dcan.h"

/*
CanOpenSDO���ݷ���
    COBID:����ID(objectID+NodeID)
    Cs:sdo����� 
    index:����
    subindex:������
    Data:����������
*/
signed char CanOpen_Sdo_SendData(unsigned short COB_ID,unsigned char Cs,\
                                 unsigned short index,unsigned char subindex,\
                                 unsigned char * Data)
{
    unsigned char senddata[8]={0};
    /*���������*/
    senddata[0]=Cs;
    senddata[1]=index&0xff;
    senddata[2]=(index>>8)&0xff;
    senddata[3]=subindex;
    /*����*/
    senddata[4]=Data[0];
    senddata[5]=Data[1];    
    senddata[6]=Data[2];    
    senddata[7]=Data[3];    
    //̨���ŷ����������޷���Ӧ��Ҫ�ӳ�
    HAL_Delay(1);       
    /*��Ӳ���ӿڷ���*/
    bsp_Can1_SendPacket(senddata,8,COB_ID);
                    
    return 0;
}
/*
CanOpen���ݷ���
    COBID:����ID(objectID+NodeID)
    len:  ���ݳ���
    Data:����������
*/
signed char CanOpen_SendData(unsigned short COB_ID,unsigned char len,unsigned char * Data)
{

    unsigned char senddata[8]={0};
    for(char i=0;i<len;i++)
    {
        senddata[i]=Data[i];    
    }   
    //̨���ŷ����������޷���Ӧ��Ҫ�ӳ�
    HAL_Delay(1);       
    /*��Ӳ���ӿڷ���*/
    bsp_Can1_SendPacket(senddata,len,COB_ID);
  
    return 0;
}

/*
CanOpen���߽ڵ�����
node_id:�豸�ڵ�ID
*/
void CanOpen_BusNodeStart(unsigned char node_id,unsigned char Cs)
{
		uint8_t busdata[8] ={0} ;
		busdata[0] = Cs;
		busdata[1] = node_id;
        
        CanOpen_SendData(ObjectID_NMT_Service,2,busdata);  
}

/*
CanOpen_SDO д����
    node_id:�豸�ڵ�ID
    wr_num:д�ֽ���(1/2/4) 
    index:����
    subindex:������
    Data:����������
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
CanOpen_SDO ������
    node_id:�豸�ڵ�ID 
    index:����
    subindex:������
*/
void CanOpen_SdoRead(unsigned char node_id,unsigned short index,unsigned char subindex)
{
        unsigned char SDOcs=0;
		unsigned char readdata[4] ={0} ;
		
        SDOcs =SDO_READ_CMD; 
        
        CanOpen_Sdo_SendData(node_id+ObjectID_SDO_Send,SDOcs,index,subindex,readdata);
}








