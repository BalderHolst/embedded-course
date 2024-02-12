#include "tm4c123gh6pm.h"
#include <stdint.h>

#define PORTF_INTERUPT_BIT 30

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

#define COLORS_LEN 8

enum LEDColor colors[] = {OFF, GREEN, BLUE, CYAN, RED, YELLOW, MAGENTA, WHITE};

int current_color = 0;


void setup() {
  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R = 0x0E;

  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R = 0x1E;

  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = 0x10;

  // Enable 
  NVIC_EN0_R = (1<<PORTF_INTERUPT_BIT);

  // Set interupt mask
  GPIO_PORTF_IM_R = 0x10;


}

void setLEDColor(enum LEDColor color) {
    GPIO_PORTF_DATA_R = color << 1;
}

void display_current_color() {
  enum LEDColor color = colors[current_color];
  setLEDColor(color);
}

void IntButtonHandler(void) {
    current_color = (current_color + 1) % COLORS_LEN;
    while (!(GPIO_PORTF_DATA_R & 0x10));
    GPIO_PORTF_ICR_R |= 0x10; // Clear interrupt bit
}

int main(void) {

  setup();

  // Loop forever.
  while (1) {
    display_current_color();
  }

  return (0);
}
