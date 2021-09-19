
#ifndef SCREENS_AUTO_OFF_OPTION_H_
#define SCREENS_AUTO_OFF_OPTION_H_


#include "stm32l4xx_hal.h"
#include "main.h"

void Auto_OFF_process(void);
void Auto_Off_option_screen_func(void);
void Auto_Off_option_screen_S9(void);
void Auto_Off_option_screen_S8(void);
void Auto_Off_option_screen_S5(void);



extern uint16_t off_sec;
extern uint16_t off_min;


#endif /* SCREENS_AUTO_OFF_OPTION_H_ */
