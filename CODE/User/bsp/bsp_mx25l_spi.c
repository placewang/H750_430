#include <string.h>
#include <stdio.h>
#include "gpio.h"
#include "spi.h"
#include "stm32h7xx_hal.h"
#include "bsp_mx25l_spi.h"

#include "my_type.h"

extern SPI_HandleTypeDef hspi1;

unsigned char G_tx_data[270];
unsigned char G_rx_data[270];
unsigned char G_sector_buf[4100];
unsigned int G_pid_data_start_address;
/*****************************************************************
MEMORY_Print: 工作指示灯任务，1ms调用一次
	address：起始地址
	line_num：打印的行数量，最小一行，每行16个数据
******************************************************************/
void DATA_Print(unsigned int address, unsigned char *data_p, int data_len)
{
	int line_num;
	unsigned char *point;
	int i, j;
	
	if (data_len < 1)
	{
		return;
	}
	
	line_num = ((data_len - 1) / 16) + 1;
	
	point = (unsigned char *)data_p;
	printf("---------------------------------------------------------------\r\n");
	
	for (j = 0; j < line_num; j++)
	{
		printf("%08X: ", address);
		
		for (i = 0; i < 16; i++)
		{
			printf("%02X ", *point);
			point++;
			address++;
		}
		printf("\r\n");
	}

	printf("---------------------------------------------------------------\r\n");
	
}

/*****************************************************************
MX25L_SPI_read_status: QSPI读取芯片的状态
返回值：芯片状态字节
******************************************************************/
unsigned char MX25L_SPI_read_status(void)
{
	G_tx_data[0] = READ_STATUS_REG_CMD;
	G_tx_data[1] = 0xFF;
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
	
	if (HAL_SPI_TransmitReceive(&hspi1, G_tx_data, G_rx_data, 2, 1000) != HAL_OK)
	{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
		return 0;
	}
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
	
	return G_rx_data[1];
}


/*****************************************************************
MX25L_SPI_enable: SPI芯片写使能
******************************************************************/
void MX25L_SPI_write_enable(void)
{
	int i;
	unsigned char status;
	G_tx_data[0] = WRITE_ENABLE_CMD;
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
	
	if (HAL_SPI_TransmitReceive(&hspi1, G_tx_data, G_rx_data, 1, 1000) != HAL_OK)
	{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
		return;
	}
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
	
	for (i = 0; i < 1000; i++)
	{
		status = MX25L_SPI_read_status();
		if ((status & 0x02) == 0x02)
		{// 设置完成
			return;
		}
		HAL_Delay(1);
	}
}

/*****************************************************************
MX25L_SPI_wait_write_complete: SPI等待写操作完成
******************************************************************/
void MX25L_SPI_wait_write_complete(void)
{
	unsigned char status;
	int i;
	for (i = 0; i < 1000; i++)
	{
		HAL_Delay(1);
		status = MX25L_SPI_read_status();
		if ((status & 0x01) == 0)
		{
			return;
		}
	}
}

/*****************************************************************
MX25L_SPI_read_id: SPI读取芯片的ID
返回值：ID
******************************************************************/
unsigned short MX25L_SPI_read_id(void)
{
	unsigned short id = 0;
	G_tx_data[0] = READ_ID_CMD;
	G_tx_data[1] = 0xFF;
	G_tx_data[2] = 0xFF;
	G_tx_data[3] = 0xFF;
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
	
	if (HAL_SPI_TransmitReceive(&hspi1, G_tx_data, G_rx_data, 4, 1000) != HAL_OK)
	{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
		return 0;
	}
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
	//printf("MX25L_SPI_read_id, data: %02X %02X %02X %02X\r\n", G_rx_data[0], G_rx_data[1], G_rx_data[2], G_rx_data[3]);
	
	id = G_rx_data[1];
	id = id << 8;
	id += G_rx_data[3];
	return id;
}


/*****************************************************************
MX25L_SPI_sector_erase: SPI芯片4K扇擦除，3字节地址信息
	address：地址
返回值：0：成功；非0：失败
******************************************************************/
unsigned int MX25L_SPI_sector_erase(unsigned int address)
{
	MX25L_SPI_write_enable();
    
	G_tx_data[0] = SECTOR_ERASE_CMD;
	G_tx_data[1] = (address >> 16) & 0xFF;
	G_tx_data[2] = (address >> 8) & 0xFF;
	G_tx_data[3] = address & 0xFF;
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
	
	if (HAL_SPI_TransmitReceive(&hspi1, G_tx_data, G_rx_data, 4, 1000) != HAL_OK)
	{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
		return 1;
	}
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);

	MX25L_SPI_wait_write_complete();
	return 0;
}


/*****************************************************************
MX25L_SPI_read_buf: QSPI芯片读命令，单线三字节地址
	时序：命令（1字节）+地址（3字节）+空（1字节）+数据
	data_p：数据保存的地址
	address：地址
	num：读取的数量
返回值：0：成功；非0：失败
******************************************************************/
unsigned int MX25L_SPI_read_page(unsigned char *data_p, unsigned int address, unsigned int num)
{
	if ((data_p == 0) || (num == 0))
	{
		return 1;
	}
	
	if (num > 256)
	{
		num = 256;
	}
	
	
	G_tx_data[0] = FAST_READ_CMD;
	G_tx_data[1] = (address >> 16) & 0xFF;
	G_tx_data[2] = (address >> 8) & 0xFF;
	G_tx_data[3] = address & 0xFF;
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
	
	if (HAL_SPI_TransmitReceive(&hspi1, G_tx_data, G_rx_data, num + 5, 1000) != HAL_OK)
	{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
		return 1;
	}
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
	
	memcpy(data_p, G_rx_data + 5, num);
	
	return 0;  
}



/*****************************************************************
MX25L_SPI_write_page: QSPI芯片编程命令，单线三字节地址
	data_p：数据的地址
	address：地址
	num：编程的数量
返回值：0：成功；非0：失败
******************************************************************/
unsigned int MX25L_SPI_write_page(unsigned char *data_p, unsigned int address, unsigned int num)
{
	MX25L_SPI_write_enable();
    
	G_tx_data[0] = PAGE_PROGRAM_CMD;
	G_tx_data[1] = (address >> 16) & 0xFF;
	G_tx_data[2] = (address >> 8) & 0xFF;
	G_tx_data[3] = address & 0xFF;
	
	if (num > 256)
	{
		num = 256;
	}
	
	memcpy(&G_tx_data[4], data_p, num);
	
	
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
	
	if (HAL_SPI_TransmitReceive(&hspi1, G_tx_data, G_rx_data, num + 4, 1000) != HAL_OK)
	{
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
		return 1;
	}
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);

	MX25L_SPI_wait_write_complete();
	
	return 0;
}

unsigned int MX25L_SPI_read_byte(unsigned int address, int len)
{
	union U32_DATA var;
	var.u32 = 0;
	MX25L_SPI_read_page(var.u8,address,len);
	return var.u32;
}

unsigned int MX25L_SPI_write_byte(unsigned int address, unsigned int data, int len)
{
	int i;
	unsigned int start_address;
	unsigned int write_offset;
	union U32_DATA var;
	
	if (len <= 0)
	{
		return 1;
	}
	
	start_address = address & 0xFFFFF000;
	write_offset = address & 0x00000FFF;
	
	// 读4K的数据
	for (i = 0; i < 4096; i+=256)
	{
		MX25L_SPI_read_page(G_sector_buf + i, start_address + i, 256);
	}
	
	// 擦除
	MX25L_SPI_sector_erase(start_address);
	
	var.u32 = data;
	for (i = 0; i < len; i++)
	{
		G_sector_buf[write_offset] = var.u8[i];
		write_offset++;
	}
	
	// 写4K的数据
	for (i = 0; i < 4096; i+=256)
	{
		MX25L_SPI_write_page(G_sector_buf + i, start_address + i, 256);
	}

	return 0;
}


void MX25L_SPI_pid_data_init(void)
{
	int i;
	
	for (i = 0; i < 4096; i++)
	{
		G_sector_buf[0] = 0xFF;
	}
}

unsigned int MX25L_SPI_pid_data_read(unsigned int index, unsigned int *data)
{
	unsigned int address;
	unsigned int pid;
	
	if (index > 512)
	{
		return 0;
	}
	
	address = index * 8;
	
	pid = MX25L_SPI_read_byte(G_pid_data_start_address + address, 4);
	*data = MX25L_SPI_read_byte(G_pid_data_start_address + address + 4, 4);

	return pid;
}


void MX25L_SPI_pid_data_write(unsigned int index, unsigned int pid, unsigned int data)
{
	union U32_DATA u32_data;
	unsigned int address;
	
	if (index > 512)
	{
		return;
	}
	
	address = index * 8;
	u32_data.u32 = pid;
	G_sector_buf[address] = u32_data.u8[0];
	G_sector_buf[address + 1] = u32_data.u8[1];
	G_sector_buf[address + 2] = u32_data.u8[2];
	G_sector_buf[address + 3] = u32_data.u8[3];
	u32_data.u32 = data;
	G_sector_buf[address + 4] = u32_data.u8[0];
	G_sector_buf[address + 5] = u32_data.u8[1];
	G_sector_buf[address + 6] = u32_data.u8[2];
	G_sector_buf[address + 7] = u32_data.u8[3];
}

void MX25L_SPI_pid_data_save(void)
{
	int i;
	unsigned int start_address;
	
	start_address = G_pid_data_start_address;
	
	// 擦除
	MX25L_SPI_sector_erase(start_address);
	
	// 写4K的数据
	for (i = 0; i < 4096; i+=256)
	{
		MX25L_SPI_write_page(G_sector_buf + i, start_address + i, 256);
	}
}


extern void MEMORY_Print(unsigned int address, int line_num);
extern void DATA_Print(unsigned int address, unsigned char *data_p, int data_len);
void MX25L_SPI_print_data(unsigned int address, unsigned int page_num) 
{
	int i;
	// 读4K的数据
	for (i = 0; i < (page_num * 256); i+=256)
	{
		MX25L_SPI_read_page(G_sector_buf + i, address + i, 256);
		DATA_Print(address + i, G_sector_buf + i, 256);
	}
}

void MX25L_SPI_init(void)
{
//	int i;
//	unsigned int data;
	
	unsigned short id;
	id = MX25L_SPI_read_id();
	printf("MX25L_SPI_init: read spi flash id[%04X]\r\n", id);

//	for (i = 0; i < 4096; i++)
//	{
//		G_sector_buf[i] = i;
//	}
//	
//	MX25L_SPI_sector_erase(0);
//	MX25L_SPI_write_page(G_sector_buf, 0, 128);
//	
//	MX25L_SPI_read_page(G_sector_buf, 0, 256);
//	MEMORY_Print((unsigned int)G_sector_buf, 32);
//	
//	MX25L_SPI_write_byte(60, 0xAA, 1);
//	MX25L_SPI_write_byte(64, 0xBCDE, 2);
//	MX25L_SPI_write_byte(68, 0x23456789, 4);
//	
//	MX25L_SPI_read_page(G_sector_buf, 0, 256);
//	MEMORY_Print((unsigned int)G_sector_buf, 32);
//	
//	data = MX25L_SPI_read_byte(60, 1);
//	printf("MX25L_SPI_read_byte(60, 1) = %08X\r\n", data);
//	data = MX25L_SPI_read_byte(64, 2);
//	printf("MX25L_SPI_read_byte(64, 2) = %08X\r\n", data);
//	data = MX25L_SPI_read_byte(68, 4);
//	printf("MX25L_SPI_read_byte(68, 4) = %08X\r\n", data);
//	MX25L_SPI_print_data(0, 1);

	G_pid_data_start_address = PID_SAVE_DATA_BASE_ADDRESS;
}



