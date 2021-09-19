
#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "stm32l4xx_hal.h"
#include "main.h"

void menu_timer(void);

void Option_screen_func(void);
void Option_screen_S4(void);
void Option_screen_S6(void);
void Option_screen_S7(void);
void Option_screen_S9(void);

void Parameter_screen_func(void);
void Parameter_screen_S4(void);
void Parameter_screen_S9(void);
void Parameter_screen_S6(void);
void Parameter_screen_S7(void);


extern uint8_t old_active_string;
extern uint8_t Menu_num;


#endif /* INC_MENU_H_ */
