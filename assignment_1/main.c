#include "tm4c123gh6pm.h"
#include "systick.h"
#include <stdbool.h>
#include <stdint.h>

// Missing definitions in tm4c123gh6pm.h file
#define NVIC_INT_CTRL_UNPEND_SYST 0x02000000 // Unpend a systick int

#define doubleClickTimeout 100  // Timeout for double click
#define longPressTimeout   2000 // Timeout for long press
#define debounceTimeout    20   // Debounce timeout
#define autoModeInterval   200  // Auto mode interval

// LED colors
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

// Modes of the program
enum Mode {
  AUTO,
  MANUAL,
};

// Direction of the increment
enum Direction {
  UP = true,
  DOWN = false,
};

// Define the order of led colors
enum LEDColor colors[] = {OFF, GREEN, BLUE, CYAN, RED, YELLOW, MAGENTA, WHITE};

// Counter values. This is used as an intex into `colors`
int counter = 0;

// Initial contitions
enum Mode mode = MANUAL;
enum Direction direction = UP;

// The ticks counter is used for timing and updated by the `systick.h` module
extern volatile uint8_t ticks;

// Check if the button is currently pressed. Returns 0 if not.
int button_pressed() {
    return ~(GPIO_PORTF_DATA_R) & 0b00010000;
}

// Get the modulo of a number (even negative ones)
int mod(int x, int N) { return (x % N + N) % N; }

// Increment or decrement the counter
void ment() {
  if (direction) counter++;
  else counter--;
  counter = mod(counter, 8);
}

// Set the color of the LED
void setLEDColor(enum LEDColor color) {
  GPIO_PORTF_DATA_R = color << 1;
} 

void setup() {
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

  init_systick();
}

int main(void) {
  setup();

  // Loop forever.
  while (1) {
    if (button_pressed()) { // If the button is pressed
      ticks = 0;

      // Wait for button release
      while (button_pressed());
        
      int elapsed_ticks = ticks; // Store the current value of ticks

      // First check for long press

      if (elapsed_ticks > longPressTimeout) {
        // If the button is pressed for more than 2 seconds

        direction = !direction;
        
        continue; // Skip the rest of the main loop
      }

      // Then check for single or double click
      if (elapsed_ticks > debounceTimeout) {
        // Handle debouncing

        // If auto mode is enabled, disable it
        if (mode == AUTO) {
          mode = MANUAL;
          continue; // Skip the rest of the main loop
        }

        // Check for another click within the double click timeout
        bool doubleClick = false;

        // Reset timer
        ticks = 0;

        // Check if the button is pressed again after the debounce timeout
        while (ticks < doubleClickTimeout) {

          if (button_pressed() && ticks > debounceTimeout) {

            // Wait for button release
            while (button_pressed());

            if (mode == MANUAL) mode = AUTO;
            else mode = MANUAL;

            doubleClick = true;
            break;
          }
        }

        // If it is not a double click, then it was a single click
        if (!doubleClick) {
          ment(); // Increment or decrement counter on single click
        }

      }
    }

    // If the button is not pressed
    // Set the color of the LED
    setLEDColor(colors[counter]);

    if (mode == AUTO) {
        if (ticks > autoModeInterval) {
            ticks = 0;
            ment();
        }
    }
  }
  return (0);
}
