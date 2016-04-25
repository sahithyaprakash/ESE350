#include <unistd.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../Arduino_ATMega/uart.h"
#include "sensor.h"
#include "storage.h"
#include "display.h"

int main (void) {
	// initialize controller state
	//how long between collections
	uint8_t periodOfCollection = 5; 
	//how long between collections
	uint8_t frequencyOfCollection = 60;

	uart_inits();

	TCCR0A = 0x05; 					// start timer, prescaler = 1024


	// set registers to outputs
	DDRB = 0xFD;
	DDRC = 0xEF; //a/ll outputs except for pin 4
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

		unsigned int beforeDecimalPlace = (unsigned int) currentVolume;
		unsigned int afterDecimalPlace = (unsigned int) ((currentVolume - beforeDecimalPlace) * 100);
		
		//double totalLastHour = totalOutputFromTheLastHour();
		//unsigned int beforeDecimalPlaceLastHour = (unsigned int) totalLastHour;
		//unsigned int afterDecimalPlaceLastHour = (unsigned int) ((totalLastHour - beforeDecimalPlaceLastHour) * 100);

		setTotalOutput(beforeDecimalPlace, afterDecimalPlace);
		//setOutputPerHour(beforeDecimalPlaceLastHour, afterDecimalPlaceLastHour);

		updateDisplay();
		delay_s(1);
	}
}