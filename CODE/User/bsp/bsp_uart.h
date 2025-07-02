#ifndef _BSP_UART_H_
#define _BSP_UART_H_
#include "QUEUE.h"
void BSP_UART_init(void);
void BSP_UART_loop(void);
//队列数据定义
#define UART5_RxQueue_SIZE  300
GENERIC_QUEUE(Uart5Queue,unsigned char ,UART5_RxQueue_SIZE)
extern Uart5Queue_t  Uart5Queue;
unsigned char BSP_UART4_read_byte(unsigned char *data_p);
unsigned char BSP_UART4_send_byte(unsigned char data);


#endif // _BSP_UART_H_
