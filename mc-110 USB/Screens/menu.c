

#include "menu.h"
#include "u8g2_LCD.h"


u8g2_t u8g2;
uint8_t old_active_string = 0xFF;		// Номер предыдущей строки
uint8_t Menu_num = 1;  					// Номер строки меню
int16_t float_menu = 0;


// Основное меню
const uint8_t menu_text_1[] = "ИЗМЕРЕНИЕ";
const uint8_t menu_text_2[] = "24 В";
const uint8_t menu_text_3[] = "ПАРАМЕТРЫ";
const uint8_t menu_text_4[] = "О ПРИБОРЕ";
const uint8_t menu_text_5[] = "КАЛИБРОВКА";
const uint8_t menu_text_6[] = "ЛОГГИРОВАНИЕ";
const uint8_t menu_text_7[] = "ТЕХ.ДАННЫЕ";
const uint8_t menu_text_8[] = "ТЕХ.ПАРАМЕТРЫ";
const uint8_t menu_text_9[] = "СЕРВИСНОЕ МЕНЮ";

#define Menu_option_num 	9	    	// Количество пунктов меню настроек
const uint8_t *menu_option[Menu_option_num] = {menu_text_1,menu_text_2,menu_text_3,menu_text_4,menu_text_5,menu_text_6,menu_text_7,menu_text_8,menu_text_9};
const uint8_t menu_option_size[Menu_option_num] = {9,4,9,9,10,12,10,14,14};


// Меню ПАРАМЕТРЫ
const uint8_t menu_text_10[] = "ДИСПЛЕЙ";
const uint8_t menu_text_11[] = "ГРОМКОСТЬ";
const uint8_t menu_text_12[] = "ДАТА И ВРЕМЯ";
const uint8_t menu_text_13[] = "АВТООТКЛЮЧЕНИЕ";
const uint8_t menu_text_14[] = "НАСТРОЙКА ПОДКЛЮЧЕНИЯ";
const uint8_t menu_text_15[] = "НАСТРОЙКА АЦП";
const uint8_t menu_text_16[] = "РЕЧ. ИНФОРМИРОВАНИЕ";

#define Menu_parameter_num 	7	    	// Количество пунктов меню настроек
const uint8_t *menu_parameter[Menu_parameter_num] = {menu_text_10,menu_text_11,menu_text_12,menu_text_13,menu_text_14,menu_text_15,menu_text_16};
const uint8_t menu_parameter_size[Menu_parameter_num] = {7,9,12,14,21,13,19};




void Menu_GUI(uint16_t x_pos, uint16_t y_pos, const unsigned char *menu_string[], uint8_t *str_size,
		uint8_t str_to_str, uint8_t menu_size, uint8_t active_string)
{
	uint8_t i=0;
	if((old_active_string!=active_string)||(float_menu!=0))
	{
		for(i=0;i<menu_size;i++)
		{
			if(active_string==i)
			{
				u8g2_SetFont(&u8g2, u8g2_font_10x20_t_cyrillic);
				u8g2_DrawUTF8(&u8g2, x_pos+((MAX_X-(str_size[i]*10))>>1), (y_pos+i*str_to_str)+float_menu, (char *)menu_string[i]);
			}
			else
			{
				u8g2_SetFont(&u8g2, u8g2_font_9x15_t_cyrillic);
				u8g2_DrawUTF8(&u8g2, x_pos+((MAX_X-(str_size[i]*9))>>1), (y_pos+i*str_to_str)+float_menu, (char *)menu_string[i]);
			}
		}
		old_active_string=active_string;

		// Рисуем
		u8g2_SendBuffer(&u8g2);
		u8g2_ClearBuffer(&u8g2);
	}
}

//------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------

void Parameter_screen_func(void)
{ Menu_GUI(0, 40, menu_parameter, menu_parameter_size, 17, Menu_parameter_num, Menu_num); }



// MENU
void Parameter_screen_S9(void)
{
	screen=Option_screen;
	old_active_string=0xFF;
	Menu_num=2;
}


// ENTER
void Parameter_screen_S4(void)
{
	switch(Menu_num)
	{
		// ДИСПЛЕЙ
		case 0: screen=Display_options_screen; old_active_string=0xFF; break;
		// ГРОМКОСТЬ
		case 1:
		break;
		// ДАТА И ВРЕМЯ
		case 2: screen=RTC_screen; old_active_string=0xFF; break;
		// АВТООТКЛЮЧЕНИЕ
		case 3: screen=Auto_Off_option_screen; old_active_string=0xFF; break;
		// НАСТРОЙКА ПОДКЛЮЧЕНИЯ
		case 4: break;
		// НАСТРОЙКА АЦП
		case 5: screen=ADC_options_screen; old_active_string=0xFF; break;
		// РЕЧЕВОЕ ИНФОРМИРОВАНИЕ
		case 6: break;
	}
	edit_param=0;
}

// DOWN
void Parameter_screen_S7(void)
{
		if(Menu_num>0)
		{
			Menu_num--;
			//float_menu=-2;
		}
		else Menu_num=Menu_parameter_num-1;
}

// UP
void Parameter_screen_S6(void)
{
		if(Menu_num<(Menu_parameter_num-1))
		{
			Menu_num++;
			//float_menu=2;
		}
		else Menu_num=0;
}


//------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------

void Option_screen_func(void)
{ Menu_GUI(0, 16, menu_option, menu_option_size, 17, Menu_option_num, Menu_num); }


// MENU
void Option_screen_S9(void)
{
		screen=Main_screen;
		old_active_string=0xFF;
}

// DOWN
void Option_screen_S7(void)
{
		if(Menu_num>0)Menu_num--; else Menu_num=Menu_option_num-1;
}

// UP
void Option_screen_S6(void)
{
		if(++Menu_num==Menu_option_num)Menu_num=0;
}

// ENTER
void Option_screen_S4(void)
{
	switch(Menu_num)
	{
		// ИЗМЕРЕНИЕ
		case 0: screen=Measure_options_screen; break;
		// 24 ВОЛЬТА
		case 1: break;
		// ПАРАМЕТРЫ
		case 2: screen=Parameter_screen; break;
		// О ПРИБОРЕ
		case 3: screen=About_options_screen; break;
		// КАЛИБРОВКА
		case 4: break;
		// ЛОГГИРОВАНИЕ
		case 5: screen=Logging_options_screen; break;
		// ТЕХ ДАННЫЕ
		case 6: break;
		// ТЕХ ПАРАМЕТРЫ
		case 7: break;
		// СЕРВИСНОЕ МЕНЮ
		case 8: break;
	}
	Menu_num=0;
	old_active_string=0xFF;
	edit_param=0;
}


//------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------









//------------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------------

uint8_t menu_delay = 0;
void menu_timer(void)
{
	if(++menu_delay==25)
	{
		if(float_menu>0)float_menu--;
		if(float_menu<0)float_menu++;
		menu_delay=0;
	}
}




