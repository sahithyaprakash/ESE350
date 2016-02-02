#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"

#define MAX_OVERFLOWS 10000
int i = 0;
int overflows = 0;

ISR(TIMER1_OVF_vect) {
	//overflows ++;
	putchar('o');
}

int main (void) {
	//initialize convenience variables
	unsigned long clockFrequency = 16000000; //Hz
	unsigned long frequency = 0;			//Hz

	//initialize controller state
	uart_init();
	DDRB = 0xFF; 					// make all pins outputs
	TCCR1B = 0x05; 					//start timer, prescaler = 1024
	TIMSK1 = 0x01; 					//enable overflow interrupt
	sei();
	PORTB = 0xFF;

 //    PORTB = 0x04; //turn port 10 to high voltage
 //    PORTB = 0x08; //turn port 11 to high voltage
	// PORTB = 0x10; //turn port 12 to high voltage
	// PORTB = 0x20; //turn port 13 to high voltage

	while (overflows <= 100 * MAX_OVERFLOWS) {
		//PORTB = 0x04; //turn port 10 to high voltage
	};
}
