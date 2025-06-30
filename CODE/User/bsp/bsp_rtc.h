#ifndef _BSP_RTC_H_
#define _BSP_RTC_H_

void BSP_RTC_init(void);
void BSP_RTC_upgrade_flag_write(unsigned short flag_data);
unsigned short BSP_RTC_upgrade_flag_read(void);

void BSP_RTC_set_upgrade_flag(void);
void BSP_RTC_clear_upgrade_flag(void);
int BSP_RTC_if_app_want_upgrade(void);

#endif // _BSP_RTC_H_
