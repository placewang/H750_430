#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "bsp_uart.h"
#include "QUEUE.h"

#define TPT487_RE_PIN      GPIO_PIN_10
#define TPT487_DE_PIN      GPIO_PIN_10
#define TPT487_GPIO_PORT   GPIOG

#define UART_RX_BUFFER_SIZE 20
uint8_t uart5_rx_buffer[UART_RX_BUFFER_SIZE];
uint8_t uart5_tx_buffer[UART_RX_BUFFER_SIZE];





//队列数据定义
#define UART5_RxQueue_SIZE  20

GENERIC_QUEUE(Uart5Queue,uint8_t ,UART5_RxQueue_SIZE)

Uart5Queue_t  Uart5Queue;


#define UART4_RX_BUF_NUM	16
#define UART4_RX_BUF_LOOP	0x0F
unsigned char G_uart4_recv_data[4];
unsigned char G_uart4_send_data[4];
unsigned char UART4_rx_buf[UART4_RX_BUF_NUM];
unsigned char UART4_rx_wr;
unsigned char UART4_rx_rd;

// 打印重定向

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout; 
/****fputc: 重定义fputc函数*****/
int fputc(int ch, FILE *f)
{      
	HAL_UART_Transmit(&huart4, (unsigned char *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}


volatile uint16_t uart5_rx_len;

// 
HAL_StatusTypeDef UART5_Send_DMA(uint8_t *data, uint16_t length)
{
    if(length > UART_RX_BUFFER_SIZE||length==0) return HAL_ERROR;
    
    // 等待前一次发送完成
    while(HAL_UART_GetState(&huart5) == HAL_UART_STATE_BUSY_TX); 
    HAL_GPIO_WritePin(TPT487_GPIO_PORT, TPT487_RE_PIN, GPIO_PIN_SET); 
    // 复制数据到发送缓冲区
    memcpy(uart5_tx_buffer, data, length);
    
    //发送
    HAL_UART_Transmit(&huart5, uart5_tx_buffer, length,500);
    while(HAL_UART_GetState(&huart5) == HAL_UART_STATE_BUSY_TX);
    HAL_GPIO_WritePin(TPT487_GPIO_PORT, TPT487_RE_PIN, GPIO_PIN_RESET); 
}
//
void UART5_IRQHandler(void)
{  
   uint8_t data[256]={0}; 
    uint32_t error = huart5.ErrorCode;
    //溢出错误   
    if(error & HAL_UART_ERROR_ORE)
    {
      __HAL_UART_CLEAR_FLAG(&huart5, UART_CLEAR_OREF);
    }    
    // 空闲中断处理
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart5);
        //HAL_UART_Receive(&huart5,data,huart5.RxXferSize-huart5.RxXferCount,500);
        printf("IDLE:%d_%d_%s\n",huart5.RxXferSize,huart5.RxXferCount,uart5_rx_buffer);
      
        UART5_Send_DMA(uart5_rx_buffer,huart5.RxXferSize-huart5.RxXferCount);
     
    }
    HAL_UART_IRQHandler(&huart5);
}
//
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    
    if(huart->Instance == UART5)
    {   huart5.RxXferCount=UART_RX_BUFFER_SIZE;
        HAL_UART_Receive_IT(&huart5, uart5_rx_buffer, UART_RX_BUFFER_SIZE);
        // 记录溢出错误
        uint32_t error = huart->ErrorCode;
        if(error & HAL_UART_ERROR_ORE)
        {
            __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF);
        }  
        printf("Rxcall:%s\n",uart5_rx_buffer);
        // 重新启动接收中断
    }
}
//错误处理增强
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == UART5)
  {
    uint32_t error = huart->ErrorCode;
    
    if(error & HAL_UART_ERROR_ORE)
    {
      __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF);
      // 记录溢出错误
    }
    if(error & HAL_UART_ERROR_FE)
    {
      __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_FEF);
      // 帧错误处理
    }
    printf("UART5_Errr:%d\n",error);
//    // 自动恢复
//    HAL_UART_DeInit(huart);
//    MX_UART5_Init();
  }
}


//函数初始化
void BSP_UART_init(void)
{
    //	HAL_UART_Receive_IT(&huart4, G_uart//4_recv_data, 1);
    Uart5Queue_init(&Uart5Queue);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
    HAL_NVIC_SetPriority(UART5_IRQn,0,0);
    HAL_UART_Receive_IT(&huart5,uart5_rx_buffer,UART_RX_BUFFER_SIZE); // 单字节接收      
    __HAL_UART_ENABLE_IT(&huart5,UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&huart5,UART_FLAG_IDLE);
   
    HAL_GPIO_WritePin(TPT487_GPIO_PORT, TPT487_RE_PIN, GPIO_PIN_RESET);  
}
/*****************************************************************
BSP_UART4_read_byte: 从UART4的接收缓冲区获取数据
	data_p：获取的数据保存的地址
返回值：0：成功；1：失败
******************************************************************/
unsigned char BSP_UART4_read_byte(unsigned char *data_p)
{
	if (data_p == NULL)
	{
		return 1;
	}
		
	if (UART4_rx_wr != UART4_rx_rd)
	{
		*data_p = UART4_rx_buf[UART4_rx_rd];
		UART4_rx_rd++;
		UART4_rx_rd &= UART4_RX_BUF_LOOP;
		return 0;
	}
	
	return 1;
}

/*****************************************************************
BSP_UART5_read_msg: 从UART5的接收缓冲区获取数据
	data_p：获取的数据保存的地址
返回值：0：成功；1：失败
******************************************************************/
unsigned char BSP_UART4_send_byte(unsigned char data)
{
	G_uart4_send_data[0] = data;
	HAL_UART_Transmit(&huart4, G_uart4_send_data, 1, HAL_MAX_DELAY);
	return 0;
}


void BSP_UART_loop(void)
{
//	if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_RXNE) == SET)
//	{
////         __HAL_UART_CLEAR_FLAG(&huart5,UART_FLAG_RXNE); 
//	   printf("%c\n", huart5.Instance->RDR & 0xFF);
//		
//	
//	}	

}





