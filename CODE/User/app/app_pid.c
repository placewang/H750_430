
/* Includes ------------------------------------------------------------------*/
#include "app_pid.h"

/* ����ȫ�ֱ��� */
_pid DownMove_pid,UpMove_pid;

/**
  * @brief  PID������ʼ��
  swt: 1���Ϸ��� 2�·���  0:ȫ��
  * @retval ��
  */
void PID_param_init(char swt)
{
  if(swt==1||swt==0)
  {      
      /* ��ʼ��λ�û�PID���� */
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
	/* ��ʼ���ٶȻ�PID���� */
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
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
  *	@note 	��
  * @retval ��
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}
/**
  * @brief  ����ʵ��ֵ
  * @param  val		Ŀ��ֵ
  *	@note 	��
  * @retval ��
  */
void set_pid_actual(_pid *pid, float temp_val)
{
  pid->actual_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}


/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
	*	@note 	��
  * @retval Ŀ��ֵ
  */
signed int get_pid_actual(_pid *pid)
{
  return pid->target_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ���ñ��������֡�΢��ϵ��
  * @param  p������ϵ�� P
  * @param  i������ϵ�� i
  * @param  d��΢��ϵ�� d
	*	@note 	��
  * @retval ��
  */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // ���ñ���ϵ�� P
	pid->Ki = i;    // ���û���ϵ�� I
	pid->Kd = d;    // ����΢��ϵ�� D
}
/**
���ò�ȡ��: ��ǰƫ��ֵ,
           ��һ��ƫ��ֵ,
           ��һ��ƫ��ֵ,
           ����ֵ
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
  * @brief  ����ʽPID�㷨ʵ��
  * @param  val����ǰʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
//signed int PID_realize(_pid *pid, signed int temp_val) 
//{
//	/*����ʵ��ֵ*/
//	pid->actual_val = temp_val;
//	/*����Ŀ��ֵ��ʵ��ֵ�����*/
//    pid->err=pid->target_val-pid->actual_val;

//	/*PID�㷨ʵ��*/
//	signed int increment_val = pid->Kp*(pid->err) + pid->Ki*pid->err + pid->Kd*(pid->err - 2 * pid->err_next + pid->err_last);
//	/*�������*/
//	pid->err_last = pid->err_next;
//	pid->err_next = pid->err;
//	/*��������ֵ*/
//	return increment_val;
//}

/**
  * @brief  λ��ʽPID�㷨ʵ��
  * @param  val����ǰʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
signed int PID_realize(_pid *pid, signed int actual_val) 
{
  /*����ʵ��ֵ*/
  pid->actual_val = actual_val;
  /*����Ŀ��ֵ��ʵ��ֵ�����*/
  pid->err = pid->target_val - pid->actual_val;

  /*����ۻ�*/
  pid->integral+= pid->err;
  if((pid->integral>100)){
      pid->integral = 100.0;
  }       
  /*PID�㷨ʵ��*/
  pid->actual_val = pid->Kp*pid->err+ pid->Ki*pid->integral+ pid->Kd*(pid->err-pid->err_last);
  /*����*/
  pid->err_last = pid->err;
  /*PID�㷨ʵ�֣������ؼ���ֵ*/
   /*�Ժ����޷�*/
  if((pid->actual_val>-100.0)){
      pid->actual_val =-100.0;
  }  
  if((pid->actual_val<-20000)){
      pid->actual_val =-20000.0;
  }       
  return pid->actual_val;
}

