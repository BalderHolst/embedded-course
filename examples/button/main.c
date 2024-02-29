//////////////////////////////////////////////////////////////////////////////////////
///////////////////// DETTE PROGRAM VIRKER IKKE!!!! //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#include "tm4c123gh6pm.h"
#include "../../lib/gpio.h"
#include "../../lib/systick.h"
#include <stdint.h>

volatile uint32_t button_ticks = 0;

void SysTick_Handler(void) {
    if (button_ticks) button_ticks--;
}

enum ButtonState {
    IDLE,
    FIRST_PUSH,
    SECOND_PUSH,
    LONG_PRESS,
};

enum Event {
    SINGLE,
    DOUBLE,
    LONG,
};

#define LONG_PRESS_TIMEOUT 2000
#define DOUBLE_PRESS_TIMEOUT 300

void handle_event(Event event) {
    switch (event) {
        case SINGLE:
            setLEDColor(GREEN);
            break;
        case DOUBLE:
            setLEDColor(BLUE);
            break;
        case LONG:
            setLEDColor(RED);
            break;
    }
}

void GPIOF_Handler(void) {

    static enum ButtonState button_state = IDLE;
    static enum Event event = IDLE;

    setLEDColor(OFF);

    // Debounce
    button_ticks = 10;
    while (button_ticks);


    switch (button_state) {
        case IDLE:
            button_state = FIRST_PUSH;
            button_ticks = LONG_PRESS_TIMEOUT;
            break
        case FIRST_PUSH:
            if (button_ticks == 0) handle_event(LONG);
            else { 
                button_state = SECOND_PUSH;
                button_ticks = DOUBLE_PRESS_TIMEOUT;
                while (button_ticks);
                button_state = IDLE;
                if (button_state == FIRST_RELEASE) goto defer;
            }
            break;
        case SECOND_PUSH:
            if (button_ticks == 0) handle_event(SINGLE);
            else handle_event(DOUBLE);
            button_state = IDLE;
            break;
    }
    

    setLEDColor(RED);

defer:
    GPIO_PORTF_ICR_R |= 0x10;    // Clear interrupt bit
    GPIO_PORTF_IM_R |= (1 << 4); // Interrupt on PF4
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

    while (1) {
        setLEDColor(RED);
    }

    return (0);
}
