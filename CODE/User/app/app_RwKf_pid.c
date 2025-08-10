
#include "app_RwKf_pid.h"

PID_Params RwKf_pid;

// PID计算函数
float PID_Compute(PID_Params *pid, float setpoint, float input, float dt) 
{
    float error = setpoint - input;
    
    // 小行程死区补偿
    if (fabsf(error) < 0.3f) {
        pid->integral = 0;
        return 0;
    }
    
    // 比例项
    float proportional = pid->Kp * error;
    
    // 积分项(带抗饱和)
    pid->integral += error * dt;
    if (pid->integral > pid->integral_limit) {
        pid->integral = pid->integral_limit;
    } else if (pid->integral < -pid->integral_limit) {
        pid->integral = -pid->integral_limit;
    }
    float integral = pid->Ki * pid->integral;
    
    // 微分项(带滤波)
    float derivative = (error - pid->prev_error) / dt;
    derivative = 0.7f * pid->prev_derivative + 0.3f * derivative;
    pid->prev_derivative = derivative;
    derivative = pid->Kd * derivative;
    
    pid->prev_error = error;
    
    // 计算输出
    float output = proportional + integral + derivative;
    
    // 输出限幅
    if (output > pid->output_limit) {
        output = pid->output_limit;
    } else if (output < -pid->output_limit) {
        output = -pid->output_limit;
    }
    
    return output;
}



















