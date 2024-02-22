/***********************************************
 * Univeristy of Southern Denmark
 * Embedded Programming (EMP)
 *
 * MODULENAME: main.c
 * PROJECT: template
 * DESCRIPTION: Empty project template
 * Change log:
 ***********************************************
 * Date of Change
 * YYMMDD
 * ----------------
 * 160224 MB Module created.
 **********************************************/

/***************** Header *********************/
/***************** Include files **************/
#include "emp_type.h"
#include "gpio.h"
#include "systick.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/***************** Defines ********************/
enum TraficLightState {
  NORMAL,
  NORWEGIAN,
  EMERGENCY,
};

enum Event {
  SINGLE,
  DOUBLE,
  LONG,
};

enum ButtonState {
  FIRST_PUSH,
  FIRST_RELEASE,
  SECOND_PUSH,
};

/***************** Constants ******************/
/***************** Variables ******************/
enum TraficLightState state = NORMAL;
volatile uint32_t ticks = 0;
/***************** Functions ******************/
void handle_event(enum Event event) {
  switch (event) {
  case SINGLE:
    state = NORWEGIAN;
    break;
  case DOUBLE:
    state = EMERGENCY;
    break;
  case LONG:
    state = NORMAL;
    break;
  }
}
/***************** End of module **************/

void GPIOF_Handler(void) {
  static enum ButtonState button_state = FIRST_PUSH;
  switch (button_state) {
  case FIRST_PUSH:
    ticks = 2000;
    setLEDColor(RED);
    if (ticks > 0) {
      ticks = 100;
      button_state = FIRST_RELEASE;
    } else {
      // Long push
      handle_event(LONG);
      button_state = FIRST_PUSH;
    }
    break;
  case FIRST_RELEASE:
    setLEDColor(GREEN);
    if (ticks > 1980) {
      button_state = FIRST_PUSH;
      break;
    } else if (ticks > 0) {
      ticks = 2000;
      button_state = SECOND_PUSH;
    } else {
      handle_event(SINGLE);
      button_state = FIRST_PUSH;
    }
    break;
  case SECOND_PUSH:
    setLEDColor(MAGENTA);
    if (ticks > 0) {
      handle_event(DOUBLE);
      button_state = FIRST_PUSH;
    } else {
      // Long push
      handle_event(LONG);
      button_state = FIRST_PUSH;
    }
    break;
  }

  GPIO_PORTF_ICR_R |= 0x10; // Clear interrupt bit
}

void SysTick_Handler(void) {
  if (ticks)
    ticks--;
}

int main(void) {
  setupPortF();
  init_systick();
  setLEDColor(WHITE);
  // Loop forever.
  while (1) {
    continue;
    switch (state) {
    case NORMAL:
      setLEDColor(BLUE);
      break;
    case NORWEGIAN:
      setLEDColor(RED);
      break;
    case EMERGENCY:
      setLEDColor(GREEN);
      break;
    default:
      setLEDColor(OFF);
      break;
    }
  }
  return (0);
}
