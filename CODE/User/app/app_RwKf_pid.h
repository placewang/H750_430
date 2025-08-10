#ifndef _APP_RwKfPID_H
#define	_APP_RwKfPID_H

#include "stm32h7xx_hal.h"
#include <math.h>

// ϵͳ��������
#define MAX_MOTOR_SPEED         15000   // RPM
#define ENCODER_RESOLUTION      2000    // ����������(4��Ƶ��)
#define CONTROL_PERIOD_MS       1       // ��������1ms
#define SMALL_MOVE_THRESHOLD    3       // С�г���ֵ(������)
#define SMALL_MOVE_SPEED_RATIO  0.2f    // С�г��ٶȱ���

// PID�����ṹ��
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
