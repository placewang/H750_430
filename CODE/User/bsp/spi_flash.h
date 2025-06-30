#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

// 容量定义
#define FLASH_BYTES_PER_SECTOR				4096	// 每段4K
#define PAGE_SIZE							256		// 每页256

// 命令
#define READ_ID_CMD							0x9F	// 读取ID
#define FAST_READ_CMD						0x0B	// 读取命令：单线三字节地址
#define FAST_READ_4B_CMD					0x0C	// 读取命令：单线四字节地址
#define FAST_READ_4P_CMD					0xEB	// 读取命令：四线三字节地址
#define FAST_READ_4B_4P_CMD					0xEC	// 读取命令：四线四字节地址
#define PAGE_PROGRAM_CMD					0x02	// 编程命令：单线三字节地址
#define PAGE_PROGRAM_4B_CMD					0x12	// 编程命令：单线四字节地址
#define PAGE_PROGRAM_4P_CMD					0x38	// 编程命令：四线三字节地址
#define PAGE_PROGRAM_4B_4P_CMD				0x3E	// 编程命令：四线四字节地址
#define QUAD_PAGE_PROGRAM_CMD				0x38
#define WRITE_ENABLE_CMD					0x06	// 写使能
#define READ_STATUS_REG_CMD					0x05	// 读取状态
#define READ_CONFIG_REG_CMD					0x15	// 读取配置
#define WRITE_STATUS_REG_CMD				0x01	// 写状态寄存器

#define SECTOR_ERASE_CMD					0x20	// 扇擦除：单线三字节地址
#define SECTOR_ERASE_4B_CMD					0x21	// 扇擦除：单线四字节地址
#define BLOCK_ERASE_CMD						0xD8	// 块擦除：单线三字节地址
#define BLOCK_ERASE_4B_CMD					0xDC	// 块擦除：单线四字节地址
#define BLOCK_32K_ERASE_CMD					0x52
#define CHIP_ERASE_CMD						0x60
#define CHIP_ERASE_CMD2						0xC7

#define ENTER_4B_MODE_CMD					0xB7
#define EXIT_4B_MODE_CMD					0xE9

// 目前支持的两个芯片
#define FLASH_CHIP_MX25L3233_ID				0xC216
#define FLASH_CHIP_MX25L6433_ID				0xC217
#define FLASH_CHIP_MX25L2567_ID				0xC219


#endif	// _SPI_FLASH_H_
