#include "quat.h"



// #define N 20      //滤波缓存数组大小
// #define G			9.80665f		      	// m/s^2
// #define RadtoDeg    57.324841f				//弧度到角度 (弧度 * 180/3.1415)
// #define DegtoRad    0.0174533f				//角度到弧度 (角度 * 3.1415/180)
// #define Acc_Gain  	0.0001220f		  //加速度变成G (初始化加速度满量程-+4g LSBa = 2*4/65535.0)
// #define Gyro_Gr	    0.0010641f		  //角速度变成弧度(3.1415/180 * LSBg) 

// float   accb[3],DCMgb[3][3];                  //方向余弦阵（将 惯性坐标系 转化为 机体坐标系）
// uint8_t AccbUpdate = 0;

// float invSqrt(float x) 
// {
// 	float halfx = 0.5f * x;
// 	float y = x;
// 	long i = *(long*)&y;
// 	i = 0x5f375a86 - (i>>1);
// 	y = *(float*)&i;
// 	y = y * (1.5f - (halfx * y * y));
// 	return y;
// }


// /*********************************************************************************************************
// * 函  数：void IMUupdate(FLOAT_XYZ *Gyr_rad,FLOAT_XYZ *Acc_filt,FLOAT_ANGLE *Att_Angle)
// * 功　能：获取姿态角
// * 参  数：Gyr_rad 	指向角速度的指针（注意单位必须是弧度）
// *         Acc_filt 	指向加速度的指针
// *         Att_Angle 指向姿态角的指针
// * 返回值：无
// * 备  注：求解四元数和欧拉角都在此函数中完成
// **********************************************************************************************************/	
// //kp=ki=0 就是完全相信陀螺仪
// #define Kp 1.50f                         // proportional gain governs rate of convergence to accelerometer/magnetometer
//                                          //比例增益控制加速度计，磁力计的收敛速率
// #define Ki 0.005f                        // integral gain governs rate of convergence of gyroscope biases  
//                                          //积分增益控制陀螺偏差的收敛速度
// #define halfT 0.005f                     // half the sample period 采样周期的一半

// float q0 = 1, q1 = 0, q2 = 0, q3 = 0;     // quaternion elements representing the estimated orientation
// float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error

// void IMUupdate(FLOAT_XYZ *Gyr_rad,FLOAT_XYZ *Acc_filt,FLOAT_ANGLE *Att_Angle)
// {
// 	uint8_t i;
// 	float matrix[9] = {1.f,  0.0f,  0.0f, 0.0f,  1.f,  0.0f, 0.0f,  0.0f,  1.f };//初始化矩阵
// 	float ax = Acc_filt->X,ay = Acc_filt->Y,az = Acc_filt->Z;
// 	float gx = Gyr_rad->X,gy = Gyr_rad->Y,gz = Gyr_rad->Z;
// 	float vx, vy, vz;
// 	float ex, ey, ez;
// 	float norm;

// 	float q0q0 = q0*q0;
// 	float q0q1 = q0*q1;
// 	float q0q2 = q0*q2;
// 	float q0q3 = q0*q3;
// 	float q1q1 = q1*q1;
// 	float q1q2 = q1*q2;
// 	float q1q3 = q1*q3;
// 	float q2q2 = q2*q2;
// 	float q2q3 = q2*q3;
// 	float q3q3 = q3*q3;

// 	if(ax*ay*az==0)
// 	return;

// 	//加速度计测量的重力向量(机体坐标系) 
// 	norm = invSqrt(ax*ax + ay*ay + az*az); 
// 	ax = ax * norm;
// 	ay = ay * norm;
// 	az = az * norm;
// 	//	printf("ax=%0.2f ay=%0.2f az=%0.2f\r\n",ax,ay,az);

// 	//陀螺仪积分估计重力向量(机体坐标系) 
// 	vx = 2*(q1q3 - q0q2);												
// 	vy = 2*(q0q1 + q2q3);
// 	vz = q0q0 - q1q1 - q2q2 + q3q3 ;
// 	// printf("vx=%0.2f vy=%0.2f vz=%0.2f\r\n",vx,vy,vz); 

// 	//测量的重力向量与估算的重力向量差积求出向量间的误差 
// 	ex = (ay*vz - az*vy); //+ (my*wz - mz*wy);                     
// 	ey = (az*vx - ax*vz); //+ (mz*wx - mx*wz);
// 	ez = (ax*vy - ay*vx); //+ (mx*wy - my*wx);

// 	//用上面求出误差进行积分
// 	exInt = exInt + ex * Ki;								 
// 	eyInt = eyInt + ey * Ki;
// 	ezInt = ezInt + ez * Ki;

// 	//将误差PI后补偿到陀螺仪
// 	gx = gx + Kp*ex + exInt;					   		  	
// 	gy = gy + Kp*ey + eyInt;
// 	gz = gz + Kp*ez + ezInt;//这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

// 	//四元素的微分方程
// 	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
// 	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
// 	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
// 	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

// 	//单位化四元数 
// 	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
// 	q0 = q0 * norm;
// 	q1 = q1 * norm;
// 	q2 = q2 * norm;  
// 	q3 = q3 * norm;

// 	//矩阵R 将惯性坐标系(n)转换到机体坐标系(b) 
// 	matrix[0] = q0q0 + q1q1 - q2q2 - q3q3;	// 11(前列后行)
// 	matrix[1] = 2.f * (q1q2 + q0q3);	    // 12
// 	matrix[2] = 2.f * (q1q3 - q0q2);	    // 13
// 	matrix[3] = 2.f * (q1q2 - q0q3);	    // 21
// 	matrix[4] = q0q0 - q1q1 + q2q2 - q3q3;	// 22
// 	matrix[5] = 2.f * (q2q3 + q0q1);	    // 23
// 	matrix[6] = 2.f * (q1q3 + q0q2);	    // 31
// 	matrix[7] = 2.f * (q2q3 - q0q1);	    // 32
// 	matrix[8] = q0q0 - q1q1 - q2q2 + q3q3;	// 33

// 	//四元数转换成欧拉角(Z->Y->X) 
// 	Att_Angle->yaw += Gyr_rad->Z *RadtoDeg*0.01f;     
// //	Att_Angle->yaw = atan2(2.f * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3)* 57.3f; // yaw
// 	Att_Angle->rol = -asin(2.f * (q1q3 - q0q2))* 57.3f;                            // roll(负号要注意) 
// 	Att_Angle->pit = -atan2(2.f * q2q3 + 2.f * q0q1, q0q0 - q1q1 - q2q2 + q3q3)* 57.3f ; // pitch
// 	for(i=0;i<9;i++)
// 	{
// 		*(&(DCMgb[0][0])+i) = matrix[i];
// 	}

// }


