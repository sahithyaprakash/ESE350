#include <unistd.h>
#include <avr/io.h>
#include <util/delay.h>
#include <time.h>
#include "../Arduino_ATMega/uart.h"
#include "sensor.h"
#include "storage.h"
#include "display.h"


unsigned int getDigitsBeforeDecimalPlace(float value) {
	return (unsigned int) value;
}

unsigned int getDigitsAfterDecimalPlace(float value) {
	return (unsigned int) ((value - getDigitsBeforeDecimalPlace(value)) * 100);
}

int main (void) {
	// initialize controller state
	//how long between collections
	uint8_t periodOfCollection = 5;
	//how many collections per archive
	uint8_t frequencyOfCollection = 60;

	uart_inits();

	TCCR0A = 0x05; 					// start timer, prescaler = 1024


	// set registers to outputs
	DDRB = 0xFD;
	DDRC = 0xEF; //all outputs except for pin 4
	DDRD = 0xFF;

	initializeSensor();
	initializeDisplay();
	initializeStorage(frequencyOfCollection);

	while (1) {
		unsigned int highest_conductor = 0x00;
		float currentVolume = 0;
		float liquidAmt = 0x00;

		for (uint8_t column = 0; column < 7; column ++) {
			highest_conductor = highestConductorForColumn(column);
			//printf("Highest Conductor: (%i, %i) \n", highest_conductor, column);
			currentVolume += volumeFromHighestConductor(highest_conductor);
			highest_conductor = 0x00;
		}
		addData(currentVolume);
		delay_s(5);

		updateDisplay();
	}
}