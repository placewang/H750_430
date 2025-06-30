#ifndef _BSP_TOUCHSET_H_
#define _BSP_TOUCHSET_H_


void BSP_TouchInit(void);
void BSP_TOUCH_loop_1ms(void);
unsigned char BSP_TOUCH_get_press(void);
void BSP_TOUCH_get_position(unsigned short *x, unsigned short *y);
#endif

