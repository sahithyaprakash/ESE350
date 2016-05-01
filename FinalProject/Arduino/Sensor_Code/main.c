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
	uint8_t periodOfCollection = 1; //seconds

	uart_inits();

	TCCR0A = 0x05; 					// start timer, prescaler = 1024

	// set registers to outputs
	DDRB = 0xFD;
	DDRC = 0xEF; //all outputs except for pin 4
	DDRD = 0xFF;

	initializeSensor();
	initializeDisplay();
	initializeStorage();

	while (1) {
		for (uint8_t loopNumber = 0; loopNumber < NUMBER_OF_DATA_POINTS_PER_ARCHIVE; loopNumber ++) {
			unsigned int highest_conductor = 0x00;
			float currentVolume = 0;
			float liquidAmt = 0x00;

			for (uint8_t column = 0; column < 7; column ++) {
				highest_conductor = highestConductorForColumn(column);
				currentVolume += volumeFromHighestConductor(highest_conductor);
				highest_conductor = 0x00;
			}
			//update the display and storage
			addData(currentVolume);
			setOutputSinceTheLastHour(getDigitsBeforeDecimalPlace(totalOutputSinceTheLastHour()), getDigitsAfterDecimalPlace(totalOutputSinceTheLastHour()));
			updateDisplay();
			//wait
			delay_s(periodOfCollection);
		}

		archiveData();
		setOutputSinceTheLastHour(0, 0);
		setOutputFromTheLastHour(getDigitsBeforeDecimalPlace(totalOutputFromTheLastHour()), getDigitsAfterDecimalPlace(totalOutputFromTheLastHour()));
		updateDisplay();
	}
}