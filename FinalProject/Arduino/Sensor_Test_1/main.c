#include <unistd.h>
#include <avr/io.h>
#include "../Arduino_ATMega/uart.h"

int main (void) {

	//initialize controller state
	uart_init();
	DDRB = 0xFF; 					// make all pins outputs
	TCCR0B = 0x05; 					// start timer, prescaler = 256


	DDRD = 0xFF; // set DDRD as output
	DDRB = 0xEF; // set everything but PB4 to outputs

	while(1) {
		unsigned int highest_conductor = 0x00;
		for (unsigned int current = 0x00; current <= 0x7F; current ++) {
			PORTD &= 0x03; 				//clear the current select on D
			PORTD |= ((current << 2) & 0x03); 		//set the new select
			PORTB &= 0xFE;				//clear the current value
			PORTB |= ((current >> 6) & 0x01);		//set the highest bit
			unsigned int a = 0x00;

			while(a < 0xFF) {
				a++;
			}
			
			if ((PINB & 0x10) == 0x10) {
				printf("current changed to: %i", current);
				highest_conductor = current;
			}
		}
		printf("Highest Conductor: %i \n", highest_conductor);
	};
}
