
#include "app_RwKf_pid.h"

PID_Params RwKf_pid;

// PID���㺯��
float PID_Compute(PID_Params *pid, float setpoint, float input, float dt) 
{
    float error = setpoint - input;
    
    // С�г���������
    if (fabsf(error) < 0.3f) {
        pid->integral = 0;
        return 0;
    }
    
    // ������
    float proportional = pid->Kp * error;
    
    // ������(��������)
    pid->integral += error * dt;
    if (pid->integral > pid->integral_limit) {
        pid->integral = pid->integral_limit;
    } else if (pid->integral < -pid->integral_limit) {
        pid->integral = -pid->integral_limit;
    }
    float integral = pid->Ki * pid->integral;
    
    // ΢����(���˲�)
    float derivative = (error - pid->prev_error) / dt;
    derivative = 0.7f * pid->prev_derivative + 0.3f * derivative;
    pid->prev_derivative = derivative;
    derivative = pid->Kd * derivative;
    
    pid->prev_error = error;
    
    // �������
    float output = proportional + integral + derivative;
    
    // ����޷�
    if (output > pid->output_limit) {
        output = pid->output_limit;
    } else if (output < -pid->output_limit) {
        output = -pid->output_limit;
    }
    
    return output;
}



















