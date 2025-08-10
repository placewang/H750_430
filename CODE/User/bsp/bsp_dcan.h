#ifndef _BSP_DCAN_H_
#define _BSP_DCAN_H_
#define  CAN1RevLen                     200
#define  CAN1SendLen                    100
#include "QUEUE.h"
typedef struct 
{
  unsigned int  StdId;       
  unsigned int  IDE;      
  unsigned int  RTR;      
  unsigned int  DLC;      
  unsigned char RxData[8];
}CAN_RevTye;

GENERIC_QUEUE(CAN1_RevQueue,CAN_RevTye,CAN1RevLen)
GENERIC_QUEUE(CAN2_RevQueue,CAN_RevTye,CAN1SendLen)

extern CAN1_RevQueue_t Can1RevQueue;
extern CAN2_RevQueue_t Can2RevQueue;

void bsp_can_int(void);
void bsp_Can1SendQueueMsg_loop(void);
void bsp_Can2SendQueueMsg_loop(void);
void bsp_Can1_SendPacket(const unsigned char * dataval,\
                         const unsigned char DLC,\
					     const unsigned int Dev_ID);
void bsp_Can2_SendPacket(const unsigned char * dataval,\
                         const unsigned char DLC,\
                         const unsigned int Dev_ID);    
#endif

