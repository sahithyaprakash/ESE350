#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"

/* 
 * The following code cycles through row values in a 0-hot state manner
 * and reads the values of the columns when the interrupt is triggered (every 4ms)
 */
unsigned int rowPorts[] = {0xFB, 0xF7, 0xEF, 0xDF}; // Values to set PORTC pins 2, 3, 4, 5 to low 
unsigned int columnPorts[] = {0x04, 0x08, 0x10, 0x20}; // Values to set PORTC pins 2, 3, 4, 5 to low 

//Index for keeping track of which row is currently set to 0
unsigned int rowIndex = 0;

// Note: when index is -1, there is no data.
int xIndex = -1; //rows
int yIndex = -1; //columns 

ISR(TIMER2_COMPA_vect) {
	//TURN LED ON
	//PORTB = 0x20;

	// switch 0-hot state to current row index
	PORTC = rowPorts[rowIndex];
	//update index based on row and column pressed
	for (int i = 0; i <= 3; i ++) {
		//if row port is low, then store the value in xIndex and store columnIndex = yIndex
		if (PIND & 0xFF == 0xFF) {
			//TURN LED ON
			PORTB = 0x20;
			break;
		} else if (PIND | columnPorts[i] == 0xFF) {
			printf("w");
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

	//initialize controller state
	uart_init();

	//Set data direction for Ports C and D
	DDRC = 0xFF; 					// make all C pins outputs
	DDRD = 0x00; 					// make all D pins inputs
	DDRB = 0xFF;					// make the LED an output
	PIND = 0xFF;					// make all input pins high

	//Setup the timer and CTC
	TCCR2A = 0x02;					// sets timer to CTC mode
	OCR2A = 0xFF;					// timer upper limit (resets after this value)
	TCCR2B = 0x01; 					// starts the timer with no prescaler
	TIMSK2 = 0x02; 					// unmasks the timer interrupt for compare
		
	sei();							//start interrupts

	while (1) {
		//Turn LED OFF
		PORTB = 0x00;
		if (xIndex >=  0 || yIndex >= 0) {
			printf("(%i, %i)", xIndex, yIndex);
			putchar(letters[xIndex][yIndex]);
			xIndex = -1;
			yIndex = -1;
		}
	}
}
