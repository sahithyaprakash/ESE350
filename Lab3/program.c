#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"

unsigned long half_period = 0;
unsigned long duration = 0; //ms
unsigned long overflows = 0;

ISR(TIMER1_COMPA_vect) {
	OCR1A += half_period;
}

ISR(TIMER1_OVF_vect) {
	overflows ++;
	if (overflows >= duration / 4.096) {
		overflows = 0;
		duration = 0;
	}
}

int main (void) {
	//initialize convenience variables
	unsigned long clockFrequency = 16000000; //Hz
	unsigned long frequency = 0;			//Hz

	//initialize controller state
	uart_init();
	DDRB = 0xFF; 					// make all pins outputs
	TCCR1B = 0x01; 					// starts the timer with no prescaler
	TIMSK1 = 0x02; 					// unmasks the timer interrupt for compare
	TIMSK1 |= 0x01;					// unmasks the timer interrupt for overflow
	
	//get user input
	scanf("%lu %lu", &frequency, &duration);
	printf("%lu %lu", frequency, duration);

	//calculate half period
	half_period = clockFrequency / frequency / 2;
	
	TCCR1A |= 0x40; 				// toggles pin 9 when a compare match is made.
	OCR1A += TCNT1 + half_period; 	//starts at low and waits until the first half period is reached
	
	sei();							//start interrupts

	while (1) {
		//TODO: check for errors
		if (frequency == 0 || duration == 0) {
			TCCR1A = 0x80;
			continue;
		}
	}
}
