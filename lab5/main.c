#include <emp_type.h>
#include "gpio.h"
#include "uart.h"

#include "tm4c123gh6pm.h"
#include "systick.h"
#include <stdint.h>

#define TIMEOUT 1000

volatile INT16U ticks;

void SysTick_Handler(void) {
    if (ticks) ticks--;
}

void setup() {
    setupPortF();
    setLEDColor(GREEN);
    init_systick();
    uart_init();
}

int main(void) {
    setup();

    while(1) {
        uart_put_stringln("Turning ON!\n");
        setLEDColor(BLUE);
        ticks = TIMEOUT;
        while (ticks);
        uart_put_stringln("Turning OFF...\n");
        setLEDColor(OFF);
        ticks = TIMEOUT;
        while (ticks);
    }

    return (0);
}
