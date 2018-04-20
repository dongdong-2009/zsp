#include "include.h"
#include "motor.h"
#include "common.h"

int quad_speed[3] = { 0,0,0 };
long quad_value[2] = { 0,0 };
int quad_temp = 0;
int total_distance = 0;



/*********************************************************************************************
函数功能：更新速度和距离
步骤：


**********************************************************************************************/
void Update_Motor()
{
	int i = 0;
	int j = 0;
	int distance_temp = 0;



	/*--------------移动速度队列------------------*/
	for (i = 2; i > 0; i--)
	{
		quad_speed[i] = quad_speed[i - 1];
	}
	quad_value[1] = quad_value[0];


	/*------------获取编码器的值并更新速度--------------*/
	quad_value[0] = quad_val;
	quad_speed[0] = quad_value[0] - quad_value[1];
	quad_temp += (quad_value[0] - quad_value[1]);


	/*------------各种距离上的控制----------------*/
	while(quad_temp >= 132)//已经行驶的1cm
	{
		quad_temp -= 132;
		total_distance++;
		distance_temp++;
	}






}