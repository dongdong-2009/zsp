#include "searchroad.h"
#include "common.h"
#include "include.h"


float average_offset[11] = { 0,0,0,0,0,0,0,0,0,0,0 };                 //ƫ��ȣ�Ϊƽ��ƫ���
//float average_offset_before = 0;                                  //��һ�ε�ƽ��ƫ���  
uint8 imgbuff[CAMERA_SIZE];                                         //����洢����ͼ�������
uint8 img[CAMERA_H][CAMERA_W];
int iscross_flag = 0;
int iscross_count = 0;
int cross_begin = 0;


void Search_line_init()
{
	//send_init();
	//motorcontrol_int();

}


void Search_line()
{
	int left_black[CAMERA_H];                          //��ߺ�������
	int left_black_before = CAMERA_W / 2;              //��һ�����ɨ�赽�ĺ���λ��
	int left_next_line = 0;                            //�ж�ɨ���Ƿ�������µ�һ��
	int left_find_flag = 0;                            //����Ƿ��ҵ����߱�־												                
	int jw_left;                                       //���������ĵ�ǰ��
	int left_end_flag = 0;                             //���û��ɨ����

	int right_black[CAMERA_H];                         //�ұߺ�������
	int right_black_before = CAMERA_W / 2;
	int right_next_line = 0;
	int right_find_flag = 0;
	int jw_right;
	int right_end_flag = 0;

	int jh;                                            //�в���
	int middleline[CAMERA_H] = { 0 };                  //�洢����λ�õ�����
	float offset = 0;                                   //ƫ��ȣ�Ϊ����ƫ���
													   //int slope[CAMERA_H] = { 0 };                       //���ÿ�м����б�ȵ�����
	int count = 0;
	int i = 0;

	left_black_before = CAMERA_W / 2;
	right_black_before = CAMERA_W / 2;
	jh = LINE_NUM-1;


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
				if (left_black_before > CAMERA_W) left_black_before = CAMERA_W;
			}
			jw_left--;                                        //��û��ɨ�赽���߾��ƶ�һ�����ؼ���ɨ��
		}
		if ((0 == jw_left) && (0 == left_find_flag))          //����ҵ����һ��������Ȼδ�ҵ����Ϻ������������أ�����Ϊ��һ��Ϊ����
		{
			left_end_flag = 1;
			left_find_flag = 1;
			left_black[jh] = jw_left;
			left_black_before = jw_left + OFF_CENTER;         //------�Ժ�ǵø��ź�ת��
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
			right_end_flag = 1;
			right_find_flag = 1;
			right_black[jh] = jw_right;
			right_black_before = jw_right - OFF_CENTER;
		}

		


		/*--------------------�����߲�����һЩ���Ĵ���------------------------*/
		if ((1 == left_find_flag) && (1 == right_find_flag))                 //���ߺ��߶��ҵ�ʱ��ʼ���㣬������ifʱ��Ϊһ���ҵ����߲��ܽ�����㣬Ҫ���߶��ҵ�
		{

			/*---------------------�ж��Ƿ���ʮ�ֵ�-------------------------------*/
			if (1 == left_end_flag && 1 == right_end_flag && fabs(average_offset[1]) < 9 && 0 == cross_flag)  //������Ҷ�û��ɨ��������ƫ���Сʱ
			{
				iscross_count++;                     //��ʱ�������߶��Ҳ����ߵ����ǲ�������
				if (10 == iscross_count)
				{
					iscross_count = 0;
					cross_flag = 1;
					cross_begin = jh - 10;
				}				
				left_black_before = CAMERA_W / 2;
				right_black_before = CAMERA_W / 2;
			}

			/*---------------------���ĵ�ļ���------------------------*/
			if (jh == (LINE_NUM-1))
				middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
			else if (cross_flag == 1)  //��ʮ�ֵ���Χ��,���ĵ�ļ���
			{
				if (0 == left_end_flag && 0 == right_end_flag)
				{
					for (i = cross_begin; i > jh - 1; i--)   //����
					{
						left_black[i] = left_black[cross_begin + 2] + (left_black[cross_begin + 2] - left_black[jh - 2])*((i - cross_begin - 2) / (cross_begin - jh));
						right_black[i] = right_black[cross_begin + 2] + (right_black[cross_begin + 2] - right_black[jh - 2])*((i - cross_begin - 2) / (cross_begin - jh));
						//left_black[i] = (left_black[cross_begin + 3] + left_black[jh - 3]) / 2;
						//right_black[i] = (right_black[cross_begin + 3] + right_black[jh - 3]) / 2;
						middleline[i] = (left_black[i] + right_black[i]) / 2;
						img[i][left_black[i]] = !img[i][left_black[i]];
						img[i][right_black[i]] = !img[i][right_black[i]];
					}
					cross_flag = 0;
				}
				else if (0 == jh)
				{
				    middleline[cross_begin] = -2; //��������Ϊ-1ʱ��Ч -2ʱ֮�����ĵ���Ч
					cross_flag = 0;
					break;
				}
				else
				{
					left_black_before = CAMERA_W / 2;
					right_black_before = CAMERA_W / 2;
				}
			}
			else
			{
				if (1 == left_end_flag || 1 == right_end_flag)
				{
					if (1 == right_end_flag) middleline[jh] = left_black[jh] + 36 / (1 + (60 - jh)*TRAPEZOID_CORRECT / 40);
					else middleline[jh] = right_black[jh] - 36 / (1 + (60 - jh)*TRAPEZOID_CORRECT / 40);
				}
				else
				{
					middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
				}
				
				/*----------------���ĵ���Ч����ɨ��ֹͣ���ж�------------------*/
				if (abs(middleline[jh + 1] - middleline[jh]) > DEGREE_OF_ERROR)     middleline[jh] = -1;   //����б�ʹ��󣨼�ת�䣩�����ϴ����ĵ���Ч

				else if (0 == img[jh][middleline[jh]] && (jh < 30))
				{
					middleline[jh] = -2;  //-2����֮��ɨ������
					break;
				}
				//else if (abs(middleline[jh + 1] - middleline[jh]) > REFLECT_LIGHT_ERROR && (jh < REFLECT_LIGHT_AREA))  //����ƫ�����
				//{
				//	break;
				//}
				//if ((middleline[jh]< CLOSE_TO_EDGE) || (middleline[jh]>(CAMERA_W - CLOSE_TO_EDGE))) //���߽ӽ�ͼ���Ե,�˳�ɨ��
				//{
				//	break;
				//}
			}
			left_end_flag = 0;
			right_end_flag = 0;
			left_next_line = 0;                                                 //�����ع�
			right_next_line = 0;
			left_find_flag = 0;
			right_find_flag = 0;
			jh--;                                                             //������һ�е�ɨ��
		} //if  
	} //while


	/*--------------------------����·���µ�ɨ��-------------------------*/
	//if()



    /*------------------------ƫ��ȼ��㼰����չ��-----------------------------*/
	for (i = LINE_NUM - 1; i >= 0; i--)
	{
		if (-2 == middleline[i])
			break;
		if (-1 == middleline[i]) {}
		else
		{
			offset = offset + ((float)(middleline[i] - CAMERA_W / 2)*(1 + (60 - i)*TRAPEZOID_CORRECT / 40));          //offset�ǲ�����������������������ƫ��,<0ƫ��
			count++;
			if (middleline[i] > CAMERA_W - 1)middleline[i] = CAMERA_W - 1;
			if (middleline[i] < 0)middleline[i] = 0;
			img[i][middleline[i]] = !img[i][middleline[i]];
		}
	}


	/*---------------�����pid---------------------*/
	for (i = 10; i > 1; i--)
	{
		average_offset[i] = average_offset[i - 1];
	}
	average_offset[1] = ((float)offset / (float)count);
	average_offset[1] -= CAMERA_HARDWARE_ERROR;
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


//float Find_slope()       //�ҵ����ε�б��
//{
//	int j = 79;     //��ǰ��
//	int i = 59;      //��ǰ��
//	int count = 0;
//	int temp = 0;
//	int find[60];
//
//
//	while (i >= 0)     //��0��1
//	{
//		while (j > 0)
//		{
//			if (0 == img[i][j])
//			{
//				temp = j;
//				while (0 == img[i][j])
//				{
//					count++;
//					j--;
//				}
//				find[i] = temp - (count / 2);
//				img[i][find[i]] = -img[i][find[i]];
//				count = 0;
//				break;
//			}
//			j--;
//		}
//		i--;
//	}
//
//	return ((float)(find[59] + find[58] + find[57] - find[0] - find[1] - find[2]) / (3.0*58.0));
//}



//float Find_slope()       //�ҵ����ε�б��
//{
//	int j_now = 0;     //��ǰ��
//	int i_now = 0;     //��ǰ��
//	int j_before = 0;  //ǰһ�н��ӵ����
//	Site_t site1;
//
//	while (i_now < 50) //��0��1
//	{
//		j_now = j_before;
//
//		while (j_now < 40)
//		{
//			if (0 == img[i_now][j_now] && 1 == img[i_now][j_now + 1])
//			{
//				site1.x = 128 - 128.0*((float)(j_now + 1) / 80.0);
//				site1.y = 128 - 128.0*((float)(i_now + 1) / 60.0);
//				LCD_point(site1, GREEN);
//				j_before = j_now;
//				break;
//			}
//			j_now++;
//		}
//		if (49 == i_now)
//		{
//			return (float)(j_now) / (float)(i_now);
//		}
//		i_now++;
//	}
//}





//void Search_line()
//{
//	int left_black[CAMERA_H];                          //��ߺ�������
//	int left_black_before = CAMERA_W / 2;              //��һ�����ɨ�赽�ĺ���λ��
//	int left_next_line = 0;                            //�ж�ɨ���Ƿ�������µ�һ��
//	int left_find_flag = 0;                            //����Ƿ��ҵ����߱�־
//	//int left_slope[CAMERA_H] = { 0 };                  //��ƫ���
//	int jw_left;                                       //���������ĵ�ǰ��
//
//	int right_black[CAMERA_H];                         //�ұߺ�������
//	int right_black_before = CAMERA_W / 2;
//	int right_next_line = 0;
//	int right_find_flag = 0;
//	//int right_slope[CAMERA_H] = { 0 };
//	int jw_right;
//
//	int jh;                                            //�в���
//	int middleline[CAMERA_H] = { 0 };                  //�洢����λ�õ�����
//	int offset = 0;                                    //ƫ��ȣ�Ϊ����ƫ���
//	//int slope[CAMERA_H] = { 0 };                       //���ÿ�м����б�ȵ�����
//	int count = 0;
//	//int sumslope = 0;                                  //б�ȵ��ܺ�   
//
//	average_slope = 0;
//	left_black_before = CAMERA_W / 2;
//	right_black_before = CAMERA_W / 2;
//	jh = 0;                       
//
//	while (jh<(0+ LINE_NUM))                                  //����������������ߣ���ÿһ��ͼ����ж�ɨ��һ��,LINE_NUM��ʾҪɨ������
//	{
//		if (0 == left_next_line)                              //left_black_next������־�Ƿ�����µ�һ�У���Ϊ0���ǽ����µ�һ��
//		{
//			jw_left = left_black_before;                      //�����µ�һ�п�ʼ����ϴ��ҵ����ߵ�λ�ÿ�ʼ�Һ���
//			left_next_line = 1;                               //��־������λ
//		}
//
//		if (jw_left>0 && (0 == left_find_flag))               //���ɨ�軹û�е���Ե��֮ǰ��ɨ��δ�ҵ��ڵ�
//		{
//			//if (1> (img[jh][jw_left] + img[jh][jw_left-1]))  //�ڵ�Ϊ0��flagΪ0��û�ҵ���������������3���ڵ�����Ϊ�ҵ�����
//			if ((img[jh][jw_left]) < 1)
//			{
//				left_find_flag = 1;                           //ɨ�赽���ߣ���־��1
//				left_black[jh] = jw_left;                     //��������Ϊjw_left
//				left_black_before = jw_left + OFF_CENTER;     //��һ��ɨ�����һ�κ��ߵ�����ƫ��15�����صĵط���ʼɨ��
//				if (left_black_before > CAMERA_W) left_black_before = CAMERA_W;
//			}
//			jw_left--;                                        //��û��ɨ�赽���߾��ƶ�һ�����ؼ���ɨ��
//		}
//		if ((0 == jw_left) && (0 == left_find_flag))          //����ҵ����һ��������Ȼδ�ҵ����Ϻ������������أ�����Ϊ��һ��Ϊ����
//		{
//			left_find_flag = 1;
//			left_black[jh] = jw_left;
//			left_black_before = jw_left + OFF_CENTER;         //------�Ժ�ǵø��ź�ת��
//			if (jh > ROAD_BEGIN && jh < ROAD_END)
//			{
//				sharp_turn = SHARP_TURN_RIGHT;
//				return;
//			}
//		}
//
//		/*���Ҳ��ɨ���Һڵ�*/
//		if (0 == right_next_line)
//		{
//			jw_right = right_black_before;
//			right_next_line = 1;
//		}
//
//		if ((jw_right< (CAMERA_W - 1)) && (0 == right_find_flag))            //��Ϊ��0λҲ�������ص㣬���Դ������ص�����һλ��CAMERA_W-1
//		{
//			//if ( (img[jh][jw_right]+ img[jh][jw_right+1])<1)             
//			if ((img[jh][jw_right])<1)
//			{
//				right_find_flag = 1;
//				right_black[jh] = jw_right;
//				right_black_before = jw_right - OFF_CENTER ;
//				if (right_black_before < 0) right_black_before = 0;
//			}
//			jw_right++;
//		}
//		if (jw_right == (CAMERA_W - 1) && (0 == right_find_flag))            //�ұߺڵ�δ�ҵ�
//		{
//			right_find_flag = 1;
//			right_black[jh] = jw_right;
//			right_black_before = jw_right - OFF_CENTER;
//			if (jh > ROAD_BEGIN && jh < ROAD_END)
//			{
//				sharp_turn = SHARP_TURN_LEFT;
//				return;
//			}
//		}
//
//
//		if ((1 == left_find_flag) && (1 == right_find_flag))                 //���ߺ��߶��ҵ�ʱ��ʼ���㣬������ifʱ��Ϊһ���ҵ����߲��ܽ�����㣬Ҫ���߶��ҵ�
//		{
//
//			if (jh == 0)
//				middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
//			if (jh < LINE_NUM)
//			{
//				//right_slope[jh] = right_black[jh] - right_black[jh + 1];
//				//left_slope[jh] = left_black[jh] - left_black[jh + 1];
//				//middleline[jh] = middleline[jh + 1] + right_slope[jh] + left_slope[jh];     //����ƫ�Ѱ������
//				middleline[jh] = (left_black[jh] + right_black[jh]) / 2;
//				//img[jh][middleline[jh]] = !img[jh][middleline[jh]];
//
//				if (middleline[jh -1] - middleline[jh] > DEGREE_OF_ERROR || middleline[jh -1] - middleline[jh] < -DEGREE_OF_ERROR) {} //����б�ʹ��󣨼�ת�䣩�����ϴ�flag��1
//
//				else if (abs(middleline[jh - 1] - middleline[jh]) > REFLECT_LIGHT_ERROR && (jh > REFLECT_LIGHT_AREA))
//				{
//					break;
//				}
//
//				else
//				{
//					//slope[jh] = (middleline[jh -1] - middleline[jh]);               //б��
//					//sumslope += slope[jh];           //Ҫ�Ӿ���ֵ��
//					offset = offset - (CAMERA_W / 2 - middleline[jh]);             //offset�ǲ�����������������������ƫ��
//					count++;
//					img[jh][middleline[jh]] = !img[jh][middleline[jh]];
//				}
//			}
//
//			if ((middleline[jh]< CLOSE_TO_EDGE) || (middleline[jh]>(CAMERA_W - CLOSE_TO_EDGE))) //���߽ӽ�ͼ���Ե,�˳�ɨ��
//			{
//				jh = LINE_NUM ;
//			}
//
//			left_next_line = 0;                                                 //�����ع�
//			right_next_line = 0;
//			left_find_flag = 0;
//			right_find_flag = 0;
//			jh++;                                                             //������һ�е�ɨ��
//		} //if  
//	} //while
//
//	//average_slope = (sumslope * 100) / count;
//	//average_offset = (offset / count);
//	average_offset_before = average_offset;
//	average_offset = -((float)offset / (float)count);
//	average_offset -= CAMERA_HARDWARE_ERROR;
//	//if (average_offset > 15) average_offset = 15;
//	//if (average_offset < -15) average_offset = -15;
//}