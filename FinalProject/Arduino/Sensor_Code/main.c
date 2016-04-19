#include <unistd.h>
#include <avr/io.h>
#include "../Arduino_ATMega/uart.h"

#define PWR_SEL_REG PORTF
#define PWR_SEL_0 PORTF5
#define PWR_SEL_1 PORTF6
#define PWR_SEL_2 PORTF7

#define MUX_SEL_LVL1_REG PORTD
#define MUX_SEL_LVL1_BIT0 PORTD0
#define MUX_SEL_LVL1_BIT1 PORTD1
#define MUX_SEL_LVL1_BIT2 PORTD4
#define MUX_SEL_LVL1_BIT3 PORTD6

#define MUX_SEL_LVL2_REG PORTB
#define MUX_SEL_LVL2_BIT0 PORTB1
#define MUX_SEL_LVL2_BIT1 PORTB3
#define MUX_SEL_LVL2_BIT2 PORTB6
#define MUX_SEL_LVL2_BIT3 PORTB7

#define INPUT_PIN PINF0


// Calculates the amount of liquid in a single column (mL)
// from the height of the liquid in number of coducting datapoints
float liquidAmount(int heighestConductor) {
	if (heighestConductor > 0) {
		float actuationDepth = 2.5;
		float unitVolume = 3.175;
		return actuationDepth + (heighestConductor - 1) * unitVolume;
	} else {
		return 0;
	}
}

double getConductivity() {
	//start ADC read
	ADCSRA |= (1 << ADSC);
	while((ADCSRA & (1 << ADIF)) != (1 << ADIF));
	double analogValue = ((double) 1 / (double) 0xFF) * ADC;
	return analogValue;
}

// sets the given registers given bit number to the given value
unsigned int setOnlyBitInRegisterToValue(unsigned int reg, unsigned int bit, unsigned int value) {
	//get a bit value with all 1s except for a zero in the designated bit's place.
	unsigned int clearedBit =  reg & 0xFF << (bit + 1) | 0xFF >> (8 - bit);
	//return the original register cleared and then set to the value given by the value
	return ((reg & clearedBit) | ((value & 0x01) << bit));
}

// Switches the high potential to the column designated by the variable columnNumber (zero index, right to left)
void switchPowerToColumnNumber(int columnNumber) {
	//save the current register value
	unsigned int PWR_SEL_REG_NEW = PWR_SEL_REG;

	//update the value to the new register value
	PWR_SEL_REG_NEW = setOnlyBitInRegisterToValue(PWR_SEL_REG_NEW, PWR_SEL_0, (columnNumber & 0x01));
	PWR_SEL_REG_NEW = setOnlyBitInRegisterToValue(PWR_SEL_REG_NEW, PWR_SEL_1, ((columnNumber & 0x02) >> 1));
	PWR_SEL_REG_NEW = setOnlyBitInRegisterToValue(PWR_SEL_REG_NEW, PWR_SEL_2, ((columnNumber & 0x04) >> 2));

	//set the register value (only happens once)
	PWR_SEL_REG = PWR_SEL_REG_NEW;
}

// Switches to reading the value at the given height
void switchMuxesToHeight(int height) {
	//save the current 1st level register value
	unsigned int MUX_SEL_LVL1_REG_NEW = MUX_SEL_LVL1_REG;

	//set the new values to the given values (height)
	MUX_SEL_LVL1_REG_NEW = setOnlyBitInRegisterToValue(MUX_SEL_LVL1_REG_NEW, MUX_SEL_LVL1_BIT0, (height & 0x01));
	MUX_SEL_LVL1_REG_NEW = setOnlyBitInRegisterToValue(MUX_SEL_LVL1_REG_NEW, MUX_SEL_LVL1_BIT1, ((height & 0x02) >> 1));
	MUX_SEL_LVL1_REG_NEW = setOnlyBitInRegisterToValue(MUX_SEL_LVL1_REG_NEW, MUX_SEL_LVL1_BIT2, ((height & 0x04) >> 2));
	MUX_SEL_LVL1_REG_NEW = setOnlyBitInRegisterToValue(MUX_SEL_LVL1_REG_NEW, MUX_SEL_LVL1_BIT3, ((height & 0x08) >> 3));

	//set the register (only happens once)
	MUX_SEL_LVL1_REG = MUX_SEL_LVL1_REG_NEW;

	//save the 2n level register value
	unsigned int MUX_SEL_LVL2_REG_NEW = MUX_SEL_LVL2_REG;

	//set the new values to the given ones (height)
	MUX_SEL_LVL2_REG_NEW = setOnlyBitInRegisterToValue(MUX_SEL_LVL2_REG_NEW, MUX_SEL_LVL2_BIT0, ((height & 0x10) >> 4));
	MUX_SEL_LVL2_REG_NEW = setOnlyBitInRegisterToValue(MUX_SEL_LVL2_REG_NEW, MUX_SEL_LVL2_BIT1, ((height & 0x20) >> 5));
	MUX_SEL_LVL2_REG_NEW = setOnlyBitInRegisterToValue(MUX_SEL_LVL2_REG_NEW, MUX_SEL_LVL2_BIT2, ((height & 0x40) >> 6));
	MUX_SEL_LVL2_REG_NEW = setOnlyBitInRegisterToValue(MUX_SEL_LVL2_REG_NEW, MUX_SEL_LVL2_BIT3, ((height & 0x80) >> 7));

	//set the register (only happens once)
	MUX_SEL_LVL2_REG = MUX_SEL_LVL2_REG_NEW;
}

//set up the ADC for reading the exact voltage
void setupADC() {
	ADMUX = 0x40; 		// choosing reference voltage
	ADMUX |= 0x05; 		// set which channel we want to read from (PIN A5)
	ADCSRA = 0xE0; 		// enables the adc and starts the conversions and enables auto trigger
	ADCSRA |= 0x07; 	// sets the clock prescalser to 128
	ADCSRB = 0x00; 		// free running mode (always taking sample) 
}

int main (void) {
	// initialize controller state
	uart_init();
	setupADC();
	TCCR0A = 0x05; 					// start timer, prescaler = 1024

	// set registers to outputs
	DDRB = 0xFF;
	DDRD = 0xFF;
	DDRF = 0xFF;



	while (1) {
		unsigned int highest_conductor = 0x00;
		unsigned int current;
		double conductingVoltage = 0x00;
		float liquidAmt = 0x00;
		for (unsigned int column = 0x00; column < 7; column ++) {
			switchPowerToColumnNumber(column);
			for (current = 0x00; current <= 0xA9; current ++) {
				if (current == 0x00) {
					conductingVoltage = getConductivity();
				}
				switchMuxesToHeight(current);

				unsigned int a = 0x00;
				while (a < 0xFF) {
					a++;
				}

				if (INPUT_PIN == 0x01) {
					highest_conductor = current + 1;
				}
			}
			liquidAmt = liquidAmount(highest_conductor);
			printf("Highest Conductor: (%i, %i) ", highest_conductor, column);
			printf(" | %d mL ", liquidAmt);
			printf(" | %d V \n", conductingVoltage);

			highest_conductor = 0x00;
		}
		
	}
}