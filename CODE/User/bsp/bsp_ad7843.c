#include <string.h>
#include <stdio.h>
#include "gpio.h"
#include "stm32h7xx_hal.h"
#include "bsp_ad7843.h"

extern SPI_HandleTypeDef hspi3;

static unsigned char G_tx_data[8];
static unsigned char G_rx_data[8];

void AD7843_init(void)
{
	
}

extern void delay_us(int us);
unsigned short AD7843_get_ad(unsigned char cmd)
{
	unsigned short data;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
	
	if (HAL_SPI_TransmitReceive(&hspi3, &cmd, G_rx_data, 1, 1000) != HAL_OK)
	{
		return 0;
	}
	
//	while(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5) == SET)
//		;
	
	delay_us(6);
	
	if (HAL_SPI_TransmitReceive(&hspi3, G_tx_data, G_rx_data, 2, 1000) != HAL_OK)
	{
		return 0;
	}
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	
	//printf("G_rx_data = %02X, %02X\r\n", G_rx_data[0], G_rx_data[1]);
	data = G_rx_data[0] & 0X7F;
	data = data << 8;
	data += G_rx_data[1];
	data = data >> 3;
	
	//printf("read data = %04X(%d)\r\n", data, data);
	return data;
}

unsigned short G_adx_record_data = 0;
unsigned short G_ady_record_data = 0;


unsigned short AD7843_read_value(unsigned char cmd)
{
	unsigned short data;
//	int i;
	
	data = AD7843_get_ad(cmd);
	if (data >= 4096)
	{
		printf("AD7843_read_value(%02X) = %d\r\n", cmd, data);
	}
	return data;
}
