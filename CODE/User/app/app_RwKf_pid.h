#ifndef _APP_RwKfPID_H
#define	_APP_RwKfPID_H

#include "stm32h7xx_hal.h"
#include <math.h>

// 系统参数配置
#define MAX_MOTOR_SPEED         15000   // RPM
#define ENCODER_RESOLUTION      2000    // 编码器线数(4倍频后)
#define CONTROL_PERIOD_MS       1       // 控制周期1ms
#define SMALL_MOVE_THRESHOLD    3       // 小行程阈值(脉冲数)
#define SMALL_MOVE_SPEED_RATIO  0.2f    // 小行程速度比例

// PID参数结构体
typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float integral;
    float prev_error;
    float prev_derivative;
    float output_limit;
    float integral_limit;
} PID_Params;


extern PID_Params RowKnife_pid;


float PID_Compute(PID_Params *pid, float setpoint, float input, float dt); 

#endif
