
#ifndef INC_TEMPERATURE_H_
#define INC_TEMPERATURE_H_

#include "stm32l4xx_hal.h"
#include "main.h"

void Temperature_process(void);

extern uint16_t temperature_buf[256];


#endif /* INC_TEMPERATURE_H_ */
