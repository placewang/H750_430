#ifndef __BSP_PID_H
#define	__BSP_PID_H


/*pid*/
typedef struct
{
    float  target_val;   //目标值
	float  actual_val;   //实际值
	float  err;          //定义当前偏差值
	float  err_next;     //定义下一个偏差值
	float  err_last;     //定义上一个偏差值
	float  Kp, Ki, Kd;   //定义比例、积分、微分系数
    float  integral;     //定义积分值
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
