#ifndef _PID_H_
#define _PID_H_

#include "main.h"
#include <math.h>
															/****普通版PID****/
typedef struct
{
	float target_val;   //目标值
	float err;          //偏差值
	float err_last;     //上一个偏差值
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float integral;     //积分值
	float output_val;   //输出值

   // float output_max_val;   //输出值
}PID;

void pid_param_init(PID *pid);//初始化参数
void  set_pid_target(PID *pid, float temp_val);//设定目标值
float get_pid_target(PID *pid);//获取目标值

void  set_p_i_d(PID *pid, float p, float i, float d);//调参
float location_pid_realize(PID *pid, float actual_val);//位置式

															/****步兵版PID****/
#define NEW_POSITION_PID(p,i,d,limit_i,limit_out,is_i_spare,begin_i,stop_grow_i) {\
	.kp                     = p,\
	.ki                     = i,\
	.kd                     = d,\
	.tar                    = 0,\
	.cur                    = 0,\
	.err                    = 0,\
	.old_err                = 0,\
	.err_integral           = 0,\
	.is_integral_spare      = is_i_spare,\
	.begin_integral         = begin_i,\
	.stop_grow_integral     = stop_grow_i,\
	.p_out                  = 0,\
	.i_out                  = 0,\
	.d_out                  = 0,\
	.output                 = 0,\
	.max_err_integral       = limit_i,\
	.max_out                = limit_out\
}

#define NEW_INCREMENT_PID(p,i,d,limit) {\
	.kp                     = p,\
	.ki                     = i,\
	.kd                     = d,\
	.tar                    = 0,\
	.cur                    = 0,\
	.err                    = 0,\
	.old_err                = 0,\
	.old_old_err            = 0,\
	.p_out                  = 0,\
	.i_out                  = 0,\
	.d_out                  = 0,\
	.output                 = 0,\
	.max_out                = limit\
}

typedef struct
{
	float kp;
	float ki;
	float kd;
	
	float tar;
	float cur;
	
	float err;          //误差值
	float old_err;      //上次误差值
	float err_integral; //积分值
	
	uint8_t    is_integral_spare;      //选择是否使用变积分分离
	//此时的积分系数为ki*index
	float begin_integral;         //当误差大于begin_integral时，不积分
	float stop_grow_integral;     //当误差小于stop_grow_integral时，积分系数处于最大值
	
	float max_err_integral;//积分限幅----max_out/20
	float max_out;  //最大输出
	
	float p_out;
	float i_out;
	float d_out;
	float output;
	
}Pid_Position_t;

typedef struct
{
	//PID 三参数
	float kp;
	float ki;
	float kd;
	
	float tar;
	float cur;
	
	float err;
	float old_err;
	float old_old_err;
	
	float max_out;  //最大输出
	
	float p_out;
	float i_out;
	float d_out;
	
	float output;
	
}Pid_Increment_t;

float Pid_Position_Calc(Pid_Position_t* pid, float tar, float cur);
float Pid_Increment_Calc(Pid_Increment_t* pid, float tar, float cur);

#endif




