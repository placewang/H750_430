#ifndef _BSP_MX25L_SPI_H_
#define _BSP_MX25L_SPI_H_

#include "spi_flash.h"

#define BSP_SAVE_DATA_BASE_ADDRESS		0x0000	// 第一块
#define PID_SAVE_DATA_BASE_ADDRESS		0x1000	// 第二块

void MX25L_SPI_init(void);


unsigned short MX25L_SPI_read_id(void);

unsigned int MX25L_SPI_read_byte(unsigned int address, int len);
unsigned int MX25L_SPI_write_byte(unsigned int address, unsigned int data, int len);

void MX25L_SPI_pid_data_init(void);
unsigned int MX25L_SPI_pid_data_read(unsigned int index, unsigned int *data);
void MX25L_SPI_pid_data_write(unsigned int index, unsigned int pid, unsigned int data);
void MX25L_SPI_pid_data_save(void);

void MX25L_SPI_print_data(unsigned int address, unsigned int page_num);
#endif

