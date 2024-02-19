// Include guard for systick.h
#ifndef SYSTICK_H
#define SYSTICK_H
// Implement EVERYTHING for systick

#include "tm4c123gh6pm.h"
// Volatile because it is changed in an ISR

#define SYSTICK_RELOAD_VALUE 16000 // SysTick interrupt every 1ms

void init_systick(void);
#endif
