#include "pid.h"
#include "include.h"
#include "common.h"


Speed_mode car_mode = OTHER;
float steer_engine_degree = 0;              //舵机的偏转角度，可正可负，正右负左
int motor_speed = 0;

void Control_core()
{

	if (LOW_SPEED == car_mode)//低速模式
	{
		steer_engine_degree = average_offset[1]*4.5;
		if (steer_engine_degree > DEGREE_MAX) steer_engine_degree = DEGREE_MAX;
		if (steer_engine_degree < -DEGREE_MAX) steer_engine_degree = -DEGREE_MAX;
		motor_speed = 100;
	}

	else if (CHECH == car_mode)//调试模式
	{
		Steer_Pid();
		steer_engine_degree = average_offset[0];
		if (steer_engine_degree > DEGREE_MAX) steer_engine_degree = DEGREE_MAX;
		if (steer_engine_degree < -DEGREE_MAX) steer_engine_degree = -DEGREE_MAX;
		motor_speed = 100;
	}	

	else if (OTHER == car_mode)//其他模式
	{
		if (total_distance > 1000)
		{
			motor_speed = 0;
		}
		else
		{
			motor_speed = 150;
		}
	}
}


void Steer_Pid()
{
	float offset_p = 0;
	float offset_i = 0;
	float offset_d = 0;

	offset_p = average_offset[1] - average_offset[2];
	offset_i = average_offset[1];
	offset_d = average_offset[1] - 2 * average_offset[2] + average_offset[3];
	//for (i = 1; i < 6; i++)
	//{
	//	offset_d += average_offset[i];
	//}
	//for (i = 6; i < 11; i++)
	//{
	//	offset_d -= average_offset[i];
	//}

	average_offset[0] = STEER_KP * offset_p + STEER_KI * offset_i + STEER_KD * offset_d;
}

