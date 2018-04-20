#include "searchroad.h"
#include "common.h"
#include "include.h"


float average_offset[11] = { 0,0,0,0,0,0,0,0,0,0,0 };                 //偏差度，为平均偏差度
uint8 imgbuff[CAMERA_SIZE];                                         //定义存储接收图像的数组
uint8 img[CAMERA_H][CAMERA_W];
int iscross_flag = 0;            //判断是否是十字道的标志 0表示没有进入1表示初步判断并开始计数距离
int iscross_count = 0;           //十字道距离计数
int isisland_flag = 0;           //是否是环岛的判断 0表示没有进入1表示初步判断并开始计数距离
int isisland_count = 0;          //环岛距离计数
//int rode_wide[CAMERA_H] = {};    //记录赛道宽度的队列




void Search_line_init()
{
	//send_init();
	//motorcontrol_int();

}


void Search_line()
{
	//int black_temp[CAMERA_H];
	int left_black[CAMERA_H];                          //左边黑线数组
	int left_black_before = CAMERA_W / 2;              //上一次左边扫描到的黑线位置
	int left_next_line = 0;                            //判断扫描是否进入了新的一行
	int left_find_flag = 0;                            //左边是否找到黑线标志												                
	int jw_left;                                       //向左搜索的当前列

	int right_black[CAMERA_H];                         //右边黑线数组
	int right_black_before = CAMERA_W / 2;
	int right_next_line = 0;
	int right_find_flag = 0;
	int jw_right;

	int jh;                                            //行参数
	int middleline[CAMERA_H] = { 0 };                  //存储中线位置的数组
	float offset = 0;                                   //偏差度，为整体偏差度
													   //int slope[CAMERA_H] = { 0 };                       //存放每行间黑线斜度的数组
	int count = 0;
	int i = 0;
	int j = 0;

	left_black_before = CAMERA_W / 2;
	right_black_before = CAMERA_W / 2;
	jh = LINE_NUM-1;


	/*-----------------------扫描像素点获取图像信息------------------------*/
	while (jh>=0)                                             //行条件满足进入找线，将每一有图像的行都扫描一遍,LINE_NUM表示要扫描行数
	{
		/*-----------------左侧扫描--------------------*/
		if (0 == left_next_line)                              //left_black_next用来标志是否进入新的一行，若为0则是进入新的一行
		{
			jw_left = left_black_before;                      //若是新的一行开始则从上次找到黑线的位置开始找黑线
			left_next_line = 1;                               //标志参数归位
		}

		if (jw_left>0 && (0 == left_find_flag))               //如果扫描还没有到边缘且之前的扫描未找到黑点
		{
			if ((img[jh][jw_left]) < 1)
			{
				left_find_flag = 1;                           //扫描到黑线，标志置1
				left_black[jh] = jw_left;                     //黑线中心为jw_left
				left_black_before = jw_left + OFF_CENTER;     //下一次扫描从上一次黑线的中心偏离15个像素的地方开始扫描
				if (left_black_before > CAMERA_W - 1) left_black_before = CAMERA_W - 1;
			}
			jw_left--;                                        //若没有扫描到黑线就移动一个像素继续扫描
		}
		if ((0 == jw_left) && (0 == left_find_flag))          //左边找到最后一个像素任然未找到符合黑线条件的像素，则认为第一个为黑线
		{
			if (left_black[jh + 1] != -1 && jh < 50)                        //补扫，判断是否是圆环
			{
				for (i = 2; i < 8; i++)
				{
					j = left_black[jh + i] - 1;
					while (img[jh + i][j] != 1 && j > 0)
					{
						j--;
					}
					if (0 == j) break;
				}
				if (7 == i)
				{
					isisland_flag++;
					//isisland_flag = 1;
				}
			}
			left_find_flag = 1;
			left_black[jh] = -1;                              //-1代表扫描到边线时还没找到黑线
			left_black_before = jw_left + OFF_CENTER;        
		}

		/*-----------------右侧扫描--------------------*/
		if (0 == right_next_line)
		{
			jw_right = right_black_before;
			right_next_line = 1;
		}

		if ((jw_right< (CAMERA_W - 1)) && (0 == right_find_flag))            //因为第0位也存有像素点，所以存有像素点的最后一位是CAMERA_W-1
		{           
			if ((img[jh][jw_right])<1)
			{
				right_find_flag = 1;
				right_black[jh] = jw_right;
				right_black_before = jw_right - OFF_CENTER;
				if (right_black_before < 0) right_black_before = 0;
			}
			jw_right++;
		}
		if (jw_right == (CAMERA_W - 1) && (0 == right_find_flag))            //右边黑点未找到
		{
			if (right_black[jh + 1] != -1 && jh < 50)                        //补扫，判断是否是圆环
			{
				for (i = 2; i < 8; i++)
				{
					j = right_black[jh + i] + 1;
					while (img[jh + i][j] != 1 && j < CAMERA_W - 1)
					{
						j++;
					}
					if ((CAMERA_W - 1) == j) break;
				}
				if (7 == i)
				{
					isisland_flag++;
					//isisland_flag = 1;
				}
			}
			right_find_flag = 1;
			right_black[jh] = -1;
			right_black_before = jw_right - OFF_CENTER;
		}	

		if ((1 == left_find_flag) && (1 == right_find_flag))           //参数回归
		{
			left_next_line = 0;                                                
			right_next_line = 0;
			left_find_flag = 0;
			right_find_flag = 0;
			jh--;
		}
	} //while


	/*----------------根据图像判断路况--------------------*/
	//jh = LINE_NUM - 1;
	//while (jh >= 0)   //计算路宽
	//{
	//	if (left_black[jh] != -1 && right_black[jh] != -1)
	//	{
	//		rode_wide[jh] = right_black[jh] - left_black[jh];
	//	}
	//	else if (left_black[jh] != -1 && -1 == right_black[jh])
	//	{
	//		rode_wide[jh] = CAMERA_W - left_black[jh];
	//	}
	//	else if (right_black[jh] != -1 && -1 == left_black[jh])
	//	{
	//		rode_wide[jh] = right_black[jh];
	//	}
	//	else
	//	{
	//		rode_wide[jh] = CAMERA_W;
	//	}
	//	j--;
	//}

	jh = LINE_NUM - 8;
	while (jh >= 8)
	{
		if ((left_black[jh + 3] - left_black[jh + 4] > 0 &&
			left_black[jh + 2] - left_black[jh + 3] > 0 &&
			left_black[jh + 1] - left_black[jh + 2] > 0 &&
			left_black[jh + 0] - left_black[jh + 1] > 0 &&
			left_black[jh - 1] - left_black[jh - 0] < 0 &&
			left_black[jh - 2] - left_black[jh - 1] < 0 &&
			left_black[jh - 3] - left_black[jh - 2] < 0 &&
			left_black[jh - 4] - left_black[jh - 3] < 0)
			)
		{
			
			if (left_black[jh + 0] - left_black[jh + 4] > 6 && left_black[jh + 0] - left_black[jh - 4] > 6)//突出点够尖锐
			{
				isisland_flag++;
				//isisland_flag = 1;  
			}
			else if (abs((left_black[jh - 4] - left_black[jh])*(left_black[jh + 4] - left_black[jh]) - 16) < 4)  //突出为直角
			{
				iscross_flag++;
				//iscross_flag = 1;
			}
		}
		//else if (
		//	    right_black[jh + 3] - right_black[jh + 4] < 0 &&
		//		right_black[jh + 2] - right_black[jh + 3] < 0 &&
		//		right_black[jh + 1] - right_black[jh + 2] < 0 &&
		//		right_black[jh + 0] - right_black[jh + 1] < 0 &&
		//		right_black[jh - 1] - right_black[jh - 0]>0 &&
		//		right_black[jh - 2] - right_black[jh - 1]>0 &&
		//		right_black[jh - 3] - right_black[jh - 2]>0 &&
		//		right_black[jh - 4] - right_black[jh - 3]>0
		//	    )
		//{
		//	if (abs((right_black[jh - 4] - right_black[jh])*(right_black[jh + 4] - right_black[jh]) - 16) < 4)//突出为直角
		//	{
		//		iscross_flag++;
		//		//iscross_flag = 1; 
		//	}
		//		
		//}
		j--;
	}


	/*---------------------中心点的计算------------------------*/
	//jh = LINE_NUM - 1;
	//while (jh >= 0)
	//{
	//	//if (-1 == left_black[jh]) left_black[jh] = 0;
	//	//if (-1 == right_black[jh]) right_black[jh] = LINE_NUM - 1;

	//	if (jh == (LINE_NUM-1))
	//		middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
	//	else
	//	{
	//		if (-1 == left_black[jh] || -1 == right_black[jh])
	//		{
	//			if (-1 == right_black[jh]) middleline[jh] = left_black[jh] + 36 / (1 + (60 - jh)*TRAPEZOID_CORRECT / 40);
	//			else middleline[jh] = right_black[jh] - 36 / (1 + (60 - jh)*TRAPEZOID_CORRECT / 40);
	//		}
	//		else
	//		{
	//			middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
	//		}
	//			
	//		/*----------------中心点无效或者扫描停止的判断------------------*/
	//		//if (abs(middleline[jh + 1] - middleline[jh]) > DEGREE_OF_ERROR)     middleline[jh] = -1;   //若果斜率过大（急转弯）或误差较大中心点无效

	//		//else if (0 == img[jh][middleline[jh]] && (jh < 30))
	//		//{
	//		//	middleline[jh] = -2;  //-2代表之后扫描作废
	//		//	break;
	//		//}
	//		//else if (abs(middleline[jh + 1] - middleline[jh]) > REFLECT_LIGHT_ERROR && (jh < REFLECT_LIGHT_AREA))  //反光偏差过大
	//		//{
	//		//	break;
	//		//}
	//		//if ((middleline[jh]< CLOSE_TO_EDGE) || (middleline[jh]>(CAMERA_W - CLOSE_TO_EDGE))) //黑线接近图像边缘,退出扫描
	//		//{
	//		//	break;
	//		//}
	//	}
	//	jh--;
	//}
		

	/*--------------------------各种路况下的预备处理-------------------------*/
	//if (iscross_flag != 0)
	//{

	//}
	//else if (isisland_flag != 0)
	//{

	//}


    /*------------------------偏差度计算及中线展现-----------------------------*/
	//for (i = LINE_NUM - 1; i >= 0; i--)
	//{
	//	if (-2 == middleline[i])
	//		break;
	//	else if (-1 == middleline[i]) {}
	//	else
	//	{
	//		offset = offset + ((float)(middleline[i] - CAMERA_W / 2)*(1 + (60 - i)*TRAPEZOID_CORRECT / 40));          //offset是补偿，用来描述整体赛道的偏向,<0偏左
	//		count++;
	//		if (middleline[i] > CAMERA_W - 1)middleline[i] = CAMERA_W - 1;
	//		if (middleline[i] < 0)middleline[i] = 0;
	//		img[i][middleline[i]] = !img[i][middleline[i]];
	//	}
	//}


	///*---------------舵机的pid---------------------*/
	//for (i = 10; i > 1; i--)
	//{
	//	average_offset[i] = average_offset[i - 1];
	//}
	//average_offset[1] = ((float)offset / (float)count);
	//average_offset[1] -= CAMERA_HARDWARE_ERROR;
}


float Find_slope()       //找到梯形的斜率
{
	int j = 79;     //当前列
	int h[3] = { 2,3,4 };

	while (0 == img[50][j] && j >= 0)
	{	
		j--;
	}
	h[1] = j;

	j = 79;
	while (0 == img[10][j])
	{
		j--;
	}
	h[2] = j;

	LCD_num(tem_site_str[3], h[1], GREEN, BLUE);
	LCD_num(tem_site_str[4], h[2], GREEN, BLUE);
	//LCD_numf(tem_site_data[4], h[1]-h[2], GREEN, BLUE);

	return (float)(abs(h[1] - h[2])) / 40.0;
}


void Negation()
{
	int i = 0;
	int j = 0;

	for (i = 0; i <CAMERA_H; i++)
	{
		for (j = 0; j <CAMERA_W; j++)
		{
			img[i][j] = !img[i][j];
		}
	}
}


