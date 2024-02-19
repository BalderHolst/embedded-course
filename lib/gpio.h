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

// Setup PORTF as output
void setupPortF();

// Set the color of the LED
void setLEDColor(enum LEDColor color);
