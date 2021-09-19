
#include "logging_option_screen.h"
#include "u8g2_LCD.h"
extern u8g2_t u8g2;
#include "stdio.h"

#define max_edit_param 6



void Logging_options_screen_func(void)
{
	RTC_view();
	u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);

	u8g2_DrawUTF8(&u8g2, 5, 40, "Логгирование:");
	u8g2_DrawUTF8(&u8g2, 5, 60, "Время старта:");
	u8g2_DrawUTF8(&u8g2, 5, 80, "Период сохранения:");
	u8g2_DrawUTF8(&u8g2, 5, 100, "Длительность:");
	u8g2_DrawUTF8(&u8g2, 5, 120, "Всего файлов:");


	// 0 parameter
	if(logging==0)Blink_Edit_Parameter(155, 40, "ДА", edit_param, 0);
	else Blink_Edit_Parameter(155, 40, "НЕТ", edit_param, 0);
	// 1 parameter
	sprintf(RAM,"%02d:", logging_start_hour);
	Blink_Edit_Parameter(155, 60, RAM, edit_param, 1);
	// 2 parameter
	sprintf(RAM,"%02d:", logging_start_min);
	Blink_Edit_Parameter(185, 60, RAM, edit_param, 2);
	// 3 parameter
	sprintf(RAM,"%02d", logging_start_sec);
	Blink_Edit_Parameter(215, 60, RAM, edit_param, 3);
	// 4 parameter
	sprintf(RAM,"%d", logging_period);
	Blink_Edit_Parameter(195, 80, RAM, edit_param, 4);
	// 5 parameter
	sprintf(RAM,"%d", logging_len);
	Blink_Edit_Parameter(155, 100, RAM, edit_param, 5);
	// 6 parameter
	sprintf(RAM,"%d", logging_num);
	Blink_Edit_Parameter(155, 120, RAM, edit_param, 6);


	u8g2_SendBuffer(&u8g2);
	u8g2_ClearBuffer(&u8g2);
}


// DOWN
void Logging_options_screen_S6(void)
{
	if(edit_param<(max_edit_param-1))edit_param++;
}

// UP
void Logging_options_screen_S7(void)
{
	if(edit_param>0)edit_param--;
}




// MENU
void Logging_options_screen_S9(void)
{
	screen=Option_screen;
	old_active_string=0xFF;
	Menu_num=5;
}

// BACKWARD
void Logging_options_screen_S8(void)
{
	switch(edit_param)
	{
		case 0: if(logging>0)logging--; break;
		case 1: if(logging_start_hour>0)logging_start_hour--; break;
		case 2: if(logging_start_min>0)logging_start_min--; break;
		case 3: if(logging_start_sec>0)logging_start_sec--; break;
		case 4: if(logging_period>0)logging_period--; break;
		case 5: if(logging_len>0)logging_len--; break;
	}
}

// FORWARD
void Logging_options_screen_S5(void)
{
	switch(edit_param)
	{
		case 0: if(logging<1)logging++; break;
		case 1: if(logging_start_hour<23)logging_start_hour++; break;
		case 2: if(logging_start_min<59)logging_start_min++; break;
		case 3: if(logging_start_sec<59)logging_start_sec++; break;
		case 4: if(logging_period<30)logging_period++; break;
		case 5: if(logging_len<100)logging_len++; break;
	}
}
