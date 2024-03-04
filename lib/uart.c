#include "uart0.h"

void uart_putc(char ch) {
    while(!uart0_tx_rdy());
    uart0_putc(ch);
}

void uart_getc(char ch) {
    while(!uart0_rx_rdy());
    uart0_getc(ch);
}

void uart_init() {
    uart0_init(9600, 8, 0, 0);
}

void uart_put_string(char* str) {
    while(*str) uart_putc(*str++);
}

void uart_put_stringln(char* str) {
    uart_put_string(str);
    uart_putc('\n');
}
