#include "include.h"
#include "motor.h"
#include "common.h"

int quad_speed[3] = { 0,0,0 };
long quad_value[2] = { 0,0 };
int quad_temp = 0;
int total_distance = 0;



/*********************************************************************************************
�������ܣ������ٶȺ;���
���裺


**********************************************************************************************/
void Update_Motor()
{
	int i = 0;
	int j = 0;
	int distance_temp = 0;



	/*--------------�ƶ��ٶȶ���------------------*/
	for (i = 2; i > 0; i--)
	{
		quad_speed[i] = quad_speed[i - 1];
	}
	quad_value[1] = quad_value[0];


	/*------------��ȡ��������ֵ�������ٶ�--------------*/
	quad_value[0] = quad_val;
	quad_speed[0] = quad_value[0] - quad_value[1];
	quad_temp += (quad_value[0] - quad_value[1]);


	/*------------���־����ϵĿ���----------------*/
	while(quad_temp >= 132)//�Ѿ���ʻ��1cm
	{
		quad_temp -= 132;
		total_distance++;
		distance_temp++;
	}






}