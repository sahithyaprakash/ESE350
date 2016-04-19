
#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdio.h>
#include <util/setbaud.h>

void uart_putch(char c, FILE *stream);
char uart_getch(FILE *stream);

FILE uart_out = FDEV_SETUP_STREAM(uart_putch, NULL, _FDEV_SETUP_WRITE);
FILE uart_in = FDEV_SETUP_STREAM(NULL, uart_getch, _FDEV_SETUP_READ);

void uart_init(void)
{

    #if USE_2X
    UCSR1A |= _BV(U2X1);
    #else
    UCSR1A &= ~(_BV(U2X1));
    #endif

    UCSR1B = (1<<RXEN1)|(1<<TXEN1);         // enable Rx & Tx
    UCSR1C=  (1 << UCSZ11) | ( 1 << UCSZ10) | (1 << UMSEL10);          // config USART; 8N1

    UBRR1H = UBRRH_VALUE; // set baud rate
    UBRR1L = UBRRL_VALUE;

    stdout = &uart_out;
    stdin = &uart_in;
}

void uart_flush(void)
{
    unsigned char dummy;
    while (UCSR1A & (1<<RXC1)) dummy = UDR1;
    dummy++; // stop compiler warning
}

void uart_putch(char ch,FILE *stream)
{
    if (ch == '\n')
    uart_putch('\r', stream);
    while (!(UCSR1A & (1<<UDRE1)));
    UDR1 = ch;
    //return 0;
}

char uart_getch(FILE *stream)
{
    unsigned char ch;
    while (!(UCSR1A & (1<<RXC1)));
    ch = UDR1;

    /* Echo the Output Back to terminal */
    uart_putch(ch,stream);

    return ch;
}

void ansi_cl(void)
{
    // ANSI clear screen: cl=\E[H\E[J
    putchar(27);
    putchar('[');
    putchar('H');
    putchar(27);
    putchar('[');
    putchar('J');
}

void ansi_me(void)
{
    // ANSI turn off all attribute: me=\E[0m
    putchar(27);
    putchar('[');
    putchar('0');
    putchar('m');
}

void uart_clr(void)
{
    ansi_me();
    ansi_cl();
    ansi_me();
    ansi_cl();
    uart_flush();   
}


#endif /* USART_H_ */