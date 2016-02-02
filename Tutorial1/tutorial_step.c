#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"

#define MAX_OVERFLOWS 1
int i = 0;
int overflows = 0;

//Brown = Port 10
//Black = Port 11
//Orange = Port 12
//Yellow = Port 13

ISR(TIMER1_OVF_vect) {
	i ++;
	printf("%i", i);
	if (i == 1) {
		PORTB = 0x10; //turn port 12 to high voltage
		PORTB |= 0x20; //turn port 13 to high voltage
	} else if (i == 2) {
		PORTB = 0x00;
		PORTB = 0x04; //turn port 10 to high voltage
		PORTB |= 0x10; //turn port 12 to high voltage
	} else if (i == 3) {
		PORTB = 0x04; //turn port 10 to high voltage
		PORTB |= 0x08; //turn port 11 to high voltage
	} else {
		PORTB = 0x08; //turn port 11 to high voltage
		PORTB |= 0x20; //turn port 13 to high voltage
		i = 0;
	}
	//overflows = 0;
}

int main (void) {

	//initialize controller state
	uart_init();
	DDRB = 0xFF; 					// make all pins outputs
	TCCR1B = 0x02; 					//start timer, prescaler = 1024
	TIMSK1 = 0x01; 					//enable overflow interrupt
	sei();

	// PORTB = 0x04; //turn port 10 to high voltage
	// PORTB = 0x08; //turn port 11 to high voltage
	// PORTB = 0x10; //turn port 12 to high voltage
	// PORTB = 0x20; //turn port 13 to high voltage

	while (1);
}
