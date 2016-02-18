#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "../Arduino_ATMega/uart.h"

#define FALSE 0
#define TRUE 1

volatile unsigned int startTicks = 0; 			//time when rising edge is detected
volatile unsigned int endTicks = 0;
volatile unsigned long width = 0; 				//number of ticks between rising and falling edge
volatile unsigned int overflows = 0; 			//number of overflows
volatile unsigned int isRisingEdge = TRUE;

ISR(TIMER1_OVF_vect) {
	overflows++; 						//increment overflow counter
}

ISR(TIMER1_CAPT_vect) {
	if (isRisingEdge) {

		//TEST
		PORTD = 0xFF;


		//measures the the number of TCNT1 ticks between a rising edge and falling edge
		startTicks = ICR1; 				//get current counter value
		TIFR1 |= 0x20; 					//clear input capture flag
		TCCR1B &= 0xBF; 				//falling edge detection
		overflows = 0;					//reset number of overflows

		//update rising edge constant
		isRisingEdge = FALSE;
	} else {

		//TEST
		PORTD = 0x00;

		endTicks = ICR1;				//capture value of TCNT1
		TIFR1 |= 0x20; 					//clear input capture flag
		isRisingEdge = TRUE;

		//Modify overflows based on number of ticks
		if (endTicks < startTicks) {
			overflows --;
		}

		unsigned int diff = endTicks - startTicks;			//Get the difference in the ticks
		width = (long) overflows * 65535u + (long) diff;	//get the width of the pulse
	}
}



void waitForDuration(unsigned long duration_in_microseconds) {

	unsigned long num_overflows = ((long) 16u * duration_in_microseconds) / 65536u;
	unsigned int target_OCR1A = ((long) 16u * duration_in_microseconds) - (num_overflows * 65535u);

	TIFR1 |= 0x20;														//clears the input capture flag
	TCCR1B |= 0x01;														//Make sure the clock is running
	overflows = 0;
	OCR1A = target_OCR1A;												//Convert to number of ticks
	while ((overflows < num_overflows) || ((TIFR1 | 0xFD) != 0xFF)); 	//Wait for duration_in_microseconds
}

//Generates a pulse of the given duration in microseconds using polling
//DO NOT CALL THIS IN AN INTERRUPT
void generatePulseOfDuration(int duration_in_microseconds) {
	DDRB = 0xFF;						//Set ports as outputs
	PORTB = 0xFF;						//Make rising edge
	waitForDuration(5);
	PORTB = 0x00; 						//Set pins to zero
}

int main (void) {
	//initialize controller state
	uart_init();

	//Setup temp pin
	DDRD = 0xFF; 						//set to output

	DDRB = 0xFF;						//Set ports as outputs

	//Setup Input Capture
	TCCR1B |= 0x40;						//Trigger on rising edge
	TIFR1 = 0x20;						//clears the input capture flag

	//Setup Output Compare
	//TCCR1A = 0x80; 						//Clears OC1A (PB1) on compare match

	//Setup General Clock Functions
	TCCR1B |= 0x01;						//start clock with no prescaler

	//Unmask Interrupts
	TIMSK1 = 0x01; 						//unmasks interrupt for overflows
	TIMSK1 |= 0x20;						//unmasks interrupt for input capture

	sei();								//start interrupt

	while (1) {
		if (width != 0) {
			printf("%u\n", width);
			width = 0;
		}
		TIMSK1 &= 0xDF;						//mask interrupt for input capture
		generatePulseOfDuration(5);
		waitForDuration(500);				//the device waits for 750 us, so wait some time
		TIMSK1 |= 0x20;						//unmasks interrupt for input capture
		DDRB = 0x00;
		TCCR1B |= 0x40;						//Trigger on rising edge
		//Wait for the chirp, the echo, and the input + a buffer of 200us
		waitForDuration(19000);
	}
}



















