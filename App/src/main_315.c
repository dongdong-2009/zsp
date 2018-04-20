//#include "common.h"
//#include "include.h"
//
//
//uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
//uint8 img[CAMERA_H][CAMERA_W];
//
//void PORTA_IRQHandler();
//void DMA0_IRQHandler();
//
//void  main()
//{
//    Site_t site     = {0, 0};                           //显示图像左上角位置    
//    Size_t size;                                        //lcd显示区域图像大小
//    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //解压图像大小
//	int i = 0;
//
//	LED_show MODE = img_mode;
//	size.H = LCD_H;
//    size.W = LCD_W;
//
//    LCD_init();
//    camera_init(imgbuff);
//	gpio_init(PTE21, GPO, 0);
//	gpio_ddr(PTA8, GPO);
//
//	set_vector_handler(PORTA_VECTORn, PORTA_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
//	set_vector_handler(DMA0_VECTORn, DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
//
//    while(1)
//    {
//        camera_get_img();                              //摄像头获取图像
//                                                       //黑白摄像头
//        img_extract(img, imgbuff, CAMERA_SIZE);        //解压
//		//Search_line();
//		//img_compress(img, imgbuff, CAMERA_SIZE);
//		LCD_Img_Binary_Z(site, size, imgbuff, imgsize);
//
//		i = 300;
//		while (i--);
//		gpio_turn(PTE21);
//    }
//}
//
//
//
//
///*!
//*  @brief      PORTA中断服务函数
//*  @since      v5.0
//*/
//void PORTA_IRQHandler()
//{
//	uint8  n;    //引脚号
//	uint32 flag;
//
//	while (!PORTA_ISFR);
//	flag = PORTA_ISFR;
//	PORTA_ISFR = ~0;                                   //清中断标志位
//
//	n = 29;                                             //场中断
//	if (flag & (1 << n))                                 //PTA29触发中断
//	{
//		camera_vsync();
//	}
//#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
//	n = 28;
//	if (flag & (1 << n))                                 //PTA28触发中断
//	{
//		camera_href();
//	}
//#endif
//
//
//}
//
///*!
//*  @brief      DMA0中断服务函数
//*  @since      v5.0
//*/
//void DMA0_IRQHandler()
//{
//	camera_dma();
//}



/*!
*     COPYRIGHT NOTICE
*     Copyright (c) 2013,山外科技
*     All rights reserved.
*     技术讨论：山外论坛 http://www.vcan123.com
*
*     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
*     修改内容时必须保留山外科技的版权声明。
*
* @file       main.c
* @brief      山外K60 平台主程序
* @author     山外科技
* @version    v5.0
* @date       2013-08-28
*/

#include "common.h"
#include "include.h"


//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();


uint8 sharp_turn=0;
uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 img[CAMERA_H][CAMERA_W];


void  main(void)
{
	//int i = 0;
	//int j = 0;
	int degree_change_speed = 0;
	int degree_change = 0;
	Site_t site = { 0, 0 };             
	Size_t size;  
	Size_t imgsize = { CAMERA_W, CAMERA_H };
	     
	size.H = LCD_H;
	size.W = LCD_W;

	LCD_init();
	camera_init(imgbuff);
    ftm_pwm_init(FTM0, FTM_CH5, 300, 0);
	ftm_pwm_init(FTM0, FTM_CH6, 300, 43);

	set_vector_handler(PORTA_VECTORn, PORTA_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
	set_vector_handler(DMA0_VECTORn, DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
	

	while (1)
	{
		camera_get_img();                                   
		img_extract(img, imgbuff, CAMERA_SIZE);

		Search_line();
		img_compress(img, imgbuff, CAMERA_SIZE);
		LCD_Img_Binary_Z(site, size, imgbuff, imgsize);


		if (average_offset < 5 && average_offset > -5)
		{
			ftm_pwm_duty(FTM0, FTM_CH6, 43 + average_offset);
		}
		else
		{
			degree_change = average_offset - average_offset_before;

					if (average_offset_before > 0)
					{
						if (degree_change > 0)
						{
							if (degree_change > 12) degree_change_speed = 7;
							else if (degree_change > 7) degree_change_speed = 4;
							else degree_change_speed = 2;
							steer_engine_degree += degree_change_speed;
						}
						if (degree_change < 0 && (abs(average_offset)<7)) steer_engine_degree -= 2;
					}
					else if (average_offset_before < 0)
					{
						if (degree_change < 0)
						{
							if (degree_change < -12) degree_change_speed = -7;
							else if (degree_change < -7) degree_change_speed = -4;
							else degree_change_speed = -2;
							steer_engine_degree += degree_change_speed;
						}
						if (degree_change > 0 && (abs(average_offset)<7)) steer_engine_degree += 2;
					}
	

					if (steer_engine_degree > 15) steer_engine_degree = 15;
					if (steer_engine_degree < -15) steer_engine_degree = -15;
		
					ftm_pwm_duty(FTM0, FTM_CH6, 43+ steer_engine_degree);
		}
		
		//ftm_pwm_duty(FTM0, FTM_CH6, 43 + average_offset_before);

		ftm_pwm_duty(FTM0, FTM_CH5,10 );                    //电机
		//if (SHARP_TURN_LEFT == sharp_turn)
		//{
		//	ftm_pwm_duty(FTM0, FTM_CH6, 43 - TURN_DEGREE);
		//	sharp_turn = 0;
		//}
		//else if (SHARP_TURN_RIGHT == sharp_turn)
		//{
		//	ftm_pwm_duty(FTM0, FTM_CH6, 43+ TURN_DEGREE);
		//	sharp_turn = 0;
		//}
		//else
		//{
		//	ftm_pwm_duty(FTM0, FTM_CH6,43+ average_offset);           //舵机
		//}
		
	}
}



/*!
*  @brief      PORTA中断服务函数，与摄像头有关
*  @since      v5.0
*/
void PORTA_IRQHandler()
{
	uint8  n;    //引脚号
	uint32 flag;

	while (!PORTA_ISFR);
	flag = PORTA_ISFR;
	PORTA_ISFR = ~0;                                   //清中断标志位

	n = 29;                                             //场中断
	if (flag & (1 << n))                                 //PTA29触发中断
	{
		camera_vsync();
	}
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
	n = 28;
	if (flag & (1 << n))                                 //PTA28触发中断
	{
		camera_href();
	}
#endif


}

/*!
*  @brief      DMA0中断服务函数，与摄像头有关
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
	camera_dma();
}

