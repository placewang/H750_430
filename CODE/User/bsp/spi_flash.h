#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

// ��������
#define FLASH_BYTES_PER_SECTOR				4096	// ÿ��4K
#define PAGE_SIZE							256		// ÿҳ256

// ����
#define READ_ID_CMD							0x9F	// ��ȡID
#define FAST_READ_CMD						0x0B	// ��ȡ����������ֽڵ�ַ
#define FAST_READ_4B_CMD					0x0C	// ��ȡ����������ֽڵ�ַ
#define FAST_READ_4P_CMD					0xEB	// ��ȡ����������ֽڵ�ַ
#define FAST_READ_4B_4P_CMD					0xEC	// ��ȡ����������ֽڵ�ַ
#define PAGE_PROGRAM_CMD					0x02	// �������������ֽڵ�ַ
#define PAGE_PROGRAM_4B_CMD					0x12	// �������������ֽڵ�ַ
#define PAGE_PROGRAM_4P_CMD					0x38	// �������������ֽڵ�ַ
#define PAGE_PROGRAM_4B_4P_CMD				0x3E	// �������������ֽڵ�ַ
#define QUAD_PAGE_PROGRAM_CMD				0x38
#define WRITE_ENABLE_CMD					0x06	// дʹ��
#define READ_STATUS_REG_CMD					0x05	// ��ȡ״̬
#define READ_CONFIG_REG_CMD					0x15	// ��ȡ����
#define WRITE_STATUS_REG_CMD				0x01	// д״̬�Ĵ���

#define SECTOR_ERASE_CMD					0x20	// �Ȳ������������ֽڵ�ַ
#define SECTOR_ERASE_4B_CMD					0x21	// �Ȳ������������ֽڵ�ַ
#define BLOCK_ERASE_CMD						0xD8	// ��������������ֽڵ�ַ
#define BLOCK_ERASE_4B_CMD					0xDC	// ��������������ֽڵ�ַ
#define BLOCK_32K_ERASE_CMD					0x52
#define CHIP_ERASE_CMD						0x60
#define CHIP_ERASE_CMD2						0xC7

#define ENTER_4B_MODE_CMD					0xB7
#define EXIT_4B_MODE_CMD					0xE9

// Ŀǰ֧�ֵ�����оƬ
#define FLASH_CHIP_MX25L3233_ID				0xC216
#define FLASH_CHIP_MX25L6433_ID				0xC217
#define FLASH_CHIP_MX25L2567_ID				0xC219


#endif	// _SPI_FLASH_H_
