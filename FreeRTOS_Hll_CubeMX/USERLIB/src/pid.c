#include "pid.h"
																							/****��ͨ��PID****/
/**
  * @brief  PID������ʼ��
  *	@note 	��
  * @retval ��
  */
void pid_param_init(PID *pid)
{
	/*��ʼ������*/
	pid->target_val = 0.0;				
	pid->output_val = 0.0;
	pid->err = 0.0;
	pid->err_last = 0.0;
	pid->integral = 0.0;

	pid->Kp = 0.05;
	pid->Ki = 0;
	pid->Kd = 0;

}

/**
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
  *	@note 	��
  * @retval ��
  */
void set_pid_target(PID *pid, float temp_val)
{  
	pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
  *	@note 	��
  * @retval Ŀ��ֵ
  */
float get_pid_target(PID *pid)
{
  return pid->target_val;    // ��ȡ��ǰ��Ŀ��ֵ
}

/**
  * @brief  ���ñ��������֡�΢��ϵ��
  * @param  p������ϵ�� P
  * @param  i������ϵ�� i
  * @param  d��΢��ϵ�� d
  *	@note 	��
  * @retval ��
  */
void set_p_i_d(PID *pid, float p, float i, float d)
{
	pid->Kp = p;    // ���ñ���ϵ�� P
	pid->Ki = i;    // ���û���ϵ�� I
	pid->Kd = d;    // ����΢��ϵ�� D
}

/**
  * @brief  λ��PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
#define LOC_DEAD_ZONE 60 /*λ�û�����*/
#define LOC_INTEGRAL_START_ERR 200 /*���ַ���ʱ��Ӧ����Χ*/
#define LOC_INTEGRAL_MAX_VAL 800   /*���ַ�Χ�޶�����ֹ���ֱ���*/
float location_pid_realize(PID *pid, float actual_val)
{
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
	pid->err = pid->target_val - actual_val;

	/* �趨�ջ�����:ƫ��̫С */
	if((pid->err >= -LOC_DEAD_ZONE) && (pid->err <= LOC_DEAD_ZONE))
	{
		pid->err = 0;
		pid->integral = 0;
		pid->err_last = 0;
	}

	/*���ַ��룬ƫ��ϴ�ʱȥ����������:ƫ��̫��*/
	if(pid->err > -LOC_INTEGRAL_START_ERR && pid->err < LOC_INTEGRAL_START_ERR)//���ֲ�����
	{
		pid->integral += pid->err;  
		/*�������޷�����ֹ���ֱ���*/
		if(pid->integral > LOC_INTEGRAL_MAX_VAL)
		{
			pid->integral = LOC_INTEGRAL_MAX_VAL;
		}
		else if(pid->integral < -LOC_INTEGRAL_MAX_VAL)
		{
			pid->integral = -LOC_INTEGRAL_MAX_VAL;
		}
	}
	else pid->integral = 0;//���ַ���

	/*PID�㷨ʵ��*/
	pid->output_val = pid->Kp * pid->err +
	                  pid->Ki * pid->integral +
	                  pid->Kd * (pid->err - pid->err_last);

	/*����*/
	pid->err_last = pid->err;

	return pid->output_val;
}

																										/****������PID****/
static float Pid_Limit(float value, float min, float max)
{
	if(value < min)
		return min;
	if(value > max)
		return max;
	
	return value;
}
///*���㴦��*/
//static float CorrectingZeroDirection(float err)
//{
//	if(err>4095){return err-8191;}
//	else if(err<-4095){return err+8191;}
//	else {return err;}
//}

//λ��ʽ
static void Calc_Position_Pid(Pid_Position_t* pid)
{
	float intergal_spare_k = 1.0f;
	
	pid->err = pid->tar - pid->cur;
	/*���ַ���*/
	if(pid->is_integral_spare)
	{
		if(fabs(pid->err) > pid->begin_integral)//��Ҫ
		{
			intergal_spare_k = 0.0f;
		}
		else if(fabs(pid->err) < pid->stop_grow_integral)//С����
		{
			pid->err_integral += pid->err;
			intergal_spare_k = 1.0f;
		}
		else
		{
			pid->err_integral += pid->err;
			intergal_spare_k = (pid->begin_integral - fabs(pid->err)) / (pid->begin_integral - pid->stop_grow_integral);
		}
	}
	else
	{
		pid->err_integral += pid->err;
	}
	
	/*�����޷�*/
	pid->err_integral = Pid_Limit(pid->err_integral, -pid->max_err_integral, pid->max_err_integral);
	
	pid->p_out = pid->kp * pid->err;
	pid->i_out = pid->ki * pid->err_integral * intergal_spare_k;
	pid->d_out = pid->kd * (pid->err - pid->old_err);
	
	pid->output = pid->p_out + pid->i_out + pid->d_out;
	
	/*����޷�*/
	pid->output = Pid_Limit(pid->output, -pid->max_out, pid->max_out);
	
	pid->old_err = pid->err;
}

//����ʽ
static void Calc_Increment_Pid(Pid_Increment_t* pid)
{
	pid->err = pid->tar - pid->cur;
	
	pid->p_out = pid->kp * (pid->err - pid->old_err);
	pid->i_out = pid->ki * pid->err;
	pid->d_out = pid->kd * (pid->err - 2 * pid->old_err + pid->old_old_err);

	pid->output = pid->p_out + pid->i_out + pid->d_out;
	
	pid->old_old_err = pid->old_err;
	pid->old_err     = pid->err;
}

/*ʹ��λ��PID*/
float Pid_Position_Calc(Pid_Position_t* pid, float tar, float cur)
{
	pid->tar = tar;
	pid->cur = cur;
	Calc_Position_Pid(pid);
	return pid->output;
}
/*ʹ������PID*/
float Pid_Increment_Calc(Pid_Increment_t* pid, float tar, float cur)
{
	pid->tar = tar;
	pid->cur = cur;
	Calc_Increment_Pid(pid);
	return pid->output;
}



