#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "../Arduino_ATMega/uart.h"

unsigned int diff, edge1, edge2, overflows;
unsigned long pulse_width;

ISR(TIMER1_CAPT_vect) {

	// TCCR1B ^= 0x40; //triggers the IRS when there is a rising edge of the pin voltage

	// if (PINB & 1) {
	// 	PORTB |= 0x20; //turn LED ON
	// } else {
	// 	PORTB &= 0xDF; //turn LED OFF
	// }
}

ISR(TIMER1_OVF_vect) {
	overflows++; //increment overflow counter
}

int main (void) {
	uart_init();
	// DDRB = 0xFF;
	// DDRC = 0xFF;
	TCCR1B |= 0x03; //start timer, prescaler = 64
	TCCR1B |= 0x40; //capture rising edges
	overflows = 0;
	TIMSK1 |= 0x01; //enable overflow interrupt
	TIFR1 |= 0x20; //clear input capture flag

	while (!(TIFR1 & 0x20)); //wait until an edge
	sei();
	edge1 = ICR1; //save time of first edge
	TCCR1B &= 0xBF; //capture falling edges
	TIFR1 |= 0x20; //clear input capture flag

	while(TIFR1 & 0x20); //wait until falling edge
	cli(); //disable interrupts
	edge2 = ICR1; //save time of first edge

	if (edge2 < edge1) {
		overflows --;
	}

	diff = edge2 - edge1;
	pulse_width = (long) overflows * 65536u + (long)diff;

	printf("pulse width = 4us x %l\n", pulse_width);
	for (;;);

	TIMSK1 |= 0x20; //enables the interrupt
	TCCR1B |= 0x40; //triggers the IRS when there is a rising edge of the pin voltage
	TCCR1B &= 0xF9; //sets no prescaling for the clock
	while(1) {

	}
}
