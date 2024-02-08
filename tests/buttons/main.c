#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>

enum LEDColor {
  OFF = 0b000,
  GREEN = 0b100,
  BLUE = 0b010,
  CYAN = 0b110,
  RED = 0b001,
  YELLOW = 0b101,
  MAGENTA = 0b011,
  WHITE = 0b111
};

int main(void) {
  int dummy;

  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

  // Do a dummy read to insert a few cycles after enabling the peripheral.
  dummy = SYSCTL_RCGC2_R;

  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R = 0x0E;

  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R = 0x1E;

  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = 0x10;

  bool increment = false;
  bool auto_mode = true;
  uint8_t counter = 0;
  // Loop forever.
  while (1) {
    if (GPIO_PORTF_DATA_R & 0x10 && !auto_mode) {
      if (increment) {
        counter = (counter + 1) % 8;
      } else {
        counter = (counter - 1) % 8;
      }
      while (GPIO_PORTF_DATA_R & 0x10) // Wait for the button to be released
        ;
    } else if (auto_mode) {
      if (increment) {
        counter = (counter + 1) % 8;
      } else {
        counter = (counter - 1) % 8;
      }
      for (int i = 0; i < 500000; i++) {
      }
    }
    switch (counter) {
    case 0:
      GPIO_PORTF_DATA_R = OFF << 1;
      break;
    case 1:
      GPIO_PORTF_DATA_R = GREEN << 1;
      break;
    case 2:
      GPIO_PORTF_DATA_R = BLUE << 1;
      break;
    case 3:
      GPIO_PORTF_DATA_R = CYAN << 1;
      break;
    case 4:
      GPIO_PORTF_DATA_R = RED << 1;
      break;
    case 5:
      GPIO_PORTF_DATA_R = YELLOW << 1;
      break;
    case 6:
      GPIO_PORTF_DATA_R = MAGENTA << 1;
      break;
    case 7:
      GPIO_PORTF_DATA_R = WHITE << 1;
      break;
    }
  }
  return (0);
}
