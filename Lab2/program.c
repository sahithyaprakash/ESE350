#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"

#define DOT_WIDTH_MIN 7500 //250 * 30 // ticks
#define DOT_WIDTH_MAX 50000 //250 * 200 //ticks 
#define SPACE_WIDTH_MIN 100000 //250 * 400 //ticks 

unsigned long diff, edge1, edge2, overflows;
unsigned long pulse_width;
char morseSymbolArray[6];

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

void addMorseSymbol(char c) {
	putchar(c);
	int i = strlen(morseSymbolArray);
	if (i >= 5) {
		return;
	}
	morseSymbolArray[i] = c;
}

void determineAndStoreMorse(long pulse_width) {
		//printf("%u", pulse_width);
		//if it's depressed or no.
		if (pulse_width > DOT_WIDTH_MIN && pulse_width < DOT_WIDTH_MAX) {
			addMorseSymbol('.');
			sevenSegmentDot();
		} else if (pulse_width > DOT_WIDTH_MAX) {
			addMorseSymbol('-'); //put dash
			sevenSegmentDash();
		}
}


/* startNewLetter()
* prints the current letter to the console 
* and clears the storage array
*/
void startNewLetter() {

	if(strcmp(morseSymbolArray, ".-") == 0) { 
			putchar('A');
	} else if (strcmp(morseSymbolArray, "-...") == 0) { 
			putchar('B');
	} else if (strcmp(morseSymbolArray, "-.-.") == 0) {		
			putchar('C');
	} else if (strcmp(morseSymbolArray, "-..") == 0) {
			putchar('D');

		} else if (strcmp(morseSymbolArray,  ".") == 0) {
			putchar('E');
		
		} else if (strcmp(morseSymbolArray,  "..-.") == 0) {
			putchar('F');
		
		} else if (strcmp(morseSymbolArray,  "--.") == 0) {
			putchar('G');
		
		} else if (strcmp(morseSymbolArray,  "....") == 0) {
			putchar('H');
		
		} else if (strcmp(morseSymbolArray,  "..") == 0) {
			putchar('I');
		
		} else if (strcmp(morseSymbolArray,  ".---") == 0) {
			putchar('J');
		
		} else if (strcmp(morseSymbolArray,  "-.-") == 0) {
			putchar('K');
		
		} else if (strcmp(morseSymbolArray,  ".-..") == 0) {
			putchar('L');
		
		} else if (strcmp(morseSymbolArray,  "--") == 0) {
			putchar('M');
		
		} else if (strcmp(morseSymbolArray,  "-.") == 0) {
			putchar('N');
		
		} else if (strcmp(morseSymbolArray,  "---") == 0) {
			putchar('O');
		
		} else if (strcmp(morseSymbolArray,  ".--.") == 0) {
			putchar('P');
		
		} else if (strcmp(morseSymbolArray,  "--.-") == 0) {
			putchar('Q');
		
		} else if (strcmp(morseSymbolArray,  ".-.") == 0) {
			putchar('R');
		
		} else if (strcmp(morseSymbolArray,  "...") == 0) {
			putchar('S');
		
		} else if (strcmp(morseSymbolArray,  "-") == 0) {
			putchar('T');
		
		} else if (strcmp(morseSymbolArray,  "..-") == 0) {
			putchar('U');
		
		} else if (strcmp(morseSymbolArray,  "...-") == 0) {
			putchar('V');
		
		} else if (strcmp(morseSymbolArray,  ".--") == 0) {
			putchar('W');
		
		} else if (strcmp(morseSymbolArray,  "-..-") == 0) {
			putchar('X');
		
		} else if (strcmp(morseSymbolArray,  "-.--") == 0) {
			putchar('Y');
		
		} else if (strcmp(morseSymbolArray,  "--..") == 0) {
			putchar('Z');
		
		} else if (strcmp(morseSymbolArray,  "-----") == 0) {
			putchar('0');
		
		} else if (strcmp(morseSymbolArray,  ".----") == 0) {
			putchar('1');
		
		} else if (strcmp(morseSymbolArray,  "..---") == 0) {
			putchar('2');
		
		} else if (strcmp(morseSymbolArray,  "...--") == 0) {
			putchar('3');
		
		} else if (strcmp(morseSymbolArray,  "....-") == 0) {
			putchar('4');
		
		} else if (strcmp(morseSymbolArray,  ".....") == 0) {
			putchar('5');
		
		} else if (strcmp(morseSymbolArray,  "-....") == 0) {
			putchar('6');
		
		} else if (strcmp(morseSymbolArray,  "--...") == 0) {
			putchar('7');
		
		} else if (strcmp(morseSymbolArray,  "---..") == 0) {
			putchar('8');
		
		} else if (strcmp(morseSymbolArray,  "----.") == 0) {
			putchar('9');
		} else {
			printf("Err");
		}
		memset(morseSymbolArray, '\0', 6);
	}

void startNewWord() {
	startNewLetter();
	putchar(' ');
}

/* 
* handleSpacing()
* looks at the edge1 value and number of overflows to 
* determine if a new alphanumeric character or word has been started
*/
void handleSpacing() {
	//floor of the number of dot time lengths that have passed
	unsigned int numberOfDots = ((long) overflows * 65536u + edge1 )/ ((long) DOT_WIDTH_MAX); 

	//see if a new word has been started
	if(numberOfDots >= 7) {
		startNewWord();
	} else if (numberOfDots >= 3) {
		startNewLetter();
	}
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
		handleSpacing();
		overflows = 0;
		
		
		while(!(TIFR1 & 0x20)); 	//wait until falling edge
		edge2 = ICR1; 				//save time of second edge

		if (edge2 < edge1) {
			overflows--;
		}

		diff = edge2 - edge1;
		pulse_width = (long) overflows * 65536u + (long) diff;

		determineAndStoreMorse(pulse_width);
		
	}
}

