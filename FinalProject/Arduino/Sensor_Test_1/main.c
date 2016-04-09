#include <unistd.h>
#include <avr/io.h>
#include "../Arduino_ATMega/uart.h"

int main (void) {
	unsigned int last_highest_conductor = 0x00;
	//initialize controller state
	uart_init();
	TCCR0A = 0x05; 					// start timer, prescaler = 1024

	DDRD = 0xFF; // set DDRD as output
	DDRB = 0xEF; // set everything but PB4 to outputs

	while(1) {
		unsigned int highest_conductor = 0x00;
		unsigned int current = 0x00;
		for (unsigned int current = 0x00; current <= 0x57; current ++) {
			PORTD = (((current << 2) & 0xFC) | (PORTD & 0x03)); 		//set the new select

			PORTB = (((current >> 6) & 0x01) | (PORTB & 0xFE));		//set the highest bit

			unsigned int a = 0x00;

			while(a < 0xFF) {
				a++;
			}
			
			if ((PINB & 0x10) == 0x10) {
				highest_conductor = current;
			}
		}
		//if (highest_conductor != last_highest_conductor) {
		//	last_highest_conductor = highest_conductor;
			printf("Highest Conductor: %i \n", highest_conductor);
		//}
	};
}
