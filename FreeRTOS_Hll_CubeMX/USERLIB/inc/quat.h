/*
 * @Description: 
 * @Author: YiPei_Fang
 * @Date: 2022-09-26 18:04:15
 */
#ifndef _QUAT_H_
#define _QUAT_H_


#include "main.h"
#include <math.h>

//锟斤拷锟斤拷锟斤拷锟酵ｏ拷MPU6050原始锟斤拷锟捷ｏ拷
typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}INT16_XYZ;
//锟斤拷锟结浮锟斤拷锟斤拷
typedef struct
{
	float X;
	float Y;
	float Z;
}FLOAT_XYZ;
//锟斤拷态锟斤拷锟斤拷锟侥角讹拷
typedef struct
{
	float rol;
	float pit;
	float yaw;
}FLOAT_ANGLE;




#endif



