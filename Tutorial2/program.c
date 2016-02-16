#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"

#define CHANNEL 1

int main (void) {
uint8_t channel = 1;
uart_init();

ADCSRA = 0x07; 			// Sets the ADC reference clock to 125kHZ
ADMUX = 0x40;			// Sets to 5V Reference
//ADMUX |= (1 << REFS0);
ADCSRA |= 0x80; 		// Powers on the ADC
ADCSRA |= 0x40;			// First Conversion

ADMUX &= 0xF0;			//Clear the older channel that was read
ADMUX |= channel;		//Defines the new ADC channel to be read
ADCSRA |= (1 << ADSC);	//Starts a new conversion

while (1) {
	while (ADCSRA & (1 << ADSC));	//wait until the conversion is done
	printf("%u \n", ADC);
	ADCSRA |= (1 << ADSC);	//Starts a new conversion
}
}

