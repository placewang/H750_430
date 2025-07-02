#include <stdio.h>
#include <string.h>
#include "usart.h"
#include "bsp_uart.h"

#define TPT487_RE_PIN      GPIO_PIN_10
#define TPT487_DE_PIN      GPIO_PIN_10
#define TPT487_GPIO_PORT   GPIOG

#define UART_RX_BUF_SIZE 256
uint8_t uart5_rx_buf1[UART_RX_BUF_SIZE];
uint8_t uart5_rx_buf2[UART_RX_BUF_SIZE];
typedef struct
{
  unsigned char *current_buf;  // ��ǰ��Ծ������  
  int     rxLen;   
}Uart_5;

Uart_5        uart5_Rx={.current_buf=uart5_rx_buf1};
Uart5Queue_t  Uart5Queue;



#define UART4_RX_BUF_NUM	16
#define UART4_RX_BUF_LOOP	0x0F
unsigned char G_uart4_recv_data[4];
unsigned char G_uart4_send_data[4];
unsigned char UART4_rx_buf[UART4_RX_BUF_NUM];
unsigned char UART4_rx_wr;
unsigned char UART4_rx_rd;

// ��ӡ�ض���

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout; 
/****fputc: �ض���fputc����*****/
int fputc(int ch, FILE *f)
{      
	HAL_UART_Transmit(&huart4, (unsigned char *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}


volatile uint16_t uart5_rx_len;

// 
HAL_StatusTypeDef UART5_SendData(uint8_t *data, uint16_t length)
{
    if(length==0) return HAL_ERROR;
    
    // �ȴ�ǰһ�η������
    while(HAL_UART_GetState(&huart5) == HAL_UART_STATE_BUSY_TX); 
    HAL_GPIO_WritePin(TPT487_GPIO_PORT, TPT487_RE_PIN, GPIO_PIN_SET); 
    
    //����
    HAL_UART_Transmit(&huart5,data,length,500);
    while(HAL_UART_GetState(&huart5) == HAL_UART_STATE_BUSY_TX);
    HAL_GPIO_WritePin(TPT487_GPIO_PORT, TPT487_RE_PIN, GPIO_PIN_RESET); 
}
//
void UART5_IRQHandler(void)
{  
    uint32_t error = huart5.ErrorCode;
    //�������   
    if(error & HAL_UART_ERROR_ORE)
    {
      __HAL_UART_CLEAR_FLAG(&huart5, UART_CLEAR_OREF);
    }    
    // �����жϴ���
    if (__HAL_UART_GET_FLAG(&huart5, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart5);
           // �ֶ����ý�����ɻص���֪ͨӦ�ò㣩
        HAL_UART_RxCpltCallback(&huart5);
    }
    HAL_UART_IRQHandler(&huart5);
}
//
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
        
    if(huart->Instance == UART5)
    {   
        uart5_Rx.rxLen= huart5.RxXferSize-huart5.RxXferCount;
        if (uart5_Rx.current_buf== uart5_rx_buf1)
        {
            uart5_Rx.current_buf=uart5_rx_buf2;
           Uart5Queue_enqueue_batch(&Uart5Queue,uart5_rx_buf1,uart5_Rx.rxLen);
        } 
        else if(uart5_Rx.current_buf==uart5_rx_buf2)
        {
            uart5_Rx.current_buf=uart5_rx_buf1;
            Uart5Queue_enqueue_batch(&Uart5Queue,uart5_rx_buf2,uart5_Rx.rxLen);
        }        
        if (huart->RxState != HAL_UART_STATE_READY)
        {
                HAL_UART_AbortReceive(&huart5);
        }
        HAL_UART_Receive_IT(&huart5, uart5_Rx.current_buf, UART_RX_BUF_SIZE);
    }
}
//��������ǿ
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == UART5)
  {
    uint32_t error = huart->ErrorCode;
    
    if(error & HAL_UART_ERROR_ORE)
    {
      __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF);
      // ��¼�������
    }
    if(error & HAL_UART_ERROR_FE)
    {
      __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_FEF);
      // ֡������
    }
    printf("UART5_Errr:%d\n",error);
//    // �Զ��ָ�
//    HAL_UART_DeInit(huart);
//    MX_UART5_Init();
  }
}


//������ʼ��
void BSP_UART_init(void)
{
    //	HAL_UART_Receive_IT(&huart4, G_uart//4_recv_data, 1);
    Uart5Queue_init(&Uart5Queue);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
    HAL_NVIC_SetPriority(UART5_IRQn,0,0);
    __HAL_UART_ENABLE_IT(&huart5,UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&huart5,UART_FLAG_IDLE);    
    HAL_UART_Receive_IT(&huart5, uart5_rx_buf1, UART_RX_BUF_SIZE);   
    HAL_GPIO_WritePin(TPT487_GPIO_PORT, TPT487_RE_PIN, GPIO_PIN_RESET);  
}
/*****************************************************************
BSP_UART4_read_byte: ��UART4�Ľ��ջ�������ȡ����
	data_p����ȡ�����ݱ���ĵ�ַ
����ֵ��0���ɹ���1��ʧ��
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
BSP_UART5_read_msg: ��UART5�Ľ��ջ�������ȡ����
	data_p����ȡ�����ݱ���ĵ�ַ
����ֵ��0���ɹ���1��ʧ��
******************************************************************/
unsigned char BSP_UART4_send_byte(unsigned char data)
{
	G_uart4_send_data[0] = data;
	HAL_UART_Transmit(&huart4, G_uart4_send_data, 1, HAL_MAX_DELAY);
	return 0;
}


void BSP_UART_loop(void)
{
   int len=0;
   uint8_t data[300]={0};
	if (Uart5Queue.count>0)
	{ 
       len=Uart5Queue_dequeue_batch(&Uart5Queue,data,Uart5Queue.count);
	   printf("%d__%s\n",len,data);	
	
	}	

}





