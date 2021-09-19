
#include "Measure_screen.h"
#include "u8g2_LCD.h"
extern u8g2_t u8g2;

uint16_t view_tim=0;
uint8_t view =0;

void Main_screen_func(void)
{
	if(view==1)
	{
		RTC_view();

		view=0;
		u8g2_SetFont(&u8g2, u8g2_font_inr30_mn);

		sprintf(RAM,"%+fV", Pressure);
		u8g2_DrawUTF8(&u8g2, 5, 70, RAM);

		switch(E_fun)
		{
		// Давление
		case 0:
			sprintf(RAM,"%+fV", Test_Pressure);
				u8g2_DrawUTF8(&u8g2, 5, 130, RAM);
				u8g2_SetFont(&u8g2, u8g2_font_9x15_t_cyrillic);
				u8g2_DrawUTF8(&u8g2, 160, 155, "давление");
			break;

			// Ток
		case 1:
			sprintf(RAM,"%+fV", current_4_20mA-Electric_zero);
				u8g2_DrawUTF8(&u8g2, 5, 130, RAM);
				u8g2_SetFont(&u8g2, u8g2_font_9x15_t_cyrillic);
				u8g2_DrawUTF8(&u8g2, 160, 155, "ток");
			break;

			// Напряжение
		case 2:
			sprintf(RAM,"%+fV", voltage_measure-Electric_zero);
				u8g2_DrawUTF8(&u8g2, 5, 130, RAM);
				u8g2_SetFont(&u8g2, u8g2_font_9x15_t_cyrillic);
				u8g2_DrawUTF8(&u8g2, 160, 155, "напряжение");
			break;
		}

		u8g2_DrawUTF8(&u8g2, 5, 35, "Каюков А.Н.");
		u8g2_DrawUTF8(&u8g2, 5, 95, "Агапов А.Д.");

		sprintf(RAM,"t %.2f", Temperature2);
		u8g2_DrawUTF8(&u8g2, 120, 35, RAM);

		sprintf(RAM,"%.6f", Temperature_coeff_25);
		u8g2_DrawUTF8(&u8g2, 5, 155, RAM);

		sprintf(RAM,"%.2f", MCU_temp);
		u8g2_DrawUTF8(&u8g2, 95, 155, RAM);


		sprintf(RAM,"t %.2f", Temperature);
		u8g2_DrawUTF8(&u8g2, 120, 95, RAM);

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


void Viewer_timer(void)
{
	if(++view_tim==300){view=1;view_tim=0;}
}

