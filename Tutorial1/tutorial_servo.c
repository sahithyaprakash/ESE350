#include <unistd.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../Arduino_ATMega/uart.h"

#define OCR0A_VALUE 187//.5 , which is equivalent to a 3ms period

int main (void) {
	//initialize convenience variables
	// unsigned long clockFrequency = 16000000; //Hz
	// unsigned long frequency = 0;			//Hz

	//initialize controller state
	uart_init();
	// DDRB = 0xFF; 					// make all pins outputs
	TCCR0B = 0x04; 						//start timer, prescaler = 256
	// TIMSK1 = 0x01; 					//enable overflow interrupt
	// sei();

	DDRD |= 0x40; // set OC0A as output
	TCCR0A |= 0x80; // use non-inverting output
	TCCR0A |= 0x03; // use fast PWM mode
	TCCR0B |= 0x01; // turn the timer on

	OCR0A = 0xFF; // which is the OCR0A_VALUE

	// wait(1);

	// OCR0A = 0xE6; // 90% Duty Cycle

	// sleep(2);

	 OCR0B = 0xB4; // 70% Duty Cycle

	// pause(2);

	while(1) ;

    // PORTB = 0x04; //turn port 10 to high voltage
 	// PORTB = 0x08; //turn port 11 to high voltage
	// PORTB = 0x10; //turn port 12 to high voltage
	// PORTB = 0x20; //turn port 13 to high voltage
}
