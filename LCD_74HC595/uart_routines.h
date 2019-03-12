
#ifndef UART_ROUTINES_H_
#define UART_ROUTINES_H_

#ifndef BAUD
#warning "setting BAUD to 9600"
#define BAUD 9600
#endif

#include <avr/io.h>
#include <util/setbaud.h>

void uart_init(void);
int uart_putc(unsigned char c);
void uart_puts (char *s);
void uart_print_byte(unsigned char byte);

#endif
