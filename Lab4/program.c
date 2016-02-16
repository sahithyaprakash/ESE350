#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "../Arduino_ATMega/uart.h"

#define FALSE 0
#define TRUE 1

unsigned int startTicks = 0; 			//time when rising edge is detected
unsigned int endTicks = 0;
volatile unsigned long width = 0; 				//number of ticks between rising and falling edge
volatile unsigned int overflows = 0; 			//number of overflows
unsigned int isRisingEdge = TRUE;


void setupForSendingPulse() {	
	printf("sp");
	cli();
	TCCR1A = 0x80; 						//Clears OC1A (PB1) on compare match and 
	TCCR1B = 0x01;						//Stops input capture 
	DDRB = 0xFF;						//Set Port B to output
	PORTB = 0x02;						//Set PB1 to high
	OCR1A = TCNT1 + 80; 				//set to match on 5us
	TIFR1 = 0x20;						//clears the input capture flag
	TIMSK1 = 0x01; 						//unmasks interrupt for overflows
	TIMSK1 |= 0x02; 					//unmasks interrupt for OC1A
	sei();
}

void setupForReceivingPulse() {
	printf("rp");
	isRisingEdge = TRUE;
	DDRB = 0x00;						//Set all B pins as inputs
	TCCR1A = 0x00;						//Disconnect OC
	TCCR1B |= 0x40;						//Trigger on rising edge
	TIFR1 = 0x20;						//Clears input capture flag
	TIMSK1 = 0x20;						//unmasks interrupt for input capture
	TIMSK1 |= 0x02; 					//unmasks interrupt for OC1A
	sei();	

}

ISR(TIMER1_OVF_vect) {
	overflows++; 						//increment overflow counter
}

ISR(TIMER1_CAPT_vect) {
	if (isRisingEdge) {
		//measures the the number of TCNT1 ticks between a rising edge and falling edge
		startTicks = ICR1; 				//get current counter value
		TCCR1B &= 0xBF; 				//falling edge detection
		TIFR1 |= 0x20; 					//clear input capture flag
		overflows = 0;					//reset number of overflows

		//update rising edge constant
		isRisingEdge = FALSE;
	} else {
		endTicks = ICR1;				//capture value of TCNT1

		//Modify overflows based on number of ticks
		if (endTicks < startTicks) {
			overflows --;
		}

		unsigned int diff = endTicks - startTicks;	//Get the difference in the ticks
		width = (long) overflows * 65535u + (long) diff;	//get the width of the pulse
	}


}

ISR(TIMER1_COMPA_vect) {
	cli();
	//sets timer 1 to receive ping
	setupForReceivingPulse();
}

int main (void) {
	//initialize controller state
	uart_init();

	setupForSendingPulse();

	//Setup timer 1 to generate ping
	TIMSK1 = 0x02; 						//unmasks interrupt for OC1A
	TIMSK1 |= 0x01; 					//unmasks interrupt for overflows
	TIMSK1 |= 0x20;						//unmasks interrupt for input capture
	TCCR1B |= 0x01;						//start clock with no prescaler
	TCCR1A = 0x80; 						//Clears OC1A (PB1) on compare match

	sei();								//start interrupt

	while (1) {
		if (width != 0) {
			//printf("%u\n", width);
			setupForSendingPulse();
			width = 0;
		} 
	}
}



















