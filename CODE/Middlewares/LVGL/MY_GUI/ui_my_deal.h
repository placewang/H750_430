#ifndef UI_MY_DEAL_H_
#define UI_MY_DEAL_H_

#include "ui.h"

#define SCREEN_LOCK_TIME				60000
#define SCREEN_BACKLIGHT_CLOSE_TIME		30000

void UI_my_deal_init(void);
void UI_my_deal_loop_1ms(void);
void GUI_setUpDw_motorNoOrOff( signed char);

void UI_StartRecovery(void);

#endif	// UI_MY_DEAL_H_





