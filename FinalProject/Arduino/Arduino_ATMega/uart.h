
#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdio.h>
#include <util/setbaud.h>

#define BAUD_RATE 57600

void uart_init(void)
{
    UBRR1H = (((F_CPU/BAUD_RATE)/16)-1)>>8; // set baud rate
    UBRR1L = (((F_CPU/BAUD_RATE)/16)-1);
    UCSR1B = (1<<RXEN1)|(1<<TXEN1);         // enable Rx & Tx
    UCSR1C=  (1<<UCSZ11)|(1<<UCSZ10);          // config USART; 8N1
}

void uart_flush(void)
{
    unsigned char dummy;
    while (UCSR1A & (1<<RXC1)) dummy = UDR1;
    dummy++; // stop compiler warning
}

int uart_putch(char ch,FILE *stream)
{
    if (ch == '\n')
    uart_putch('\r', stream);
    while (!(UCSR1A & (1<<UDRE1)));
    UDR1=ch;
    return 0;
}

int uart_getch(FILE *stream)
{
    unsigned char ch;
    while (!(UCSR1A & (1<<RXC1)));
    ch=UDR1;

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