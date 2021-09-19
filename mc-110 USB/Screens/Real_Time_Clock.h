
#ifndef INC_REAL_TIME_CLOCK_H_
#define INC_REAL_TIME_CLOCK_H_



#include "stm32l4xx_hal.h"
#include "main.h"

extern uint8_t year, month, day, hour, minute, sec;

void Battery_process(void);
void RTC_process(void);
void RTC_view(void);
void RTC_set(uint8_t rtc_year, uint8_t rtc_month, uint8_t rtc_day, uint8_t rtc_hour, uint8_t rtc_min, uint8_t rtc_sec);
void RTC_screen_func(void);
void RTC_screen_S8(void);
void RTC_screen_S4(void);
void RTC_screen_S7(void);
void RTC_screen_S6(void);
void RTC_screen_S5(void);
void RTC_screen_S9(void);

#endif /* INC_REAL_TIME_CLOCK_H_ */
