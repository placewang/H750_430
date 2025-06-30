#ifndef _BSP_UART_H_
#define _BSP_UART_H_

void BSP_UART_init(void);
void BSP_UART_loop(void);


unsigned char BSP_UART4_read_byte(unsigned char *data_p);
unsigned char BSP_UART4_send_byte(unsigned char data);


#endif // _BSP_UART_H_
