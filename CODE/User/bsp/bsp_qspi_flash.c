#include <string.h>
#include <stdio.h>
#include "stm32h7xx_hal.h"
#include "bsp_qspi_flash.h"
#include "spi_flash.h"

extern QSPI_HandleTypeDef hqspi;


unsigned char G_qspi_init_flag = 0;
unsigned short G_flash_id = 0;

/*****************************************************************
QSPI_read_status: QSPI��ȡоƬ��״̬
����ֵ��оƬ״̬�ֽ�
******************************************************************/
unsigned char BSP_QSPI_FLASH_read_status(void)
{
//	unsigned short id;
	unsigned char data[8];
	QSPI_CommandTypeDef sCommand;

	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD; 

	sCommand.Instruction       = READ_STATUS_REG_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0; 
	sCommand.NbData            = 1;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 0xFF;
	}

	if (HAL_QSPI_Receive(&hqspi, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 0xFF;
	}   
	
	return data[0];
}

/*****************************************************************
QSPI_write_enable: QSPIоƬдʹ��
******************************************************************/
void BSP_QSPI_FLASH_write_enable(void)
{
	int i;
	unsigned char status;
	
	QSPI_CommandTypeDef s_command;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.Instruction       = WRITE_ENABLE_CMD;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DataMode          = QSPI_DATA_NONE;
	s_command.DummyCycles       = 0;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return;
	}

	for (i = 0; i < 1000; i++)
	{
		status = BSP_QSPI_FLASH_read_status();
		if ((status & 0x02) == 0x02)
		{// �������
			return;
		}
		HAL_Delay(1);
	}
}

/*****************************************************************
BSP_QSPI_FLASH_wait_write_complete: QSPI�ȴ�д�������
******************************************************************/
void BSP_QSPI_FLASH_wait_write_complete(void)
{
	unsigned char status;
	int i;
	for (i = 0; i < 1000; i++)
	{
		HAL_Delay(1);
		status = BSP_QSPI_FLASH_read_status();
		if ((status & 0x01) == 0)
		{
			return;
		}
	}
}

/*****************************************************************
BSP_QSPI_FLASH_set_quad_enable: QSPIоƬ����4�߲���ģʽ��
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
unsigned char BSP_QSPI_FLASH_set_quad_enable(void)
{
	unsigned char data[8];
	
	QSPI_CommandTypeDef sCommand;

	data[0] = BSP_QSPI_FLASH_read_status();
	data[0] |= 0x40;
	
	BSP_QSPI_FLASH_write_enable();
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD; 

	sCommand.Instruction       = WRITE_STATUS_REG_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0; 
	sCommand.NbData            = 1;
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}

	if (HAL_QSPI_Transmit(&hqspi, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}
	
	BSP_QSPI_FLASH_wait_write_complete();
	return 0;
}


/*****************************************************************
BSP_QSPI_FLASH_read_id: QSPI��ȡоƬ��ID
����ֵ��ID
******************************************************************/
unsigned short BSP_QSPI_FLASH_read_id(void)
{
	unsigned short id;
	unsigned char data[8];
	QSPI_CommandTypeDef sCommand;

	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD; 

	sCommand.Instruction       = READ_ID_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0; 
	sCommand.NbData            = 3;

	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 0xFFFF;
	}

	if (HAL_QSPI_Receive(&hqspi, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 0xFFFF;
	}   
	
	id = data[0];
	id = id << 8;
	id += data[2];
	
	return id;
}


/*****************************************************************
__BSP_QSPI_FLASH_SectorErase: QSPIоƬ4K�Ȳ�����3�ֽڵ�ַ��Ϣ
	address����ַ
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_SectorErase(unsigned int address)
{
    QSPI_CommandTypeDef sCommand = {0};
	
	BSP_QSPI_FLASH_write_enable();
    
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS; 
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
 
    sCommand.Instruction = SECTOR_ERASE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
    sCommand.DataMode    = QSPI_DATA_NONE;
    sCommand.Address     = address;
	sCommand.DummyCycles = 0;
	sCommand.NbData      = 0;
	
    if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return 1;
    }
	
	BSP_QSPI_FLASH_wait_write_complete();
	
	return 0;
	
}

/*****************************************************************
__BSP_QSPI_FLASH_SectorErase_4B: QSPIоƬ4K�Ȳ�����4�ֽڵ�ַ��Ϣ
	address����ַ
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_SectorErase_4B(unsigned int address)
{
    QSPI_CommandTypeDef sCommand = {0};
	
	BSP_QSPI_FLASH_write_enable();
    
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
    sCommand.AddressSize       = QSPI_ADDRESS_32_BITS; 
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
 
    sCommand.Instruction = SECTOR_ERASE_4B_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
    sCommand.DataMode    = QSPI_DATA_NONE;
    sCommand.Address     = address;
	sCommand.DummyCycles = 0;
    
    if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return 1;
    }
	
	BSP_QSPI_FLASH_wait_write_complete();
	
	return 0;
}

/*****************************************************************
QSPI_sector_erase: QSPIоƬ4K�Ȳ������ⲿ����
	address����ַ
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
unsigned int QSPI_sector_erase(unsigned int address)
{
	if (G_qspi_init_flag == 0)
	{
		return 1;
	}
	
	if (G_flash_id == FLASH_CHIP_MX25L3233_ID)
	{
		return __BSP_QSPI_FLASH_SectorErase(address);
	}
	else if (G_flash_id == FLASH_CHIP_MX25L2567_ID)
	{
		return __BSP_QSPI_FLASH_SectorErase_4B(address);
	}

	return 1;
}

/*****************************************************************
__BSP_QSPI_FLASH_BlockErase: QSPIоƬ64K�������3�ֽڵ�ַ��Ϣ
	address����ַ
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_BlockErase(unsigned int address)
{
    QSPI_CommandTypeDef sCommand = {0};
	
	BSP_QSPI_FLASH_write_enable();
    
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
    sCommand.AddressSize       = QSPI_ADDRESS_24_BITS; 
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
 
    sCommand.Instruction = BLOCK_ERASE_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
    sCommand.DataMode    = QSPI_DATA_NONE;
    sCommand.Address     = address;
	sCommand.DummyCycles = 0;
    
    if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return 1;
    }
	
	BSP_QSPI_FLASH_wait_write_complete();
	
	return 0;
}

/*****************************************************************
__BSP_QSPI_FLASH_BlockErase_4B: QSPIоƬ64K�������4�ֽڵ�ַ��Ϣ
	address����ַ
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_BlockErase_4B(unsigned int address)
{
    QSPI_CommandTypeDef sCommand = {0};
	
	BSP_QSPI_FLASH_write_enable();
    
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
    sCommand.AddressSize       = QSPI_ADDRESS_32_BITS; 
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
 
    sCommand.Instruction = BLOCK_ERASE_4B_CMD;
    sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
    sCommand.DataMode    = QSPI_DATA_NONE;
    sCommand.Address     = address;
	sCommand.DummyCycles = 0;
    
    if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return 1;
    }
	
	BSP_QSPI_FLASH_wait_write_complete();
	
	return 0;
	
}

/*****************************************************************
BSP_QSPI_FLASH_block_erase: QSPIоƬ64K��������ⲿ����
	address����ַ
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
unsigned int BSP_QSPI_FLASH_block_erase(unsigned int address)
{
	if (G_qspi_init_flag == 0)
	{
		return 1;
	}
	
	if (G_flash_id == FLASH_CHIP_MX25L3233_ID)
	{
		return __BSP_QSPI_FLASH_BlockErase(address);
	}
	else if (G_flash_id == FLASH_CHIP_MX25L2567_ID)
	{
		return __BSP_QSPI_FLASH_BlockErase_4B(address);
	}

	return 1;
}

/*****************************************************************
__BSP_QSPI_FLASH_ReadBuffer: QSPIоƬ������������ֽڵ�ַ
	data_p�����ݱ���ĵ�ַ
	address����ַ
	num����ȡ������
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_ReadBuffer(unsigned char *data_p, unsigned int address, unsigned int num)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	if ((data_p == 0) || (num == 0))
	{
		return 1;
	}
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
	
	sCommand.Instruction = FAST_READ_CMD;
	sCommand.DummyCycles = 8;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
	sCommand.DataMode    = QSPI_DATA_1_LINE;
	sCommand.NbData      = num;
	sCommand.Address     = address;
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}

	if (HAL_QSPI_Receive(&hqspi, data_p, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	} 
	
	return 0;  
}

/*****************************************************************
__BSP_QSPI_FLASH_ReadBuffer_4B: QSPIоƬ������������ֽڵ�ַ
	data_p�����ݱ���ĵ�ַ
	address����ַ
	num����ȡ������
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_ReadBuffer_4B(unsigned char *data_p, unsigned int address, unsigned int num)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	if ((data_p == 0) || (num == 0))
	{
		return 1;
	}
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_32_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
	
	sCommand.Instruction = FAST_READ_4B_CMD;
	sCommand.DummyCycles = 8;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
	sCommand.DataMode    = QSPI_DATA_1_LINE;
	sCommand.NbData      = num;
	sCommand.Address     = address;
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}
	
	if (HAL_QSPI_Receive(&hqspi, data_p, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	} 
	
	return 0;  
}

/*****************************************************************
QSPI_read_spi_flash: QSPIоƬ������ⲿ����
	data_p�����ݱ���ĵ�ַ
	address����ַ
	num����ȡ������
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
unsigned int QSPI_read_spi_flash(unsigned char *data_p, unsigned int address, unsigned int num)
{
	if (G_qspi_init_flag == 0)
	{
		return 1;
	}
	
	if (G_flash_id == FLASH_CHIP_MX25L3233_ID)
	{
		return __BSP_QSPI_FLASH_ReadBuffer(data_p, address, num);
	}
	else if (G_flash_id == FLASH_CHIP_MX25L2567_ID)
	{
		return __BSP_QSPI_FLASH_ReadBuffer_4B(data_p, address, num);
	}

	return 1;
}

/*****************************************************************
__BSP_QSPI_FLASH_ReadBuffer_4P: QSPIоƬ������������ֽڵ�ַ
	data_p�����ݱ���ĵ�ַ
	address����ַ
	num����ȡ������
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_ReadBuffer_4P(unsigned char *data_p, unsigned int address, unsigned int num)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	if ((data_p == 0) || (num == 0))
	{
		return 1;
	}
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
	
	sCommand.Instruction = FAST_READ_4P_CMD;
	sCommand.DummyCycles = 6;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.DataMode    = QSPI_DATA_4_LINES;
	sCommand.NbData      = num;
	sCommand.Address     = address;
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}

	if (HAL_QSPI_Receive(&hqspi, data_p, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	} 
	
	return 0;  
}

/*****************************************************************
__QSPI_ReadBuffer_4B_4P: QSPIоƬ������������ֽڵ�ַ
	data_p�����ݱ���ĵ�ַ
	address����ַ
	num����ȡ������
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_ReadBuffer_4B_4P(unsigned char *data_p, unsigned int address, unsigned int num)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	if ((data_p == 0) || (num == 0))
	{
		return 1;
	}
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_32_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
	
	sCommand.Instruction = FAST_READ_4B_4P_CMD;
	sCommand.DummyCycles = 8;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.DataMode    = QSPI_DATA_4_LINES;
	sCommand.NbData      = num;
	sCommand.Address     = address;
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}
	
	if (HAL_QSPI_Receive(&hqspi, data_p, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	} 
	
	return 0;  
}

/*****************************************************************
BSP_QSPI_FLASH_read_spi_flash_4p: QSPIоƬ������ⲿ���ã�4IOģʽ
	data_p�����ݱ���ĵ�ַ
	address����ַ
	num����ȡ������
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
unsigned int BSP_QSPI_FLASH_read_spi_flash_4p(unsigned char *data_p, unsigned int address, unsigned int num)
{
	if (G_qspi_init_flag == 0)
	{
		return 1;
	}
	
	if (G_flash_id == FLASH_CHIP_MX25L3233_ID)
	{
		return __BSP_QSPI_FLASH_ReadBuffer_4P(data_p, address, num);
	}
	else if (G_flash_id == FLASH_CHIP_MX25L2567_ID)
	{
		return __BSP_QSPI_FLASH_ReadBuffer_4B_4P(data_p, address, num);
	}

	return 1;
}


/*****************************************************************
__BSP_QSPI_FLASH_WriteBuffer: QSPIоƬ�������������ֽڵ�ַ
	data_p�����ݵĵ�ַ
	address����ַ
	num����̵�����
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_WriteBuffer(unsigned char *data_p, unsigned int address, unsigned int num)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	if ((data_p == 0) || (num == 0))
	{
		return 1;
	}
	
	BSP_QSPI_FLASH_write_enable();
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
	
	sCommand.Instruction = PAGE_PROGRAM_CMD;
	sCommand.DummyCycles = 0;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
	sCommand.DataMode    = QSPI_DATA_1_LINE;
	sCommand.NbData      = num;
	sCommand.Address     = address;
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}
	
	if (HAL_QSPI_Transmit(&hqspi, data_p, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}   
	
	BSP_QSPI_FLASH_wait_write_complete();
	
	return 0;
}

/*****************************************************************
__QSPI_WriteBuffer_4B: QSPIоƬ�������������ֽڵ�ַ
	data_p�����ݵĵ�ַ
	address����ַ
	num����̵�����
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_WriteBuffer_4B(unsigned char *data_p, unsigned int address, unsigned int num)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	if ((data_p == 0) || (num == 0))
	{
		return 1;
	}
	
	BSP_QSPI_FLASH_write_enable();
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_32_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
	
	sCommand.Instruction = PAGE_PROGRAM_4B_CMD;
	sCommand.DummyCycles = 0;
	sCommand.AddressMode = QSPI_ADDRESS_1_LINE;
	sCommand.DataMode    = QSPI_DATA_1_LINE;
	sCommand.NbData      = num;
	sCommand.Address     = address;
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}
	
	if (HAL_QSPI_Transmit(&hqspi, data_p, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}   
	
	BSP_QSPI_FLASH_wait_write_complete();
	
	return 0;
}

/*****************************************************************
BSP_QSPI_FLASH_write_spi_flash: QSPIоƬ�������ⲿ����
	data_p�����ݵĵ�ַ
	address����ַ
	num��д������
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
unsigned int BSP_QSPI_FLASH_write_spi_flash(unsigned char *data_p, unsigned int address, unsigned int num)
{
	if (G_qspi_init_flag == 0)
	{
		return 1;
	}
	
	if (G_flash_id == FLASH_CHIP_MX25L3233_ID)
	{
		return __BSP_QSPI_FLASH_WriteBuffer(data_p, address, num);
	}
	else if (G_flash_id == FLASH_CHIP_MX25L2567_ID)
	{
		return __BSP_QSPI_FLASH_WriteBuffer_4B(data_p, address, num);
	}

	return 1;
}

/*****************************************************************
__BSP_QSPI_FLASH_WriteBuffer_4P: QSPIоƬ�������������ֽڵ�ַ
	data_p�����ݵĵ�ַ
	address����ַ
	num����̵�����
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_WriteBuffer_4P(unsigned char *data_p, unsigned int address, unsigned int num)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	if ((data_p == 0) || (num == 0))
	{
		return 1;
	}
	
	BSP_QSPI_FLASH_write_enable();
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
	
	sCommand.Instruction = PAGE_PROGRAM_4P_CMD;
	sCommand.DummyCycles = 0;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.DataMode    = QSPI_DATA_4_LINES;
	sCommand.NbData      = num;
	sCommand.Address     = address;
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}
	
	if (HAL_QSPI_Transmit(&hqspi, data_p, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}   
	
	BSP_QSPI_FLASH_wait_write_complete();
	
	return 0;
}

/*****************************************************************
__BSP_QSPI_FLASH_WriteBuffer_4B_4P: QSPIоƬ�������������ֽڵ�ַ
	data_p�����ݵĵ�ַ
	address����ַ
	num����̵�����
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_WriteBuffer_4B_4P(unsigned char *data_p, unsigned int address, unsigned int num)
{
	QSPI_CommandTypeDef sCommand = {0};
	
	if ((data_p == 0) || (num == 0))
	{
		return 1;
	}
	
	BSP_QSPI_FLASH_write_enable();
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_32_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  
	
	sCommand.Instruction = PAGE_PROGRAM_4B_4P_CMD;
	sCommand.DummyCycles = 0;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.DataMode    = QSPI_DATA_4_LINES;
	sCommand.NbData      = num;
	sCommand.Address     = address;
	
	if (HAL_QSPI_Command(&hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}
	

	if (HAL_QSPI_Transmit(&hqspi, data_p, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return 1;
	}   
	
	BSP_QSPI_FLASH_wait_write_complete();
	
	return 0;
}

/*****************************************************************
QSPI_write_spi_flash_4p: QSPIоƬ������4IO��ʽ���ⲿ����
	data_p�����ݵĵ�ַ
	address����ַ
	num��д������
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
unsigned int QSPI_write_spi_flash_4p(unsigned char *data_p, unsigned int address, unsigned int num)
{
	if (G_qspi_init_flag == 0)
	{
		return 1;
	}
	
	if (G_flash_id == FLASH_CHIP_MX25L3233_ID)
	{
		return __BSP_QSPI_FLASH_WriteBuffer_4P(data_p, address, num);
	}
	else if (G_flash_id == FLASH_CHIP_MX25L2567_ID)
	{
		return __BSP_QSPI_FLASH_WriteBuffer_4B_4P(data_p, address, num);
	}

	return 1;
}

/*****************************************************************
__BSP_QSPI_FLASH_memory_mapped: QSPI�ڴ�ӳ�䣬3�ֽڵ�ַ
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_memory_mapped(void)
{
	QSPI_CommandTypeDef      sCommand;
	QSPI_MemoryMappedTypeDef sMemMappedCfg;
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  

	sCommand.Instruction = FAST_READ_4P_CMD;
	sCommand.DummyCycles = 6;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.DataMode    = QSPI_DATA_4_LINES;
	
	sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
	sMemMappedCfg.TimeOutPeriod     = 0;
	if (HAL_QSPI_MemoryMapped(&hqspi, &sCommand, &sMemMappedCfg) != HAL_OK)
	{
		return HAL_ERROR;
	}
	
	return HAL_OK;
}

/*****************************************************************
__BSP_QSPI_FLASH_memory_mapped_4B: QSPI�ڴ�ӳ�䣬4�ֽڵ�ַ
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
static unsigned int __BSP_QSPI_FLASH_memory_mapped_4B(void)
{
	QSPI_CommandTypeDef      sCommand;
	QSPI_MemoryMappedTypeDef sMemMappedCfg;
	
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE; 
	sCommand.AddressSize       = QSPI_ADDRESS_32_BITS; 
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE; 
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;   
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;  
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;  

	sCommand.Instruction = FAST_READ_4B_4P_CMD;
	sCommand.DummyCycles = 8;
	sCommand.AddressMode = QSPI_ADDRESS_4_LINES;
	sCommand.DataMode    = QSPI_DATA_4_LINES;
	
	sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
	sMemMappedCfg.TimeOutPeriod     = 0;
	if (HAL_QSPI_MemoryMapped(&hqspi, &sCommand, &sMemMappedCfg) != HAL_OK)
	{
		return HAL_ERROR;
	}
	
	return HAL_OK;
}

/*****************************************************************
QSPI_memory_mapped: QSPI�ڴ�ӳ�䣬�ⲿ����
����ֵ��0���ɹ�����0��ʧ��
******************************************************************/
unsigned int QSPI_memory_mapped(void)
{
	if (G_qspi_init_flag == 0)
	{
		return 1;
	}
	
	if ((G_flash_id == FLASH_CHIP_MX25L3233_ID) || (G_flash_id == FLASH_CHIP_MX25L6433_ID))
	{
		return __BSP_QSPI_FLASH_memory_mapped();
	}
	else if (G_flash_id == FLASH_CHIP_MX25L2567_ID)
	{
		return __BSP_QSPI_FLASH_memory_mapped_4B();
	}

	return 1;
}

extern const unsigned char test[];
/*****************************************************************
BSP_QSPI_FLASH_init: QSPI�ĳ�ʼ��������ȫ��MX_QUADSPI_Init�У�
			 MX_QUADSPI_InitΪ�Զ����ɣ���main.c��
******************************************************************/
int BSP_QSPI_FLASH_init(void)
{
	unsigned char result;

	result = BSP_QSPI_FLASH_set_quad_enable();
	if (result != 0)
	{
		printf("<QSPI> QSPI_set_quad_enable failed\r\n");
		HAL_QSPI_DeInit(&hqspi);
		return -1;
	}
	result = BSP_QSPI_FLASH_read_status();
	printf("<QSPI> QSPI_set_quad_enable success, device status [%2X]\r\n", result);
	
	G_flash_id = BSP_QSPI_FLASH_read_id();
	printf("<QPSI> QSPI_read_id = %04X, ", G_flash_id);
	if (G_flash_id == FLASH_CHIP_MX25L3233_ID)
	{
		printf("flash chip is [MX25L3233][4MB]\r\n");
	}
	else if (G_flash_id == FLASH_CHIP_MX25L6433_ID)
	{
		printf("flash chip is [MX25L6433][8MB]\r\n");
	}
	else if (G_flash_id == FLASH_CHIP_MX25L2567_ID)
	{
		printf("flash chip is [MX25L2567][32MB]\r\n");
	}
	else
	{
		printf("flash chip is [unknown]\r\n");
	}
	G_qspi_init_flag = 1;
	
	result = QSPI_memory_mapped();
	if (result == HAL_OK)
	{
		printf("<QSPI> QSPI_memory_mapped success\r\n");
		printf("EXT FLASH data, test = %02X %02X %02X %02X\r\n",test[0], test[1], test[2], test[3]);
	}
	else
	{
		printf("<QSPI> QSPI_memory_mapped failed\r\n");
	}
	
	return 0;
}
