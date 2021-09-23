
#include "Measure_screen.h"
#include "u8g2_LCD.h"
extern u8g2_t u8g2;

uint16_t view_tim=0;
uint8_t view=0;

void Main_screen_func(void)
{
	if(view==1)
	{
		RTC_view();
		view=0;

		// Отображение давления
		u8g2_SetFont(&u8g2, u8g2_font_inr30_mn);
		if(Pressure<10.0) sprintf(RAM,"%+.5f", Pressure);
		else if(Pressure<100.0) sprintf(RAM,"%+.4f", Pressure);
		else sprintf(RAM,"%+.3f", Pressure);
		u8g2_DrawUTF8(&u8g2, 0, 70, RAM);
		u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);
		u8g2_DrawUTF8(&u8g2, 200, 70, UNITS_NAME[Units]);

		// Отображение електрических величин
		u8g2_SetFont(&u8g2, u8g2_font_inr27_t_cyrillic);
		switch(E_fun)
		{
			// Контакты
			case SW:
				if(SW_state==0)u8g2_DrawUTF8(&u8g2, 25, 130, "РАЗОМКНУТ");
				else u8g2_DrawUTF8(&u8g2, 25, 130, " ЗАМКНУТ ");
			break;

			// Ток
			case mA:
				if(Pressure<10.0) sprintf(RAM,"%+.6f", current_4_20mA-Current_zero);
				else if(Pressure<100.0) sprintf(RAM,"%+.5f", current_4_20mA-Current_zero);
				else sprintf(RAM,"%+.4f", current_4_20mA-Current_zero);
				u8g2_DrawUTF8(&u8g2, 0, 130, RAM);
				u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);
				u8g2_DrawUTF8(&u8g2, 200, 130, E_UNITS_NAME[E_fun]);
			break;

			// Напряжение
			case V:
				if(Pressure<10.0) sprintf(RAM,"%+.6f", voltage_measure-Voltage_zero);
				else if(Pressure<100.0) sprintf(RAM,"%+.5f", voltage_measure-Voltage_zero);
				else sprintf(RAM,"%+.4f", voltage_measure-Voltage_zero);
				u8g2_DrawUTF8(&u8g2, 0, 130, RAM);
				u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);
				u8g2_DrawUTF8(&u8g2, 200, 130, E_UNITS_NAME[E_fun]);
			break;
		}

		u8g2_SendBuffer(&u8g2);
		u8g2_ClearBuffer(&u8g2);
	}
}



// Меню
void Main_screen_S9(void)
{
	if(screen=Main_screen)
	{
		screen=Option_screen;
		old_active_string=0xFF;
		Menu_num=0;
	}
}


// Таймер для частоты обновления экрана с показаниями
void Viewer_timer(void)
{
	if(++view_tim==300){view=1;view_tim=0;}
}

