
#ifndef INC_DISPLAY_OPTIONS_H_
#define INC_DISPLAY_OPTIONS_H_


#include "stm32l4xx_hal.h"
#include "main.h"


void Display_options_screen_func(void);
void Display_options_screen_S6(void);
void Display_options_screen_S7(void);
void Display_options_screen_S8(void);
void Display_options_screen_S5(void);
void Display_options_screen_S9(void);
void Display_options_screen_S4(void);
void Light_OFF_process(void);

extern uint8_t light_sec, light_min;
extern uint8_t temp_light;

#endif /* INC_DISPLAY_OPTIONS_H_ */
