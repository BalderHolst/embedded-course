// Implement EVERYTHING for systick

#include <stdint.h>
#include "tm4c123gh6pm.h"

#define SYSTICK_RELOAD_VALUE 16000 // SysTick interrupt every 1ms
#define SYSTICK_PRIORITY     0x7E  // Priority of the systick interrupt


// Volatile because it is changed in an ISR
volatile uint32_t ticks = 0;


void IntSysTickHandler(void) {
  // Hardware clears systick int reguest
  ticks++; // Increment ticks every 1ms

  /* // Handle auto mode */
  /* if (mode == AUTO) { */
  /*   if (ticks % autoModeInterval == 0) // If 0.2 second has passed */
  /*     ment();                          // Increment or decrement counter */
  /* } */
}

void init_systick() {

  // Disable systick timer
  NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE);

  // Set current systick counter to reload value
  NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE;
  // Set Reload value, Systick reload register
  NVIC_ST_RELOAD_R = SYSTICK_RELOAD_VALUE;

  // Set systick priority to 0x10, first clear then set.
  NVIC_SYS_PRI3_R &= ~(NVIC_SYS_PRI3_TICK_M);
  NVIC_SYS_PRI3_R |=
      (NVIC_SYS_PRI3_TICK_M & (SYSTICK_PRIORITY << NVIC_SYS_PRI3_TICK_S));

  // Select systick clock source, Use core clock
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;

  // Enable systick interrupt
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;

  // Enable and start timer
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}
