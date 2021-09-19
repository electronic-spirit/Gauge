
#include "ELSPIRE_OS.h"

#define wait 0

unsigned int tasks_timer[num_of_tasks];
unsigned char task = wait;
unsigned char task_ready_cnt=0;
unsigned int delay_count=0;
unsigned char RAM[100];
unsigned int j=0;

void wait_scheduler(void)
{
	if(tasks_timer[task_ready_cnt]==0)task=task_ready_cnt+1;
	if(++task_ready_cnt==num_of_tasks)task_ready_cnt=0;
}

void ELSPIRE_OS_KERNEL(void)
{
	new_task[0]=wait_scheduler;
	while (1) for(j=0;j<=num_of_tasks;j++)if(task==j)new_task[j]();
}

void ELSPIRE_OS_INIT(void)
{
	unsigned char i;
	for(i=1;i<=num_of_tasks;i++)
	tasks_timer[i]=0xFFFF;
}

void ELSPIRE_OS_TIMER(void)
{
	unsigned char i = 0;

	for(i=0;i<num_of_tasks;i++)
	{
		if(tasks_timer[i]>0)
		{
			if(tasks_timer[i]!=0xFFFF) tasks_timer[i]--;
		}
	}
	// Задержка
	if (delay_count > 0)
	{ delay_count--; }
}

void sleep(unsigned int ms)
{
	tasks_timer[task-1] = ms;
	task = wait;
}

void Task_STOP(unsigned char task_num)
{
	tasks_timer[task_num-1]=0xFFFF;
	task=wait;
}

void Task_RUN(unsigned char task_num, unsigned int del)
{
	tasks_timer[task_num-1]=del;
	task=wait;
}

void delay_ms(unsigned int ms)
{
	delay_count = ms;
	while (delay_count){}
}

void delay_us(unsigned long int us)
{ while (us--); }

