#ifndef INC_ELSPIRE_OS_H_
#define INC_ELSPIRE_OS_H_

#include "main.h"


// Количество задач
#define num_of_tasks        	11

void Task_Quantity(uint8_t num);
void (*new_task[num_of_tasks+1])(void);
void ELSPIRE_OS_KERNEL(void);
void ELSPIRE_OS_INIT(void);
void ELSPIRE_OS_TIMER(void);
void sleep(unsigned int ms);
void Task_STOP(unsigned char task_num);
void Task_RUN(unsigned char task_num, unsigned int del);
void delay_ms(unsigned int ms);
void delay_us(unsigned long int us);

extern unsigned char RAM[100];

#endif /* INC_ELSPIRE_OS_H_ */
