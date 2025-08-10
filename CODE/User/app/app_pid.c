
/* Includes ------------------------------------------------------------------*/
#include "app_pid.h"

/* 定义全局变量 */
_pid DownMove_pid,UpMove_pid;

/**
  * @brief  PID参数初始化
  swt: 1：上放料 2下放料  0:全部
  * @retval 无
  */
void PID_param_init(char swt)
{
  if(swt==1||swt==0)
  {      
      /* 初始化位置环PID参数 */
      UpMove_pid.target_val=0;				
      UpMove_pid.actual_val=0;
      UpMove_pid.err = 0;
      UpMove_pid.err_last = 0;
      UpMove_pid.err_next = 0;
      UpMove_pid.Kp = 0;
      UpMove_pid.Ki = 0;
      UpMove_pid.Kd = 0;	
      UpMove_pid.integral=0;	
  }
  if(swt==2||swt==0)
  {       
	/* 初始化速度环PID参数 */
    DownMove_pid.target_val=0;				
    DownMove_pid.actual_val=0;
    DownMove_pid.err = 0;
    DownMove_pid.err_last = 0;
    DownMove_pid.err_next = 0;
    DownMove_pid.Kp = 0;
    DownMove_pid.Ki = 0;
    DownMove_pid.Kd = 0;
    DownMove_pid.integral=0;    
  }
}
/**
  * @brief  设置目标值
  * @param  val		目标值
  *	@note 	无
  * @retval 无
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // 设置当前的目标值
}
/**
  * @brief  设置实际值
  * @param  val		目标值
  *	@note 	无
  * @retval 无
  */
void set_pid_actual(_pid *pid, float temp_val)
{
  pid->actual_val = temp_val;    // 设置当前的目标值
}


/**
  * @brief  获取目标值
  * @param  无
	*	@note 	无
  * @retval 目标值
  */
signed int get_pid_actual(_pid *pid)
{
  return pid->target_val;    // 设置当前的目标值
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
	*	@note 	无
  * @retval 无
  */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // 设置比例系数 P
	pid->Ki = i;    // 设置积分系数 I
	pid->Kd = d;    // 设置微分系数 D
}
/**
设置并取反: 当前偏差值,
           下一个偏差值,
           上一个偏差值,
           积分值
  */
void PID_set_err(_pid *pid, float actual_val,float integral)
{
 
    float integral_val=0.0;
    pid->actual_val=actual_val;
    
    pid->Ki=pid->Ki==0?1:pid->Ki;
    integral_val=integral*(1/pid->Ki);

    pid->integral=integral_val>0?-integral_val:integral_val;  
}


/**
  * @brief  增量式PID算法实现
  * @param  val：当前实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
//signed int PID_realize(_pid *pid, signed int temp_val) 
//{
//	/*传入实际值*/
//	pid->actual_val = temp_val;
//	/*计算目标值与实际值的误差*/
//    pid->err=pid->target_val-pid->actual_val;

//	/*PID算法实现*/
//	signed int increment_val = pid->Kp*(pid->err) + pid->Ki*pid->err + pid->Kd*(pid->err - 2 * pid->err_next + pid->err_last);
//	/*传递误差*/
//	pid->err_last = pid->err_next;
//	pid->err_next = pid->err;
//	/*返回增量值*/
//	return increment_val;
//}

/**
  * @brief  位置式PID算法实现
  * @param  val：当前实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
signed int PID_realize(_pid *pid, signed int actual_val) 
{
  /*传入实际值*/
  pid->actual_val = actual_val;
  /*计算目标值与实际值的误差*/
  pid->err = pid->target_val - pid->actual_val;

  /*误差累积*/
  pid->integral+= pid->err;
  if((pid->integral>100)){
      pid->integral = 100.0;
  }       
  /*PID算法实现*/
  pid->actual_val = pid->Kp*pid->err+ pid->Ki*pid->integral+ pid->Kd*(pid->err-pid->err_last);
  /*误差传递*/
  pid->err_last = pid->err;
  /*PID算法实现，并返回计算值*/
   /*对翰出限幅*/
  if((pid->actual_val>-100.0)){
      pid->actual_val =-100.0;
  }  
  if((pid->actual_val<-20000)){
      pid->actual_val =-20000.0;
  }       
  return pid->actual_val;
}

