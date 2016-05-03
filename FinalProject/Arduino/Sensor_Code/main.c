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
	SpecificityOfData specificity = LOW;
	uart_inits();

	TCCR0A = 0x05; 					// start timer, prescaler = 1024

	// set registers to outputs
	DDRB = 0xFD;
	DDRC = 0xEF; //all outputs except for pin 4
	DDRD = 0xFF;

	initializeSensor();
	initializeDisplay();
	initializeStorage(specificity);

	while (1) {
		for (uint8_t loopNumber = numberOfSamplesForSpecificityLevel(specificity); loopNumber > 0; loopNumber --) {
			unsigned int highest_conductor = 0;
			float currentVolume = 0.0;
			//float liquidAmt = 0x00;

			 for (unsigned int column = 0; column < 7; column ++) {
			 	highest_conductor = highestConductorForColumn(column);
			 	currentVolume += volumeFromHighestConductor(highest_conductor);
			 	highest_conductor = 0;
			 }

			//update the display and storage
			addData(currentVolume);
			setContainerLiquidLevel(getDigitsBeforeDecimalPlace(currentVolume), getDigitsAfterDecimalPlace(currentVolume));
			setOutputSinceTheLastHour(getDigitsBeforeDecimalPlace(totalOutputSinceTheLastHour()), getDigitsAfterDecimalPlace(totalOutputSinceTheLastHour()));
			updateDisplay();
			//wait
			uint8_t delayTime = 3 - specificity;
			delay_s(delayTime);
		}
		int totalOutputSinceLastHour = (int) totalOutputSinceTheLastHour();
		if (totalOutputSinceLastHour < 10) {
			specificity = HIGH;
		} else if (totalOutputSinceLastHour < 20) {
			specificity = MEDIUM;
		} else {
			specificity = LOW;
		}
		setDisplaySpecificity(specificity);
		archiveData(specificity);
		setOutputFromTheLastHour(getDigitsBeforeDecimalPlace(totalOutputFromTheLastHour()), getDigitsAfterDecimalPlace(totalOutputFromTheLastHour()));
		updateDisplay();
	}
}