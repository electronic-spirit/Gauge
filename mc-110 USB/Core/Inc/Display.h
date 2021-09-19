
#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_


#include "stm32l4xx_hal.h"
#include "main.h"


#define MAX_Y 160
#define MAX_X 240



enum Screens{
	BIOS_screen,				// Технические параметры
	Main_screen,				// Главный экран
	Test_screen,				// Тестовый экран
	Off_screen,					// Экран выключения
	Option_screen,				// Экран основного меню
	Display_options_screen,		// Экран настройки дисплея
	ADC_options_screen,			// Экран настройки АЦП
	Measure_options_screen,		// Настройки измерения
	Parameter_screen,			// Меню параметры
	RTC_screen,					// Меню настройки времени и даты
	Auto_Off_option_screen,		// Настройка автоотключения
	About_options_screen,		// О приборе
	Logging_options_screen,		// Логгирование
};


void Display_init(void);
void Display_process(void);
void Menu_GUI(uint16_t x_pos, uint16_t y_pos, const unsigned char *menu_string[], uint8_t *str_size,
		uint8_t str_to_str, uint8_t menu_size, uint8_t active_string);
void DrawImage(const uint8_t *image, uint16_t x, uint16_t y, uint8_t inverse);
void Blink_Edit_Parameter_timer(void);
void Blink_Edit_Parameter(uint8_t x, uint8_t y, const char* text, uint8_t param_num, uint8_t curr_param);
void Gauge_loading(uint8_t*text, uint8_t percent);


extern uint8_t screen;
extern uint8_t edit_param;

extern const char *PGA_name[9];

#endif /* INC_DISPLAY_H_ */
