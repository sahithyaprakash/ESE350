#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <stdio.h>
#include <util/setbaud.h>

void uart_putchars(char c, FILE *stream);
char uart_getchars(FILE *stream);

FILE uart_out = FDEV_SETUP_STREAM(uart_putchars, NULL, _FDEV_SETUP_WRITE);
FILE uart_in = FDEV_SETUP_STREAM(NULL, uart_getchars, _FDEV_SETUP_READ);

void uart_inits() {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
            
    #if USE_2X
    UCSR0A |= _BV(U2X0);
    #else
    UCSR0A &= ~(_BV(U2X0));
    #endif
    
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */

    stdout = &uart_out;
    stdin = &uart_in;
}

void uart_putchars(char c, FILE *stream) {
    if (c == '\n') {     
        uart_putchars('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

char uart_getchars(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

#endif