#ifndef __DELAY_USING_SYSTICK_H
#define __DELAY_USING_SYSTICK_H

#include "main.h"
extern void systick_init(void);
extern void start_systick(unsigned int value);
extern unsigned int get_flag_systick(void);
extern void delay_ms_systick(unsigned int value);
extern void delay_us_systick(unsigned int time);
#endif
