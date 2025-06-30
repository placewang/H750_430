#ifndef __CANOPENSDO__H
#define __CANOPENSDO__H

#define SDOCS_WRITE1BYTE        0x2F 
#define SDOCS_WRITE2BYTE        0x2B
#define SDOCS_WRITE4BYTE        0x23

#define SDOCS_WRITE_SUCCESS_ACK 0x60 
#define SDOCS_WR_FAIL_ACK       0x80

#define SDO_READ_CMD            0x40
#define SDOCS_READ1BYTE_ACK     0x43
#define SDOCS_READ2BYTE_ACK     0x4B 
#define SDOCS_READ4BYTE_ACK     0x4F


/*
CAN-ID���弰NMT������Э��
*/
enum CommObject_ID
{
    Obj_EMCY             =0x80,    //��������
    ObjectID_NMT_Service =0x000,   //�������ID
    ObjectID_NodeOnline  =0x700,   //�ڵ����� 
    ObjectID_SDO_Send    =0x600,   //SDO����
    ObjectID_SDO_Receive =0x580,   //SDO����
};




void CanOpen_BusNodeStart(unsigned char,unsigned char);
void CanOpen_SdoWrite(unsigned char node_id,unsigned char SDOcs,unsigned short index,unsigned char subindex,signed int value);
void CanOpen_SdoRead(unsigned char node_id,unsigned short index,unsigned char subindex);

#endif






