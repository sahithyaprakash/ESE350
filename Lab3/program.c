#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"
/* 
 * Variables for the generation of square waves
*/
unsigned long clock_frequency = 16000000;

/* 
 * The following code cycles through row values in a 0-hot state manner
 * and reads the values of the columns when the interrupt is triggered (every 4ms)
 */
unsigned int rowPorts[] = {0xFB, 0xF7, 0xEF, 0xDF}; // Values to set PORTC pins 2, 3, 4, 5 to low 
unsigned int columnPorts[] = {0x38, 0x34, 0x2C, 0x1C}; // Values to check PIND pins 2, 3, 4, 5 against

//Index for keeping track of which row is currently set to 0
unsigned int rowIndex = 0;

// Note: when index is -1, there is no data.
int xIndex = -1; //rows
int yIndex = -1; //columns 

ISR(TIMER2_COMPA_vect) {
	// switch 0-hot state to current row index
	PORTC = rowPorts[rowIndex];
	//update index based on row and column pressed
	for (int i = 0; i <= 3; i ++) {
		//if row port is low, then store the value in xIndex and store columnIndex = yIndex
		if ((PIND & 0x3C) == 0x3C) {
			break;
		} else if ((PIND & 0x3C) == columnPorts[i]) { 
			//store key
			yIndex = i;
			xIndex = rowIndex;
		}
	}

	//update row index for next time. 
	if (rowIndex >= 3) {
		rowIndex = 0;
	} else {
		rowIndex ++;
	}
}

int main (void) {
	//initialize convenience variables
	char letters[4][4] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
	unsigned int rowFrequencies[4] = {697, 770, 852, 941};
	unsigned int columnFrequencies[4] = {1209, 1336, 1477, 1633};

	//initialize controller state
	uart_init();

	//Set data direction for Ports C and D
	DDRB = 0xFF;					// make all B pins outputs
	DDRC = 0xFF; 					// make all C pins outputs
	DDRD = 0x00; 					// make all D pins inputs
	DDRD |= 0x40; 					// make PD6 an output
	

	//Setup the timer and CTC
	//Setup timers for frequency generation


	//Timer 0
	TCCR0A = 0x02;					// enable CTC mode
	TCCR0A |= 0x40;					// toggle on compare match
	TCCR0B = 0x01;					// starts the timer with no prescaler
	OCR0A = 0x00;

	//Timer 1
	TCCR1A = 0x00;					// enable CTC mode
	TCCR1B = 0x08;					// enables CTC mode
	TCCR1A |= 0x40;					// toggle on compare match
	TCCR1B |= 0x01;					// starts the timer with no prescaler
	OCR1A = 0x00;



	//The code below uses fast PWM mode to do the frequency generations
	/*
	//Timer 0 
	TCCR0A = 0x80;					// uses non-inverting output (Clears OC0A on compare match)
	TCCR0A |= 0x03;					// use fast PWM 
	TCCR0B = 0x01; 					// start clock 0 with no prescaler
	TCCR0B |= 0x08;					// reset based on OCR0A
	OCR0B = 0x00;					// set to 50% duty cycle

	//Timer 1
	TCCR1A = 0x80;					// uses non-inverting output (Clears OC1A on compare match)
	TCCR1A |= 0x03;					// use fast PWM 
	TCCR1B = 0x01; 					// start clock 1 with no prescaler
	TCCR1B |= 0x18;					// reset based on OCR1A
	OCR1B = 0x00;					// set to 50% duty cycle
	*/

	/*
	 * The following setup code sets up the interception of a buton being pressed on
	 * the keypad. Do not delete unless you no longer need this funcitonality.
	 * Note that this uses Timer 2, so make sure there is nothing setting up Timer 2
	 * in the previous setup code. 
	*/

	//Setup timers for intercepting when a key is pressed
	TCCR2A = 0x02;					// sets timer to CTC mode
	OCR2A = 0xFF;					// timer upper limit (resets after this value)
	TCCR2B = 0x07; 					// starts the timer with 1024 prescaler
	TIMSK2 = 0x02; 					// unmasks the timer interrupt for compare
		
	sei();							//start interrupts

	while (1) {
		//printf("(%i, %i)", xIndex, yIndex);
		if (xIndex >=  0 && yIndex >= 0 && xIndex < 4 && yIndex < 4) {
			//printf("(%i, %i)", xIndex, yIndex);
			//printf("%c", letters[xIndex][yIndex]);
			OCR0A = (clock_frequency / columnFrequencies[xIndex]) * 0.5;
			OCR1A = (clock_frequency / rowFrequencies[yIndex]) * 0.5;

			//reset variables
			// xIndex = -1;
			// yIndex = -1;
		}
	}
}
