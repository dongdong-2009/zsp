#ifndef __PID_H__
#define __PID_H__

#include "include.h"
#include "common.h"

#define	DEGREE_MAX    120     //舵机最大偏角
#define M_QUITE_AREA  7     //中速偏差的静态区，将对数据使用静态处理
#define M_BACK1       3.5      //脱离中线的速度为1挡时
#define M_BACK1_SPEED 3      //回中速度为1挡
#define M_BACK2       2
#define M_BACK2_SPEED 2
#define M_BACK_SPEED  1
#define M_BUFF_AREA   12
#define M_BUFF_SPEED  2

#define STEER_KP     3.0 
#define STEER_KI     4.0
#define STEER_KD     3.0


typedef enum Speed_mode
{
	STOP_SPEED,
	LOW_SPEED,
	MIDDLE_SPEED,
	HIGH_SPEED,
	CHECH,
	OTHER
}Speed_mode;

extern Speed_mode car_mode;
extern float steer_engine_degree;        //舵机转动的角度
extern int motor_speed;                //电机转速


extern void Steer_Pid();
void Control_core();





#endif  