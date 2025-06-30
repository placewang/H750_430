#ifndef __BSP_PID_H
#define	__BSP_PID_H


/*pid*/
typedef struct
{
    float  target_val;   //Ŀ��ֵ
	float  actual_val;   //ʵ��ֵ
	float  err;          //���嵱ǰƫ��ֵ
	float  err_next;     //������һ��ƫ��ֵ
	float  err_last;     //������һ��ƫ��ֵ
	float  Kp, Ki, Kd;   //������������֡�΢��ϵ��
    float  integral;     //�������ֵ
}_pid;
 
extern _pid DownMove_pid,UpMove_pid;
extern signed int get_pid_actual(_pid *pid);
extern void time_period_fun(void);

void PID_param_init(char swt);
signed int PID_realize(_pid *, signed int);
void PID_set_err(_pid *pid, float actual_val,float integral);
void set_pid_target(_pid *pid, float temp_val);
void set_pid_actual(_pid *pid, float temp_val);
void set_p_i_d(_pid *pid, float p, float i, float d);

#endif
