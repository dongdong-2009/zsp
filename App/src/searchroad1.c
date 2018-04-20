#include "searchroad.h"

#include "common.h"
#include "include.h"

      
float average_slope = 0;
float average_offset = 0;                          //偏差度，为平均偏差度


void Search_line_init()
{
	//send_init();
	//motorcontrol_int();
}


void Search_line()
{
	int left_black[CAMERA_H];                          //左边黑线数组
	int left_black_before = CAMERA_W / 2;              //上一次左边扫描到的黑线位置
	int left_next_line = 0;                            //判断扫描是否进入了新的一行
	int left_find_flag = 0;                            //左边是否找到黑线标志
	int left_slope[CAMERA_H] = { 0 };                  //左偏差度
	int jw_left;                                       //向左搜索的当前列

	int right_black[CAMERA_H];                         //右边黑线数组
	int right_black_before = CAMERA_W / 2;
	int right_next_line = 0;
	int right_find_flag = 0;
	int right_slope[CAMERA_H] = { 0 };       
	int jw_right;

	int jh;                                            //行参数
	int middleline[CAMERA_H] = { 0 };                  //存储中线位置的数组
	int offset = 0;                                    //偏差度，为整体偏差度
	int slope[CAMERA_H] = { 0 };                       //存放每行间黑线斜度的数组
	int count = 0;
	int sumslope = 0;                                  //斜度的总和   

	average_slope = 0;
	average_offset = 0;
	left_black_before = CAMERA_W / 2;
	right_black_before = CAMERA_W / 2;
	jh = (CAMERA_H - BLANK_LINE_NUM);                         //BLANK_LINE_NUM为10,表示在最上方无效的行的数目

	while (jh>(CAMERA_H - LINE_NUM - BLANK_LINE_NUM))         //行条件满足进入找线，将每一有图像的行都扫描一遍,LINE_NUM表示要扫描行数
	{
		if (0 == left_next_line)                              //left_black_next用来标志是否进入新的一行，若为0则是进入新的一行
		{
			jw_left = left_black_before;                      //若是新的一行开始则从上次找到黑线的位置开始找黑线
			left_next_line = 1;                               //标志参数归位
		}
			
		if (jw_left>0 && (0 == left_find_flag))               //如果扫描还没有到边缘且之前的扫描未找到黑点
		{
			//if (1> (img[jh][jw_left] + img[jh][jw_left-1]))  //黑点为0，flag为0是没找到。若有连续出现3个黑点则视为找到黑线
			if ((img[jh][jw_left]) < 1)
			{
				left_find_flag = 1;                           //扫描到黑线，标志置1
				left_black[jh] = jw_left;                     //黑线中心为jw_left
				left_black_before = jw_left + OFF_CENTER;     //下一次扫描从上一次黑线的中心偏离15个像素的地方开始扫描
			}
			jw_left--;                                        //若没有扫描到黑线就移动一个像素继续扫描
		}
		if ((0 == jw_left) && (0 == left_find_flag))          //左边找到最后一个像素任然未找到符合黑线条件的像素，则认为第一个为黑线
		{
			left_find_flag = 1;                       
			left_black[jh] = jw_left;
			left_black_before = jw_left + OFF_CENTER+20;
		}

		/*对右侧的扫描找黑点*/
		if (0 == right_next_line)
		{
			jw_right = right_black_before;
			right_next_line = 1;
		}
			
		if ((jw_right< (CAMERA_W - 1)) && (0 == right_find_flag))            //因为第0位也存有像素点，所以存有像素点的最后一位是CAMERA_W-1
		{
			//if ( (img[jh][jw_right]+ img[jh][jw_right+1])<1)                        //gai
			if ((img[jh][jw_right])<1)
			{
				right_find_flag = 1;
				right_black[jh] = jw_right;
				right_black_before = jw_right - OFF_CENTER-20;
			}
			jw_right++;
		}
		if (jw_right == (CAMERA_W - 1) && (0 == right_find_flag))            //右边黑点未找到
		{
			right_find_flag = 1;
			right_black[jh] = jw_right;
			right_black_before = jw_right - OFF_CENTER;
		}


		if ((1 == left_find_flag) && (1 == right_find_flag))                 //两边黑线都找到时开始计算，这里用if时因为一边找到黑线不能进入计算，要两边都找到
		{

			if (jh == (CAMERA_H - BLANK_LINE_NUM))
				middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
			if (jh < (CAMERA_H - BLANK_LINE_NUM))
			{
				//right_slope[jh] = right_black[jh] - right_black[jh + 1];
				//left_slope[jh] = left_black[jh] - left_black[jh + 1];
				//middleline[jh] = middleline[jh + 1] + right_slope[jh] + left_slope[jh];     //中线偏差？寻找中线
				middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
				//img[jh][middleline[jh]] = !img[jh][middleline[jh]];

				if (middleline[jh + 1] - middleline[jh] > DEGREE_OF_ERROR || middleline[jh + 1] - middleline[jh] < -DEGREE_OF_ERROR) {} //若果斜率过大（急转弯）或误差较大，flag置1

				else
				{
					slope[jh] = (middleline[jh + 1] - middleline[jh]);               //斜率
					sumslope += slope[jh];
					offset = offset - (CAMERA_W / 2 - middleline[jh]);             //offset是补偿，用来描述整体赛道的偏向
					count++;
					img[jh][middleline[jh]] = !img[jh][middleline[jh]];
				}
			}

			if ((middleline[jh]< CLOSE_TO_EDGE) || (middleline[jh]>(CAMERA_W - CLOSE_TO_EDGE))) //黑线接近图像边缘,退出扫描
			{
				jh = (CAMERA_H - LINE_NUM - BLANK_LINE_NUM + 1);
			}

			left_next_line = 0;                                                 //参数回归
			right_next_line = 0;
			left_find_flag = 0;
			right_find_flag = 0;
			jh--;                                                             //进入下一行的扫描
		} //if  
	} //while

	average_slope = (sumslope * 100) / count;
	average_offset = (offset / count);
}