#include "include.h"

uint8 lcd_mode = IMG_MODE;
uint8 ui_point = 0;
uint8 key_on = 0;
int colour[] = { GREEN ,WHITE ,WHITE,WHITE };
int ui_data[] = { 0,0,0,0 };
Site_t tem_site_str[] = { 0, 0, 0, 20, 0, 40, 0, 60, 0, 80, 0, 100 };
Site_t tem_site_data[] = { 60, 0, 60, 20, 60, 40, 60, 60, 60, 80, 60, 100};


void PORTD_IRQHandler()
{
	uint32 flag;

	while (!PORTD_ISFR);
	flag = PORTD_ISFR;
	PORTD_ISFR = ~0;                                   //清中断标志位

	if (IMG_MODE == lcd_mode)
	{
		if (flag & (1 << 13))  lcd_mode = UI_MODE;         //如果中键按下，则进入ui模式
														   //LCD_clear(WHITE);
	}
	else
	{
		if (flag & (1 << 13))    //中键按下
		{
			if (ui_point)
			{
				if (GREEN == colour[ui_point])   //背景色改变
				{
					colour[ui_point] = RED;
				}
				else
				{
					colour[ui_point] = GREEN;
				}
			}
			else
			{
				lcd_mode = IMG_MODE;
			}
		}
		else if (flag & (1 << 12))  //右键按下
		{
			if (ui_point)
			{
				if (RED == colour[ui_point])  //背景为红色表示可以改对应值
				{
					ui_data[ui_point]++;
				}
			}
		}
		else if (flag & (1 << 11))  //左键按下
		{
			if (ui_point)
			{
				if (RED == colour[ui_point])  //背景为红色表示可以改对应值
				{
					ui_data[ui_point]--;
					if (ui_data[ui_point] < 0) ui_data[ui_point] = 0;
				}
			}
		}
		else if (flag & (1 << 10)) //上键按下
		{
			if (GREEN == colour[ui_point] && ui_point != 0)
			{
				if (1 == ui_point)
				{
					colour[ui_point] = WHITE;
					ui_point--;
				}
				else
				{
					colour[ui_point] = WHITE;
					ui_point--;
					colour[ui_point] = GREEN;
				}
			}
		}
		else if (flag & (1 << 14)) //下键按下
		{
			if (GREEN == colour[ui_point] && ui_point != ITEM_NUM)
			{
				if (0 == ui_point)
				{
					ui_point++;
					colour[ui_point] = GREEN;
				}
				else
				{
					colour[ui_point] = WHITE;
					ui_point++;
					colour[ui_point] = GREEN;
				}
			}

		}
		else {}

	}
	key_on = 1;
	disable_irq(PORTD_IRQn);
}


void Open_UI()
{
	if (1 == key_on)
	{
		LCD_clear(WHITE);
		LCD_str(tem_site_str[0], "dgree:", BLACK, colour[1]);
		LCD_str(tem_site_str[1], "ANG:", BLACK, colour[2]);
		LCD_str(tem_site_str[2], "STA:", BLACK, colour[3]);
		LCD_num(tem_site_data[0], _temp, BLACK, WHITE);
		//LCD_num(tem_site_data[0], quad_val, BLACK, WHITE);
		LCD_num(tem_site_data[1], ui_data[2], BLACK, WHITE);
		LCD_num(tem_site_data[2], ui_data[3], BLACK, WHITE);
		key_on = 0;
		enable_irq(PORTD_IRQn);
	}
}


void UI_INIT()
{
	/*配置各个按键的中断*/
	port_init(PTD14, ALT1 | IRQ_FALLING | PULLUP);  //下
	port_init(PTD13, ALT1 | IRQ_FALLING | PULLUP);  //中
	port_init(PTD12, ALT1 | IRQ_FALLING | PULLUP);      //右
	port_init(PTD11, ALT1 | IRQ_FALLING | PULLUP);      //左
	port_init(PTD10, ALT1 | IRQ_FALLING | PULLUP);      //上
	enable_irq(PORTD_IRQn);                //使能d对应的端口也就是按键的port
}