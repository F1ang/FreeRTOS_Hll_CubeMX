#include "pid.h"
																							/****普通版PID****/
/**
  * @brief  PID参数初始化
  *	@note 	无
  * @retval 无
  */
void pid_param_init(PID *pid)
{
	/*初始化参数*/
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
  * @brief  设置目标值
  * @param  val		目标值
  *	@note 	无
  * @retval 无
  */
void set_pid_target(PID *pid, float temp_val)
{  
	pid->target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  无
  *	@note 	无
  * @retval 目标值
  */
float get_pid_target(PID *pid)
{
  return pid->target_val;    // 获取当前的目标值
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
  *	@note 	无
  * @retval 无
  */
void set_p_i_d(PID *pid, float p, float i, float d)
{
	pid->Kp = p;    // 设置比例系数 P
	pid->Ki = i;    // 设置积分系数 I
	pid->Kd = d;    // 设置微分系数 D
}

/**
  * @brief  位置PID算法实现
  * @param  actual_val:实际值
  *	@note 	无
  * @retval 通过PID计算后的输出
  */
#define LOC_DEAD_ZONE 60 /*位置环死区*/
#define LOC_INTEGRAL_START_ERR 200 /*积分分离时对应的误差范围*/
#define LOC_INTEGRAL_MAX_VAL 800   /*积分范围限定，防止积分饱和*/
float location_pid_realize(PID *pid, float actual_val)
{
	/*计算目标值与实际值的误差*/
	pid->err = pid->target_val - actual_val;

	/* 设定闭环死区:偏差太小 */
	if((pid->err >= -LOC_DEAD_ZONE) && (pid->err <= LOC_DEAD_ZONE))
	{
		pid->err = 0;
		pid->integral = 0;
		pid->err_last = 0;
	}

	/*积分分离，偏差较大时去掉积分作用:偏差太大*/
	if(pid->err > -LOC_INTEGRAL_START_ERR && pid->err < LOC_INTEGRAL_START_ERR)//积分不分离
	{
		pid->integral += pid->err;  
		/*积分项限幅，防止积分饱和*/
		if(pid->integral > LOC_INTEGRAL_MAX_VAL)
		{
			pid->integral = LOC_INTEGRAL_MAX_VAL;
		}
		else if(pid->integral < -LOC_INTEGRAL_MAX_VAL)
		{
			pid->integral = -LOC_INTEGRAL_MAX_VAL;
		}
	}
	else pid->integral = 0;//积分分离

	/*PID算法实现*/
	pid->output_val = pid->Kp * pid->err +
	                  pid->Ki * pid->integral +
	                  pid->Kd * (pid->err - pid->err_last);

	/*误差传递*/
	pid->err_last = pid->err;

	return pid->output_val;
}

																										/****步兵版PID****/
static float Pid_Limit(float value, float min, float max)
{
	if(value < min)
		return min;
	if(value > max)
		return max;
	
	return value;
}
///*过零处理*/
//static float CorrectingZeroDirection(float err)
//{
//	if(err>4095){return err-8191;}
//	else if(err<-4095){return err+8191;}
//	else {return err;}
//}

//位置式
static void Calc_Position_Pid(Pid_Position_t* pid)
{
	float intergal_spare_k = 1.0f;
	
	pid->err = pid->tar - pid->cur;
	/*积分分离*/
	if(pid->is_integral_spare)
	{
		if(fabs(pid->err) > pid->begin_integral)//大不要
		{
			intergal_spare_k = 0.0f;
		}
		else if(fabs(pid->err) < pid->stop_grow_integral)//小不看
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
	
	/*积分限幅*/
	pid->err_integral = Pid_Limit(pid->err_integral, -pid->max_err_integral, pid->max_err_integral);
	
	pid->p_out = pid->kp * pid->err;
	pid->i_out = pid->ki * pid->err_integral * intergal_spare_k;
	pid->d_out = pid->kd * (pid->err - pid->old_err);
	
	pid->output = pid->p_out + pid->i_out + pid->d_out;
	
	/*输出限幅*/
	pid->output = Pid_Limit(pid->output, -pid->max_out, pid->max_out);
	
	pid->old_err = pid->err;
}

//增量式
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

/*使用位置PID*/
float Pid_Position_Calc(Pid_Position_t* pid, float tar, float cur)
{
	pid->tar = tar;
	pid->cur = cur;
	Calc_Position_Pid(pid);
	return pid->output;
}
/*使用增量PID*/
float Pid_Increment_Calc(Pid_Increment_t* pid, float tar, float cur)
{
	pid->tar = tar;
	pid->cur = cur;
	Calc_Increment_Pid(pid);
	return pid->output;
}



