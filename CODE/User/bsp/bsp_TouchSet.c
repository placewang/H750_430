#include <stdio.h>
#include "bsp_touch.h"
#include "bsp_gt911.h"
#include "bsp_iic.h"
typedef struct
{
	unsigned char type;
}TouchSet;

TouchSet Touch={0};


void BSP_TouchInit(void)
{
    CT_IIC_Init();
    BSP_GT911_Init();
    if(G_gt911_ph_address==0)
    {
        TP_Init(); 
    }
}

void BSP_TOUCH_loop_1ms(void)
{
    if(G_gt911_ph_address==0)
    {
        BSP_ad7843_loop_1ms();
    }
    else
    {
        BSP_GT911_timer_1ms();
    }
}
unsigned char BSP_TOUCH_get_press(void)
{
    char touch_press;
    if(G_gt911_ph_address==0)
    {
        touch_press=BSP_ad7843_get_press();
    }
    else
    {
        touch_press=BSP_GT911_get_pen();
    }
    return touch_press;    
}
void BSP_TOUCH_get_position(unsigned short *x, unsigned short *y)
{
    if(G_gt911_ph_address==0)
    {
        BSP_ad7843_get_position(x,y);
    }
    else
    {
        BSP_GT911_Scan(x,y);
    }
}


