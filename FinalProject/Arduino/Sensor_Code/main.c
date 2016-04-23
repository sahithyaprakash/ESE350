#include <unistd.h>
#include <avr/io.h>
#include "../Arduino_ATMega/uart.h"
#include "sensor.h"
#include "storage.h"
#include "display.h"

int main (void) {
	// initialize controller state
	uint8_t x = 10;
	uint8_t y = 10;
	uint8_t color = 0;

	uart_inits();
	//setupADC();
	initializeSensor();
	initializeDisplay();

	TCCR0A = 0x05; 					// start timer, prescaler = 1024

	// set registers to outputs
	DDRB = 0xFD;
	DDRC = 0xEF; //a/ll outputs except for pin 4
	DDRD = 0xFF;

	while (1) {
		unsigned int highest_conductor = 0x00;
		unsigned int current;
		float conductingVoltage = 0x00;
		float liquidAmt = 0x00;

		for (unsigned int column = 0x00; column < 7; column ++) {
			highest_conductor = highestConductorForColumn(column);
			// liquidAmt = liquidAmount(highest_conductor);
			printf("Highest Conductor: (%i, %i) \n", highest_conductor, column);
			//printf(" | %u mL ", liquidAmt);
			//printf(" | %u V \n", conductingVoltage);

			highest_conductor = 0x00;
		}

	}
}