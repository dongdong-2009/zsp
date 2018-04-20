#ifndef _LCD_SHOW_H_
#define _LCD_SHOW_H_

#include "common.h"
#include "include.h"

#define IMG_MODE 1
#define UI_MODE  2
#define ITEM_NUM 3

extern int _temp;
extern uint8 key_on;
extern uint8 lcd_mode;
extern uint8 ui_point;
extern int colour[];
extern int ui_data[];
extern Site_t tem_site_str[];
extern Site_t tem_site_data[];



void PORTD_IRQHandler();
void UI_INIT();
void Open_UI();



#endif