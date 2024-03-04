#ifndef UART
#define UART

void uart_putc(char ch);
char uart_getc();
void uart_init();
void uart_put_string(char* str);
void uart_put_stringln(char* str);

#endif
