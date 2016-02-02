#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"

int i = 0;

ISR(TIMER1_OVF_vect) {
	//overflows ++;
	if (i = 0) {
		PORTB = 0x10; //turn port 12 to high voltage
		PORTB |= 0x20; //turn port 13 to high voltage
	} else if (i = 1) {
		PORTB = 0x04; //turn port 10 to high voltage
		PORTB |= 0x10; //turn port 12 to high voltage
	} else if (i = 2) {
		PORTB = 0x04; //turn port 10 to high voltage
    	PORTB |= 0x08; //turn port 11 to high voltage
	} else {
		i = 0;
		PORTB = 0x08; //turn port 11 to high voltage
		PORTB |= 0x20; //turn port 13 to high voltage
	}
}

int main (void) {
	//initialize convenience variables
	unsigned long clockFrequency = 16000000; //Hz
	unsigned long frequency = 0;			//Hz

	//initialize controller state
	uart_init();
	DDRB = 0xFF; 					// make all pins outputs
	TCCR1B = 0x03; 					//start timer, prescaler = 64
	TIMSK1 = 0x01; 					//enable overflow interrupt

 //    PORTB = 0x04; //turn port 10 to high voltage
 //    PORTB = 0x08; //turn port 11 to high voltage
	// PORTB = 0x10; //turn port 12 to high voltage
	// PORTB = 0x20; //turn port 13 to high voltage

	while (1) {
		//TODO: check for errors
		// if (frequency == 0 || duration == 0) {
		// 	TCCR1A = 0x80;
		// 	continue;
		// }
	}
}
