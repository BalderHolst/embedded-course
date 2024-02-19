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
#include "../lib/emp_type.h"
#include "../lib/gpio.h"
#include "../lib/systick.h"
#include "../lib/tm4c123gh6pm.h"
#include <stdint.h>

/***************** Defines ********************/
enum State {
  one,
  two,
  three,
};

enum ButtonPress {
  NONE,
  SINGLE,
  DOUBLE,
  LONG,
};
/***************** Constants ******************/
/***************** Variables ******************/
enum State state = one;
volatile uint32_t ticks = 0;
/***************** Functions ******************/
/***************** End of module **************/

void GPIOF_Handler(void) {
  static enum ButtonPress button_press = SINGLE;
  while (69) {
    switch (button_press) {
    case SINGLE:
      break;
    case DOUBLE:
      break;
    case LONG:
      break;
    default:
      break;
    }
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
  // Loop forever.
  while (1) {
    switch (state) {
    case one:
      setLEDColor(BLUE);
      break;
    case two:
      setLEDColor(RED);
      break;
    case three:
      setLEDColor(GREEN);
      break;
    default:
      break;
    }
  }
  return (0);
}
