
#include "Auto_Off_option.h"
#include "u8g2_LCD.h"
extern u8g2_t u8g2;
uint16_t off_sec = 0;
uint16_t off_min = 0;



void Auto_Off_option_screen_func(void)
{
	RTC_view();
	u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);

	u8g2_DrawUTF8(&u8g2, 20, 60, "Автовыключение:");
	sprintf(RAM,"%d", Auto_Off_Time);
	Blink_Edit_Parameter(180, 60, RAM, edit_param, 0);


	u8g2_SendBuffer(&u8g2);
	u8g2_ClearBuffer(&u8g2);
}



// MENU
void Auto_Off_option_screen_S9(void)
{
	screen=Parameter_screen;
	old_active_string=0xFF;
	Menu_num=3;
}

// BACKWARD
void Auto_Off_option_screen_S8(void)
{ if(Auto_Off_Time>1)Auto_Off_Time--; }

// FORWARD
void Auto_Off_option_screen_S5(void)
{ if(Auto_Off_Time<120)Auto_Off_Time++; }




void Auto_OFF_process(void)
{
	if(Auto_Off_Time!=0)
	{
		if(++off_sec>=59){off_min++;off_sec=0;}

		if(screen != Off_screen)
		{
			if(off_min>=Auto_Off_Time)
			{
				old_active_string=0xFF;
				OnOffCntr = on_off_time;
				screen = Off_screen;
			}
		}
	}
	sleep(1000);
}


