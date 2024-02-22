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
#define DOUBLE_CLICK_TIMEOUT 100 // Timeout for double click
#define LONG_PRESS_TIMEOUT 2000  // Timeout for long press
#define DEBOUNCE_TIMEOUT 10      // Debounce timeout

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
volatile uint32_t button_ticks = 0;
volatile uint32_t ticks = 0;
/***************** Functions ******************/
/***************** End of module **************/
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
  ticks = 0;
}
void process_press() {
  button_ticks = LONG_PRESS_TIMEOUT;
  // Temporary disable the interrupt on PF4
  while (button_sw1_pressed()) {
  }
  uint32_t current_ticks = button_ticks;
  if (current_ticks > (LONG_PRESS_TIMEOUT - DEBOUNCE_TIMEOUT)) {
    // Filter out debouncing
  } else if (current_ticks == 0) {
    // Long press
    handle_event(LONG);
  } else {

    // Short press

    // Reset the counter
    button_ticks = DOUBLE_CLICK_TIMEOUT;

    enum Event event = SINGLE;
    // Wait for a potential double click
    while (button_ticks) {
      if (button_sw1_pressed() &&
          button_ticks < (DOUBLE_CLICK_TIMEOUT - DEBOUNCE_TIMEOUT)) {
        event = DOUBLE;
        // setLEDColor(GREEN);
        while (button_sw1_pressed()) {
          // Wait for the button to be released
        }
        break;
      }
    }
    // Handle the press type
    handle_event(event);
  }
}

void GPIOF_Handler(void) { // Check if the interrupt is for rising edge (Press)
  GPIO_PORTF_IM_R &= ~(1 << 4); // Disable interrupt on PF4
  process_press();
  // Re-enable the interrupt on PF4
  GPIO_PORTF_ICR_R |= 0x10;    // Clear interrupt bit
  GPIO_PORTF_IM_R |= (1 << 4); // Interrupt on PF4
}

void SysTick_Handler(void) {
  if (button_ticks)
    button_ticks--;
  if (ticks)
    ticks--;
}

void norwegian_lights() {
  if (!ticks) {
    GPIO_PORTF_DATA_R |= 0b00001010;
    ticks = 1000;
    GPIO_PORTF_DATA_R ^= 0b00000100;
  }
}

enum NormalLightState {
  STOP,
  READY,
  GO,
  YIELD,
};

void normal_lights() {
  static enum NormalLightState light_state = STOP;

  switch (light_state) {
  case STOP:
    GPIO_PORTF_DATA_R = ~0b00000010;
    ticks = 2000;
    while (ticks)
      ;
    light_state = READY;
    break;
  case READY:
    GPIO_PORTF_DATA_R = ~0b00000110;
    ticks = 500;
    while (ticks)
      ;
    light_state = GO;
    break;
  case GO:
    GPIO_PORTF_DATA_R = ~0b00001000;
    ticks = 2000;
    while (ticks)
      ;
    light_state = YIELD;
    break;
  case YIELD:
    GPIO_PORTF_DATA_R = ~0b00000100;
    ticks = 500;
    while (ticks)
      ;
    light_state = STOP;
    break;
  }
}
void setup() {
  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R = 0b00001110;
  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R = 0b00011110;
  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = 0b00010000;

  // Set the interrupt type for PF4 (SW1)
  NVIC_EN0_R |= (1 << (INT_GPIOF - 16)); // enable interrupt in NVIC
  NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0x00A00000; // priority 5
  GPIO_PORTF_IS_R |= 0b00010000; // PF4 is edge-sensitive
  // GPIO_PORTF_IBE_R |= 0b00010000; // PF4 is both edges
  GPIO_PORTF_IEV_R &= ~0b00010000; // PF4 falling edge event
  GPIO_PORTF_IM_R = 0b00010000;    // Interrupt on PF4
  GPIO_PORTF_ICR_R = 0b00010000;   // clear flag4
}
int main(void) {
  setup();
  init_systick();
  // Loop forever.
  while (1) {
    // continue;
    switch (state) {
    case NORMAL:
      normal_lights();
      break;
    case NORWEGIAN:
      norwegian_lights();
      break;
    case EMERGENCY:
      GPIO_PORTF_DATA_R |= 0b00001100;
      GPIO_PORTF_DATA_R &= ~0b00000010;
      break;
    default:
      setLEDColor(WHITE);
      break;
    }
  }
  return (0);
}
