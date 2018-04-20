#include "searchroad.h"
#include "common.h"
#include "include.h"


float average_offset[11] = { 0,0,0,0,0,0,0,0,0,0,0 };                 //ƫ��ȣ�Ϊƽ��ƫ���
uint8 imgbuff[CAMERA_SIZE];                                         //����洢����ͼ�������
uint8 img[CAMERA_H][CAMERA_W];
int iscross_flag = 0;            //�ж��Ƿ���ʮ�ֵ��ı�־ 0��ʾû�н���1��ʾ�����жϲ���ʼ��������
int iscross_count = 0;           //ʮ�ֵ��������
int isisland_flag = 0;           //�Ƿ��ǻ������ж� 0��ʾû�н���1��ʾ�����жϲ���ʼ��������
int isisland_count = 0;          //�����������
//int rode_wide[CAMERA_H] = {};    //��¼������ȵĶ���




void Search_line_init()
{
	//send_init();
	//motorcontrol_int();

}


void Search_line()
{
	//int black_temp[CAMERA_H];
	int left_black[CAMERA_H];                          //��ߺ�������
	int left_black_before = CAMERA_W / 2;              //��һ�����ɨ�赽�ĺ���λ��
	int left_next_line = 0;                            //�ж�ɨ���Ƿ�������µ�һ��
	int left_find_flag = 0;                            //����Ƿ��ҵ����߱�־												                
	int jw_left;                                       //���������ĵ�ǰ��

	int right_black[CAMERA_H];                         //�ұߺ�������
	int right_black_before = CAMERA_W / 2;
	int right_next_line = 0;
	int right_find_flag = 0;
	int jw_right;

	int jh;                                            //�в���
	int middleline[CAMERA_H] = { 0 };                  //�洢����λ�õ�����
	float offset = 0;                                   //ƫ��ȣ�Ϊ����ƫ���
													   //int slope[CAMERA_H] = { 0 };                       //���ÿ�м����б�ȵ�����
	int count = 0;
	int i = 0;
	int j = 0;

	left_black_before = CAMERA_W / 2;
	right_black_before = CAMERA_W / 2;
	jh = LINE_NUM-1;


	/*-----------------------ɨ�����ص��ȡͼ����Ϣ------------------------*/
	while (jh>=0)                                             //����������������ߣ���ÿһ��ͼ����ж�ɨ��һ��,LINE_NUM��ʾҪɨ������
	{
		/*-----------------���ɨ��--------------------*/
		if (0 == left_next_line)                              //left_black_next������־�Ƿ�����µ�һ�У���Ϊ0���ǽ����µ�һ��
		{
			jw_left = left_black_before;                      //�����µ�һ�п�ʼ����ϴ��ҵ����ߵ�λ�ÿ�ʼ�Һ���
			left_next_line = 1;                               //��־������λ
		}

		if (jw_left>0 && (0 == left_find_flag))               //���ɨ�軹û�е���Ե��֮ǰ��ɨ��δ�ҵ��ڵ�
		{
			if ((img[jh][jw_left]) < 1)
			{
				left_find_flag = 1;                           //ɨ�赽���ߣ���־��1
				left_black[jh] = jw_left;                     //��������Ϊjw_left
				left_black_before = jw_left + OFF_CENTER;     //��һ��ɨ�����һ�κ��ߵ�����ƫ��15�����صĵط���ʼɨ��
				if (left_black_before > CAMERA_W - 1) left_black_before = CAMERA_W - 1;
			}
			jw_left--;                                        //��û��ɨ�赽���߾��ƶ�һ�����ؼ���ɨ��
		}
		if ((0 == jw_left) && (0 == left_find_flag))          //����ҵ����һ��������Ȼδ�ҵ����Ϻ������������أ�����Ϊ��һ��Ϊ����
		{
			if (left_black[jh + 1] != -1 && jh < 50)                        //��ɨ���ж��Ƿ���Բ��
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
			left_black[jh] = -1;                              //-1����ɨ�赽����ʱ��û�ҵ�����
			left_black_before = jw_left + OFF_CENTER;        
		}

		/*-----------------�Ҳ�ɨ��--------------------*/
		if (0 == right_next_line)
		{
			jw_right = right_black_before;
			right_next_line = 1;
		}

		if ((jw_right< (CAMERA_W - 1)) && (0 == right_find_flag))            //��Ϊ��0λҲ�������ص㣬���Դ������ص�����һλ��CAMERA_W-1
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
		if (jw_right == (CAMERA_W - 1) && (0 == right_find_flag))            //�ұߺڵ�δ�ҵ�
		{
			if (right_black[jh + 1] != -1 && jh < 50)                        //��ɨ���ж��Ƿ���Բ��
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

		if ((1 == left_find_flag) && (1 == right_find_flag))           //�����ع�
		{
			left_next_line = 0;                                                
			right_next_line = 0;
			left_find_flag = 0;
			right_find_flag = 0;
			jh--;
		}
	} //while


	/*----------------����ͼ���ж�·��--------------------*/
	//jh = LINE_NUM - 1;
	//while (jh >= 0)   //����·��
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
			
			if (left_black[jh + 0] - left_black[jh + 4] > 6 && left_black[jh + 0] - left_black[jh - 4] > 6)//ͻ���㹻����
			{
				isisland_flag++;
				//isisland_flag = 1;  
			}
			else if (abs((left_black[jh - 4] - left_black[jh])*(left_black[jh + 4] - left_black[jh]) - 16) < 4)  //ͻ��Ϊֱ��
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
		//	if (abs((right_black[jh - 4] - right_black[jh])*(right_black[jh + 4] - right_black[jh]) - 16) < 4)//ͻ��Ϊֱ��
		//	{
		//		iscross_flag++;
		//		//iscross_flag = 1; 
		//	}
		//		
		//}
		j--;
	}


	/*---------------------���ĵ�ļ���------------------------*/
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
	//		/*----------------���ĵ���Ч����ɨ��ֹͣ���ж�------------------*/
	//		//if (abs(middleline[jh + 1] - middleline[jh]) > DEGREE_OF_ERROR)     middleline[jh] = -1;   //����б�ʹ��󣨼�ת�䣩�����ϴ����ĵ���Ч

	//		//else if (0 == img[jh][middleline[jh]] && (jh < 30))
	//		//{
	//		//	middleline[jh] = -2;  //-2����֮��ɨ������
	//		//	break;
	//		//}
	//		//else if (abs(middleline[jh + 1] - middleline[jh]) > REFLECT_LIGHT_ERROR && (jh < REFLECT_LIGHT_AREA))  //����ƫ�����
	//		//{
	//		//	break;
	//		//}
	//		//if ((middleline[jh]< CLOSE_TO_EDGE) || (middleline[jh]>(CAMERA_W - CLOSE_TO_EDGE))) //���߽ӽ�ͼ���Ե,�˳�ɨ��
	//		//{
	//		//	break;
	//		//}
	//	}
	//	jh--;
	//}
		

	/*--------------------------����·���µ�Ԥ������-------------------------*/
	//if (iscross_flag != 0)
	//{

	//}
	//else if (isisland_flag != 0)
	//{

	//}


    /*------------------------ƫ��ȼ��㼰����չ��-----------------------------*/
	//for (i = LINE_NUM - 1; i >= 0; i--)
	//{
	//	if (-2 == middleline[i])
	//		break;
	//	else if (-1 == middleline[i]) {}
	//	else
	//	{
	//		offset = offset + ((float)(middleline[i] - CAMERA_W / 2)*(1 + (60 - i)*TRAPEZOID_CORRECT / 40));          //offset�ǲ�����������������������ƫ��,<0ƫ��
	//		count++;
	//		if (middleline[i] > CAMERA_W - 1)middleline[i] = CAMERA_W - 1;
	//		if (middleline[i] < 0)middleline[i] = 0;
	//		img[i][middleline[i]] = !img[i][middleline[i]];
	//	}
	//}


	///*---------------�����pid---------------------*/
	//for (i = 10; i > 1; i--)
	//{
	//	average_offset[i] = average_offset[i - 1];
	//}
	//average_offset[1] = ((float)offset / (float)count);
	//average_offset[1] -= CAMERA_HARDWARE_ERROR;
}


float Find_slope()       //�ҵ����ε�б��
{
	int j = 79;     //��ǰ��
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


