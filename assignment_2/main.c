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
/***************** Constants ******************/
/***************** Variables ******************/
enum State state = two;
/***************** Functions ******************/
/***************** End of module **************/

void GPIOF_Handler(void) {
  // Check if the interrupt is for rising edge (Press)
  state = three;

  GPIO_PORTF_ICR_R |= 0x10; // Clear interrupt bit
}

void SysTick_Handler(void) { state = one; }

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
