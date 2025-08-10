#include "main.h"
#include "fdcan.h"
#include <stdio.h>
#include <string.h>
#include "bsp_dcan.h"
#include "app_queue.h"

CAN1_RevQueue_t Can1RevQueue,Can1SendQueue;
CAN2_RevQueue_t Can2RevQueue,Can2SendQueue;
	

void bsp_can_int(void)
{

    CAN1_RevQueue_init(&Can1RevQueue);
    CAN1_RevQueue_init(&Can1SendQueue);
    CAN2_RevQueue_init(&Can2RevQueue);
    CAN2_RevQueue_init(&Can2SendQueue);    
}
/**************************************************************/
void bsp_Can1SendQueueMsg_loop(void)
{   
    CAN_RevTye QCanData={0}; 
    FDCAN_TxHeaderTypeDef   CAN1_TxHeader;

    while(1)
    {
        if(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1)==0)
        {
            return;
        }
        if(!CAN1_RevQueue_dequeue(&Can1SendQueue,&QCanData))
           return;          
        CAN1_TxHeader.Identifier =QCanData.StdId;        
        CAN1_TxHeader.IdType   =QCanData.IDE;              
        CAN1_TxHeader.TxFrameType   =QCanData.RTR;               
        CAN1_TxHeader.DataLength   =QCanData.DLC<<16; 
        CAN1_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; /* 设置错误状态指示 */
        CAN1_TxHeader.BitRateSwitch = FDCAN_BRS_ON;           /* 开启可变波特率 */
        CAN1_TxHeader.FDFormat = FDCAN_FD_CAN;                /* FDCAN格式 */
        CAN1_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;/* 用于发送事件FIFO控制, 不存储 */
        CAN1_TxHeader.MessageMarker = 0;           
        if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &CAN1_TxHeader,QCanData.RxData) != HAL_OK)
        {
             Error_Handler();
        }
    }
}
void bsp_Can1_SendPacket(const unsigned char * dataval,\
                          const unsigned char DLC,\
					      const unsigned int Dev_ID)
{
   CAN_RevTye Qsdata={0}; 
   FDCAN_TxHeaderTypeDef   CAN1_TxHeader;   
   
   if(dataval==NULL)
   {
     while(1)
     {;}
   }       

   Qsdata.StdId=Dev_ID;
   Qsdata.DLC=DLC<=8?DLC:8; 
   Qsdata.IDE=FDCAN_STANDARD_ID;
   Qsdata.RTR=FDCAN_DATA_FRAME;
   for(int i=0;i<Qsdata.DLC;i++)
   {
         Qsdata.RxData[i]=dataval[i];
   }   
   if(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1)==0)
   {
       CAN1_RevQueue_enqueue(&Can1SendQueue,&Qsdata);
   }
   else
   {
        CAN1_TxHeader.Identifier =Qsdata.StdId;        
        CAN1_TxHeader.IdType   =Qsdata.IDE;              
        CAN1_TxHeader.TxFrameType   =Qsdata.RTR;               
        CAN1_TxHeader.DataLength   =Qsdata.DLC<<16; 
        CAN1_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; /* 设置错误状态指示 */
        CAN1_TxHeader.BitRateSwitch = FDCAN_BRS_ON;           /* 开启可变波特率 */
        CAN1_TxHeader.FDFormat = FDCAN_FD_CAN;                /* FDCAN格式 */
        CAN1_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;/* 用于发送事件FIFO控制, 不存储 */
        CAN1_TxHeader.MessageMarker = 0;           
        if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &CAN1_TxHeader,Qsdata.RxData) != HAL_OK)
        {
             Error_Handler();
        }     
    }    
}
void bsp_Can2SendQueueMsg_loop(void)
{
    CAN_RevTye QCanData={0};     
    FDCAN_TxHeaderTypeDef   CAN2_TxHeader;

    while(1)
    {
        if(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2)==0)
        {
            return;
        }
        if(!CAN2_RevQueue_dequeue(&Can2SendQueue,&QCanData)){
            return;
        }
        CAN2_TxHeader.Identifier =QCanData.StdId;        
        CAN2_TxHeader.IdType   =QCanData.IDE;              
        CAN2_TxHeader.TxFrameType   =QCanData.RTR;               
        CAN2_TxHeader.DataLength   =QCanData.DLC<<16; 
        CAN2_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; /* 设置错误状态指示 */
        CAN2_TxHeader.BitRateSwitch = FDCAN_BRS_ON;           /* 开启可变波特率 */
        CAN2_TxHeader.FDFormat = FDCAN_FD_CAN;                /* FDCAN格式 */
        CAN2_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;/* 用于发送事件FIFO控制, 不存储 */
        CAN2_TxHeader.MessageMarker = 0;           
        if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &CAN2_TxHeader,QCanData.RxData) != HAL_OK)
        {
             Error_Handler();
        }
    }
}
void bsp_Can2_SendPacket(const unsigned char * dataval,\
                          const unsigned char DLC,\
					      const unsigned int Dev_ID)
{
//   QUEUE_DATA_T         Qsdata;
   CAN_RevTye Qsdata={0}; 
   FDCAN_TxHeaderTypeDef   CAN2_TxHeader;   
   
   if(dataval==NULL)
   {
     while(1)
     {;}
   }       
   Qsdata.StdId=Dev_ID;
   Qsdata.DLC=DLC<=8?DLC:8; 
   Qsdata.IDE=FDCAN_STANDARD_ID;
   Qsdata.RTR=FDCAN_DATA_FRAME;
   for(int i=0;i<Qsdata.DLC;i++)
   {
         Qsdata.RxData[i]=dataval[i];
   }   
   if(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2)==0)
   {
     CAN2_RevQueue_enqueue(&Can2SendQueue,&Qsdata);
   }
   else
   {
        CAN2_TxHeader.Identifier =Qsdata.StdId;        
        CAN2_TxHeader.IdType     =Qsdata.IDE;              
        CAN2_TxHeader.TxFrameType   =Qsdata.RTR;               
        CAN2_TxHeader.DataLength   =Qsdata.DLC<<16; 
        CAN2_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; /* 设置错误状态指示 */
        CAN2_TxHeader.BitRateSwitch = FDCAN_BRS_ON;           /* 开启可变波特率 */
        CAN2_TxHeader.FDFormat = FDCAN_FD_CAN;                /* FDCAN格式 */
        CAN2_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;/* 用于发送事件FIFO控制, 不存储 */
        CAN2_TxHeader.MessageMarker = 0;           
        if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &CAN2_TxHeader,Qsdata.RxData) != HAL_OK)
        {
             Error_Handler();
        }   
    }    
}
/*
Can接收数据中转到数据队列
*/
void  CanDataTransferToQueue(FDCAN_HandleTypeDef *hfdcan,FDCAN_RxHeaderTypeDef *CanRxType,uint8_t *CanRevData)
{
    CAN_RevTye  Revdata={0};
    
    if(CanRxType==NULL)
        return;
    Revdata.DLC  =CanRxType->DataLength>>16;
    Revdata.StdId=CanRxType->Identifier;   
    
    for(uint8_t i=0;i<Revdata.DLC;i++)    
    {
        Revdata.RxData[i]=CanRevData[i];
    }
    
    //数据入队
 	if(hfdcan == &hfdcan1)
	   CAN1_RevQueue_enqueue(&Can1RevQueue,&Revdata);
    if(hfdcan == &hfdcan2)
       CAN2_RevQueue_enqueue(&Can2RevQueue,&Revdata);
}

/*
	函 数 名: HAL_FDCAN_RxFifo0Callback
	功能说明: CAN中断服务程序-回调函数
	形    参: hfdcan
	返 回 值: 无
*/
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    FDCAN_RxHeaderTypeDef g_Can1RxHeader;
    FDCAN_RxHeaderTypeDef g_Can2RxHeader;
//    uint8_t g_Can1RxData[8];
    uint8_t g_Can2RxData[8];
	if (hfdcan == &hfdcan1)
	{
		if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_WATERMARK) != RESET)
		{
			HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &g_Can1RxHeader, g_Can2RxData);
			/* 激活Rx FIFO0 watermark notification */
			HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_WATERMARK, 0);
			if (g_Can1RxHeader.IdType == FDCAN_STANDARD_ID)
			{
                CanDataTransferToQueue(hfdcan,&g_Can1RxHeader,g_Can2RxData);
			}
		}
	}
	if (hfdcan == &hfdcan2)
	{
		if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_WATERMARK) != RESET)
		{
			HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &g_Can2RxHeader, g_Can2RxData);
			/* 激活Rx FIFO0 watermark notification */
			HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_WATERMARK, 0);
			if (g_Can2RxHeader.IdType == FDCAN_STANDARD_ID)
			{
                CanDataTransferToQueue(hfdcan,&g_Can2RxHeader,g_Can2RxData);
			}
		}
	}    
}

void HAL_FDCAN_ErrorCallback(FDCAN_HandleTypeDef *hfdcan)
{
	if (hfdcan == &hfdcan1)
	{
        printf("can1_eer:%d\n",hfdcan->ErrorCode);
    }
    if (hfdcan == &hfdcan2)
	{
       printf("can2_eer:%d\n",hfdcan->ErrorCode); 
    }

}


