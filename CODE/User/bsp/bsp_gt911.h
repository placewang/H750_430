#ifndef _BSP_GT911_H_
#define _BSP_GT911_H_	

#include <stdbool.h>

//GT911 ���ּĴ������� 
#define GT911_READ_XY_REG          0x814E /* ����Ĵ��� */ 
#define GT911_CLEARBUF_REG         0x814E /* �������Ĵ��� */ 
#define GT911_CONFIG_REG           0x8047 /* ���ò����Ĵ��� */ 
#define GT911_COMMAND_REG          0x8040 /* ʵʱ���� */ 
#define GT911_PRODUCT_ID_REG       0x8140 /* оƬID */ 
#define GT911_VENDOR_ID_REG        0x814A /* ��ǰģ��ѡ����Ϣ */ 
#define GT911_CONFIG_VERSION_REG   0x8047 /* �����ļ��汾�� */ 
#define GT911_CONFIG_CHECKSUM_REG  0x80FF /* �����ļ�У���� */ 
#define GT911_FIRMWARE_VERSION_REG 0x8144 /* �̼��汾�� */ 
typedef struct
{
	unsigned char Enable;
	unsigned char TimerCount;
	unsigned char i2c_addr;
	
	unsigned char TouchpointFlag;
	unsigned char Touchkeystate;

	unsigned short X0;
	unsigned short Y0;
	unsigned short P0;

	unsigned short X1;
	unsigned short Y1;
	unsigned short P1;

	unsigned short X2;
	unsigned short Y2;
	unsigned short P2;

}GT911_T;
extern unsigned char G_gt911_ph_address;
 
void BSP_GT911_Init(void);
bool BSP_GT911_get_pen(void);
int BSP_GT911_Scan(unsigned short * x, unsigned short * y);
void BSP_GT911_timer_1ms(void);

#endif	// _BSP_GT911_H_



