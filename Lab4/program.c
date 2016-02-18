#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "../Arduino_ATMega/uart.h"

#define FALSE 0
#define TRUE 1
#define ADC_READ_CHANNEL 1						//ADC read channle

//Input Capture
volatile unsigned int startTicks = 0; 			//time when rising edge is detected
volatile unsigned int endTicks = 0;				//time when falling edge is detected
volatile unsigned long width = 0; 				//number of ticks between rising and falling edge
volatile unsigned int overflows = 0; 			//number of overflows
volatile unsigned int isRisingEdge = TRUE;		//the input capture interrupt is reading a rising edge

//ADC
volatile unsigned int darkness = 0;				//the value read from the photoresistor

//Timer 1 has overflowed
ISR(TIMER1_OVF_vect) {
	overflows++; 						//increment overflow counter
}

//ADC Conversion has completed
ISR(ADC_vect) {
	darkness = ADC;			//store the value received from the photoresistor/adc conversion
	ADCSRA |= (1 << ADSC);	//starts a new conversion
}

//Timer 1 input capture has been triggered
ISR(TIMER1_CAPT_vect) {
	if (isRisingEdge) {

		//measures the the number of TCNT1 ticks between a rising edge and falling edge
		startTicks = ICR1; 				//get current counter value
		TIFR1 |= 0x20; 					//clear input capture flag
		TCCR1B &= 0xBF; 				//falling edge detection
		overflows = 0;					//reset number of overflows

		//update rising edge constant
		isRisingEdge = FALSE;
	} else {

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



void waitForDuration(unsigned long durationInMicroseconds) {

	unsigned long numOverflows = ((long) 16u * durationInMicroseconds) / 65536u;
	unsigned int targetOCR1A = ((long) 16u * durationInMicroseconds) - (numOverflows * 65535u);

	TIFR1 |= 0x20;														//clears the input capture flag
	TCCR1B |= 0x01;														//Make sure the clock is running
	overflows = 0;
	OCR1A = targetOCR1A;												//Convert to number of ticks
	while ((overflows < numOverflows) || ((TIFR1 | 0xFD) != 0xFF)); 	//Wait for durationInMicroseconds
}

//Generates a pulse of the given duration in microseconds using polling
//DO NOT CALL THIS IN AN INTERRUPT
void generatePulseOfDuration(int durationInMicroseconds) {
	DDRB = 0xFF;						//Set ports as outputs
	PORTB = 0xFF;						//Make rising edge
	waitForDuration(5);
	PORTB = 0x00; 						//Set pins to zero
}

int main (void) {
	//initialize controller state
	uart_init();

	//initialize local variables
	uint8_t channel = ADC_READ_CHANNEL;			//grab a local variable for the ADC channel
	unsigned long clockFrequencyADC = 250000; 	//the frequency of the ADC clock
	unsigned int frequencies[9] = {1000, 1125, 1250, 1375, 1500, 1625, 1750, 1875, 2000}; // in Hz

	//Setup data directions
	DDRD = 0xFF; 						//set to output
	DDRB = 0xFF;						//Set ports as outputs

	//Setup Input Capture for Timer 1
	TCCR1B |= 0x40;						//trigger on rising edge
	TIFR1 = 0x20;						//clears the input capture flag

	//Setup General Clock Functions for Timer 1
	TCCR1B |= 0x01;						//start clock with no prescaler

	//Unmask Timer Interrupts for Timer 1
	TIMSK1 = 0x01; 						//unmasks interrupt for overflows
	TIMSK1 |= 0x20;						//unmasks interrupt for input capture

	//Setup PWM for Timer 0
	TCCR0A = 0x40;						//toggles pin OC0A (Pin 6) on output compare
	TCCR0A |= 0x02;						//set to CTC mode

	//Setup General Clock Function for Timer 0
	TCCR0B = 0x03;						//start clock with prescaler 64



	//Configure ADC
	ADCSRA = 0x07; 						//sets the ADC reference clock to 125kHZ
	ADMUX = 0x40;						//sets to 5V Reference
	ADCSRA |= 0x80; 					//powers on the ADC
	ADCSRA |= 0x40;						//first Conversion

	//Configure 
	ADMUX &= 0xF0;						//clear the older channel that was read
	ADMUX |= channel;					//defines the new ADC channel to be read
	ADCSRA |= (1 << ADSC);				//starts a new conversion

	//ADC Interrupt Setup
	ADCSRA |= 0x08;						//enables the ADC interrupt

	sei();								//start interrupt

	while (1) {
		if (width != 0) {
			printf("%u\n", width);
			width = 0;
		}
		if (darkness != 0) {
			//printf("%u\n", darkness);
			unsigned int index = darkness / 114;
			OCR0A = (clockFrequencyADC / (2 * frequencies[index]));
			darkness = 0;
		}
		TIMSK1 &= 0xDF;						//mask interrupt for input capture
		generatePulseOfDuration(5);
		waitForDuration(400);				//the device waits for 750 us, so wait some time
		TIMSK1 |= 0x20;						//unmasks interrupt for input capture
		DDRB = 0x00;
		TCCR1B |= 0x40;						//Trigger on rising edge
		//Wait for the chirp, the echo, and the input + a buffer of 200us
		waitForDuration(19000);
	}
}



















