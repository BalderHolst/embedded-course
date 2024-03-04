#include "uart0.h"

void uart_putc(char ch) {
    while(!uart0_tx_rdy());
    uart0_putc(ch);
}

char uart_getc() {
    while(!uart0_rx_rdy());
    return uart0_getc();
}

char * uart_get_string() {
    static char buffer[256];
    char *buffer_pointer = buffer;

    char current_char = '\0';

    do {
        current_char = uart_getc();
        *buffer_pointer = current_char;
        buffer_pointer++;
    } while(current_char != '\n' && current_char != '\r' && current_char != '\0');

    *buffer_pointer = '\0';

    return buffer;
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
