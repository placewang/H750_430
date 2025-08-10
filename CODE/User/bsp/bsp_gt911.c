#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gpio.h"
#include "bsp_iic.h"
#include "bsp_gt911.h"
//#include "bsp_bell.h"

//GT911 ID :00313139, Version :1060
#define GT991_I2C_PH_ADDRESS1		0x28
#define GT991_I2C_PH_ADDRESS2		0xBA

unsigned char G_gt911_ph_address;
unsigned char G_gt911_id;
unsigned char G_gt911_version;

unsigned char G_write_buf[32];
unsigned char G_read_buf[32];

unsigned char G_int_pen_crt_status = 0;
unsigned char G_int_pen_dealy_count = 0;
unsigned short G_crt_x_position;
unsigned short G_crt_y_position;


GT911_T g_GT911;
/*
*	函 数 名: GT911_ReadReg
*	功能说明: 读1个或连续的多个寄存器
*	形    参: _usRegAddr : 寄存器地址
*			  _pRegBuf : 寄存器数据缓冲区
*			 _ucLen : 数据长度
*	返 回 值: 无
*/
static void GT911_ReadReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen)
{
	uint8_t i;
	
	{
		i2c_Start();					/* 总线开始信号 */

		i2c_SendByte(g_GT911.i2c_addr);	/* 发送设备地址+写信号 */
		i2c_WaitAck();

		i2c_SendByte(_usRegAddr >> 8);	/* 地址高8位 */
		i2c_WaitAck();

		i2c_SendByte(_usRegAddr);		/* 地址低8位 */
		i2c_WaitAck();

		i2c_Start();
		i2c_SendByte(g_GT911.i2c_addr + 0x01);	/* 发送设备地址+读信号 */

		i2c_WaitAck();
	}
	
	for (i = 0; i < 30; i++);

	for (i = 0; i < _ucLen - 1; i++)
	{
	    _pRegBuf[i] = i2c_ReadByte();	/* 读寄存器数据 */
		i2c_Ack();
	}

	/* 最后一个数据 */
	 _pRegBuf[i] = i2c_ReadByte();		/* 读寄存器数据 */
	i2c_NAck();

    i2c_Stop();							/* 总线停止信号 */
}

/*
*	函 数 名: GT911_WriteReg
*	功能说明: 写1个或连续的多个寄存器
*	形    参: _usRegAddr : 寄存器地址
*			  _pRegBuf : 寄存器数据缓冲区
*			 _ucLen : 数据长度
*	返 回 值: 无
*/
static void GT911_WriteReg(uint16_t _usRegAddr, uint8_t *_pRegBuf, uint8_t _ucLen)
{
	uint8_t i;

    i2c_Start();					/* 总线开始信号 */

    i2c_SendByte(g_GT911.i2c_addr);	/* 发送设备地址+写信号 */
	i2c_WaitAck();

    i2c_SendByte(_usRegAddr >> 8);	/* 地址高8位 */
	i2c_WaitAck();

    i2c_SendByte(_usRegAddr);		/* 地址低8位 */
	i2c_WaitAck();

	for (i = 0; i < _ucLen; i++)
	{
	    i2c_SendByte(_pRegBuf[i]);	/* 寄存器数据 */
		i2c_WaitAck();
	}
    i2c_Stop();                   	/* 总线停止信号 */
}
unsigned int BSP_GT911_read_id(void)
{
	unsigned int id = 0;
    
	GT911_ReadReg(GT911_PRODUCT_ID_REG,G_read_buf, 4);
	
    id = G_read_buf[3];
    id = id << 8;
    id += G_read_buf[2];
    id = id << 8;
    id += G_read_buf[1];
    id = id << 8;
    id += G_read_buf[0];
	
	return id;
}

unsigned short BSP_GT911_read_version(void)
{
    uint8_t buf[2]; 
	GT911_ReadReg(GT911_FIRMWARE_VERSION_REG, buf, 2);

    return ((uint16_t)buf[1] << 8) + buf[0]; 
}

int BSP_GT911_get_coordinate(unsigned short *x_point, unsigned short *y_point)
{
	unsigned short x, y;
	
	GT911_ReadReg(GT911_READ_XY_REG, G_read_buf, 8);

    G_write_buf[0] = 0;
    GT911_WriteReg(GT911_CLEARBUF_REG,G_write_buf, 1);
    
    x = ((uint16_t)G_read_buf[3] << 8) + G_read_buf[2];
    y = ((uint16_t)G_read_buf[5] << 8) + G_read_buf[4];
    
    if (x > 480)
        x = 480;
    if (y > 272)
        y = 272;
    
    *x_point = x;
    *y_point = y;

    //printf("x = %d, y = %d\r\n", x, y);
    return 0;
}

void BSP_GT911_Init(void)
{   
    GPIO_InitTypeDef gpio_init;
	unsigned short x, y;
	// 硬件复位
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_Delay(200);
	
	if (i2c_CheckDevice(GT991_I2C_PH_ADDRESS2)==0)
	{
        g_GT911.i2c_addr=GT991_I2C_PH_ADDRESS2;
		G_gt911_ph_address = GT991_I2C_PH_ADDRESS2;
	}
	else if (i2c_CheckDevice(GT991_I2C_PH_ADDRESS1)==0)
	{
        g_GT911.i2c_addr=GT991_I2C_PH_ADDRESS1;
		G_gt911_ph_address = GT991_I2C_PH_ADDRESS1;
	}
	else
	{
		G_gt911_ph_address = 0;
	}
	printf("GT911 ADDRESS [%02X]: ", G_gt911_ph_address);
    G_gt911_id=BSP_GT911_read_id(), 
    G_gt911_version= BSP_GT911_read_version();
	printf("ID = %08X, VERSION = %04X\r\n", G_gt911_id, G_gt911_version);

	while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5) == GPIO_PIN_RESET)
	{
		BSP_GT911_get_coordinate(&x, &y);
		HAL_Delay(100);
	}
      /*Configure GPIO pin : PD11 */
    gpio_init.Pin = GPIO_PIN_11;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOD, &gpio_init);	
	G_crt_x_position = 0xFFFF;
	G_crt_y_position = 0xFFFF;
	G_int_pen_crt_status = 1;
	G_int_pen_dealy_count = 0;
}

bool BSP_GT911_get_pen(void)
{
//	if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5) == GPIO_PIN_RESET)
//	{
//		return true;
//	}
//	return false;
	if (G_int_pen_crt_status == 0)
	{
		return true;
	}
	return false;
}

int BSP_GT911_Scan(uint16_t * x, uint16_t * y)
{
//	if (BSP_GT911_get_pen() == true)
//	{
//		return BSP_GT911_get_coordinate(x,y);
//	}
	*x = G_crt_x_position;
	*y = G_crt_y_position;
	return 1;
}


void BSP_GT911_timer_1ms(void)
{
	static unsigned char count = 0;
	unsigned char pin_status;
	
	count++;
	if (count >= 10)
	{
		count = 0;
		pin_status = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11);
		if (G_int_pen_crt_status != pin_status)
		{
			G_int_pen_dealy_count++;
			if (G_int_pen_dealy_count >= 5)
			{
				G_int_pen_dealy_count = 0;
				G_int_pen_crt_status = pin_status;
				if (G_int_pen_crt_status == 1)
				{
					G_crt_x_position = 0xFFFF;
					G_crt_y_position = 0xFFFF;
				}
				//printf("G_int_pen_crt_status = %d\r\n", G_int_pen_crt_status);
			}
		}
		else
		{
			G_int_pen_dealy_count = 0;
		}
		
		if (pin_status == 0)
		{
			BSP_GT911_get_coordinate(&G_crt_x_position, &G_crt_y_position);
		}
		
//		if (G_int_pen_crt_status == 0)
//		{
//			printf("X=%d,Y=%d\r\n", G_crt_x_position, G_crt_y_position);
//		}
	}
}

