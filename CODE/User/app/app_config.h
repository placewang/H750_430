#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

#define APP_CONFIG_START_PID	0x00000005	// 起始PID，数据为0x5AA5AA55
#define APP_CONFIG_END_PID		0x00000000	// 结束PID，数据为记录的数量
#define APP_CONFIG_START_FLAG	0x5AA5AA55	// 起始PID的标志


// 放料操作从参数
#define APP_CONFIG_PID_RUN_MODE		            0x1000		//放料运行模式
#define APP_CONFIG_PID_UP_MOVE_ENAE		        0x1001		//上放料工作使能
#define APP_CONFIG_PID_UP_DRI           		0x1002		//上放料方向
#define APP_CONFIG_PID_UP_TENSION_POS           0x1003      //上放料张力位置
#define APP_CONFIG_PID_UP_JOG_SPEED             0x1004      //上放料点动速度
#define APP_CONFIG_PID_UP_PERlIMIT              0x1009      //上放料张力上限位置
#define APP_CONFIG_PID_UP_LOWERlIMIT            0x100A      //上放料张力下线位置 
#define APP_CONFIG_PID_UP_ENCODERFDIR            0x100D      //上编码器上电默认方向

#define APP_CONFIG_PID_DOWN_MOVE_ENAE		    0x1005		//下放料工作使能
#define APP_CONFIG_PID_DOWN_DRI           		0x1006		//下放料方向
#define APP_CONFIG_PID_DOWN_TENSION_POS         0x1007      //下放料张力位置
#define APP_CONFIG_PID_DOWN_JOG_SPEED           0x1008      //下放料点动速度
#define APP_CONFIG_PID_DOWN_PERlIMIT            0x100B      //下放料张力上限位置
#define APP_CONFIG_PID_DOWN_LOWERlIMIT          0x100C      //下放料张力下线位置 
#define APP_CONFIG_PID_DOWN_ENCODERFDIR          0x100E      //下编码器上电默认方向
#define APP_CONFIG_PID_LANGUAGE_MODE		    0x1010 

#define APP_CONFIG_PID_DEV_MODE		            0x100F		//设备模式
#define APP_CONFIG_PID_LANGUAGE_MODE		    0x1010      //语言切换

void APP_CONFIG_load(void);
void APP_CONFIG_save(void);

signed char  APP_AddConfigParameterToFlash(void);

#endif	// _APP_CONFIG_H_



