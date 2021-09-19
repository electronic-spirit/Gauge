
#include "about_screen_options.h"
#include "u8g2_LCD.h"
#include "stdio.h"
extern u8g2_t u8g2;



const char* GAUGE_RANGE_TEXT[11]=
{
	[0]="-100 100 MPa",
	[1]="0 200 MPa",
	[2]="0 500 MPa",
	[3]="0 300 MPa",
	[4]="0 300 MPa",
	[5]="0 300 MPa",
	[6]="0 300 MPa",
	[7]="0 300 MPa",
	[8]="0 300 MPa",
	[9]="0 300 MPa",
	[10]="0 300 MPa",
};



void About_options_screen_func(void)
{
	RTC_view();
	u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);

	u8g2_DrawUTF8(&u8g2, 10, 35, "Серийный номер:");
	sprintf((char*)RAM, "%05d", serial_num);
	u8g2_DrawUTF8(&u8g2, 170, 35, (char*)RAM);

	u8g2_DrawUTF8(&u8g2, 10, 50, "Диапазон:");
	u8g2_DrawUTF8(&u8g2, 110, 50, GAUGE_RANGE_TEXT[gauge_range]);

	u8g2_DrawUTF8(&u8g2, 10, 65, "Погрешность:");
	sprintf((char*)RAM, "%.4f", gauge_error);
	u8g2_DrawUTF8(&u8g2, 140, 65, (char*)RAM);

	u8g2_DrawUTF8(&u8g2, 10, 145, "Код ошибки:");
	sprintf((char*)RAM, "%d", ERROR_REG);
	u8g2_DrawUTF8(&u8g2, 130, 145, (char*)RAM);



	u8g2_SendBuffer(&u8g2);
	u8g2_ClearBuffer(&u8g2);
}


// MENU
void About_options_screen_S9(void)
{
	screen=Option_screen;
	old_active_string=0xFF;
	Menu_num=3;
}

