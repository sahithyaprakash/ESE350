#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "../Arduino_ATMega/uart.h"

#define DOT_WIDTH_MIN 7500 //250 * 30 // ticks
#define DOT_WIDTH_MAX 50000 //250 * 200 //ticks 
#define SPACE_WIDTH_MIN 100000 //250 * 400 //ticks 

unsigned long diff, edge1, edge2, overflows;
unsigned long pulse_width;

// ISR(TIMER1_CAPT_vect) {

// 	// TCCR1B ^= 0x40; //triggers the IRS when there is a rising edge of the pin voltage

// 	// if (PINB & 1) {
// 	// 	PORTB |= 0x20; //turn LED ON
// 	// } else {
// 	// 	PORTB &= 0xDF; //turn LED OFF
// 	// }
// }

ISR(TIMER1_OVF_vect) {
	overflows++; 	//increment overflow counter
}

void turnOffAllLEDs() {
	PORTB |= 0x10; //turn port 12 to high voltage
	PORTB |= 0x08; //turn port 11 to high voltage
	PORTB |= 0x20; //turn port 13 to high voltage
}

void sevenSegmentDot() { //grounds port 12
	turnOffAllLEDs();
	PORTB  &= 0xEF; 
}

void sevenSegmentDash() { //grounds port 11
	turnOffAllLEDs();
	PORTB &= 0xF7;
}

void sevenSegmentSpace() { //grounds port 13
	turnOffAllLEDs();
	PORTB &= 0xDF;
}

int main (void) {
	uart_init();
	DDRB = 0xFF;
	TCCR1B = 0x03; 				//start timer, prescaler = 64
	TIMSK1 = 0x01; 				//enable overflow interrupt
	TIFR1 = 0x20; 				//clear input capture flag
	TCCR1B |= 0x40; 			//capture rising edges
	overflows = 0;
	sei(); 						//accept all interrupts
	
	while (1) {
		TIFR1 |= 0x20; 				//clear input capture flag
		TCCR1B |= 0x40; 			//capture rising edges
		overflows = 0;

		while(!(TIFR1 & 0x20)); 	//wait until a rising edge

		edge1 = ICR1; 				//save time of first edge
		TCCR1B &= 0xBF; 			//capture falling edges of the input
		TIFR1 |= 0x20; 				//clear input capture flag
		if (overflows >= 2 || (overflows > 0 && edge1 > 34449)) {
			putchar('s');
			sevenSegmentSpace();
		}
		overflows = 0;
		
		
		while(!(TIFR1 & 0x20)); 	//wait until falling edge
		edge2 = ICR1; 				//save time of second edge

		if (edge2 < edge1) {
			overflows--;
		}

		diff = edge2 - edge1;
		pulse_width = (long) overflows * 65536u + (long) diff;
		//printf("%u", pulse_width);
		//if it's depressed or no.
		if (pulse_width > DOT_WIDTH_MIN && pulse_width < DOT_WIDTH_MAX) {
			putchar('.'); //put dot
			sevenSegmentDot();
		} else if (pulse_width > DOT_WIDTH_MAX) {
			putchar('-'); //put dash
			sevenSegmentDash();
		}
	}
}
