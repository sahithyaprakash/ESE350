#include <unistd.h>
#include <avr/io.h>
#include "../Arduino_ATMega/uart.h"
#include "../ST7565-LCD/c/glcd.h"
#include "../ST7565-LCD/c/stlcd.h"


#define PWR_SEL_REG PORTD
#define PWR_SEL_0 PORTD2
#define PWR_SEL_1 PORTD3
#define PWR_SEL_2 PORTD4

#define MUX_SEL_LVL1_REG PORTC
#define MUX_SEL_LVL1_BIT0 PORTC0
#define MUX_SEL_LVL1_BIT1 PORTC1
#define MUX_SEL_LVL1_BIT2 PORTC2
#define MUX_SEL_LVL1_BIT3 PORTC3

#define MUX_SEL_LVL2_REG PORTB
#define MUX_SEL_LVL2_BIT0 PORTB2
#define MUX_SEL_LVL2_BIT1 PORTB3
#define MUX_SEL_LVL2_BIT2 PORTB4
#define MUX_SEL_LVL2_BIT3 PORTB5

#define INPUT_PIN PINC4
#define INPUT_PIN_REG DDRC



// Calculates the amount of liquid in a single column (mL)
// from the height of the liquid in number of coducting datapoints
float liquidAmount(int heighestConductor) {
	if (heighestConductor > 0) {
		float actuationDepth = 2.5;
		float unitVolume = 3.175;
		// printf("actuation: %f \n", actuationDepth);
		// printf("unitVolume: %f \n", unitVolume);
		// printf("first: %d \n", (heighestConductor - 1) * unitVolume);
		// printf("second: %u \n", actuationDepth + (heighestConductor - 1) * unitVolume);

		return actuationDepth + (heighestConductor - 1) * unitVolume;
	} else {
		return 0;
	}
}

double getConductivity() {
	//start ADC read
	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADIF)) != (1 << ADIF));
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
	uint8_t x = 10;
	uint8_t y = 10;
	uint8_t color = 0;

	uart_inits();
	setupADC();

	TCCR0A = 0x05; 					// start timer, prescaler = 1024

	// set registers to outputs
	DDRB = 0xFD;
	DDRC = 0xEF; //all outputs except for pin 4
	DDRD = 0xFF;

	setup();
	uint8_t* buffer = get_buffer();

	int shift = 100;
	int padding = 6;
	clear_buffer(buffer);
	drawchar(buffer, shift + 0 * padding, 0, 'M');
	drawchar(buffer, shift + 1 * padding, 0, 'e');
	drawchar(buffer, shift + 2 * padding, 0, 'n');
	drawchar(buffer, shift + 3 * padding, 0, 'u');

	drawchar(buffer, shift + 0 * padding, 1, 'D');
	drawchar(buffer, shift + 1 * padding, 1, 'i');
	drawchar(buffer, shift + 2 * padding, 1, 's');
	drawchar(buffer, shift + 3 * padding, 1, 'p');

	drawchar(buffer, 0 * padding, 0, 'H');
	drawchar(buffer, 1 * padding, 0, 'I');
	drawchar(buffer, 2 * padding, 0, 'S');
	drawchar(buffer, 3 * padding, 0, 'T');
	drawchar(buffer, 4 * padding, 0, 'O');
	drawchar(buffer, 5 * padding, 0, 'G');
	drawchar(buffer, 6 * padding, 0, 'R');
	drawchar(buffer, 7 * padding, 0, 'A');
	drawchar(buffer, 8 * padding, 0, 'M');


	write_buffer(buffer);


	while (1) {
		unsigned int highest_conductor = 0x00;
		unsigned int current;
		float conductingVoltage = 0x00;
		float liquidAmt = 0x00;

		for (unsigned int column = 0x00; column < 7; column ++) {
			switchPowerToColumnNumber(column);
			for (current = 0x00; current < 0xA9; current ++) {
				if (current == 0x00) {
					conductingVoltage = getConductivity();
				}
				switchMuxesToHeight(current);

				//wait for a little bit
				for (unsigned int a = 0x00; a < 0xFF; a ++);

				if (((INPUT_PIN_REG >> (INPUT_PIN - 1)) & 0x01) == 0x01) {
					highest_conductor = current + 1;
				}
			}
			liquidAmt = liquidAmount(highest_conductor);
			// printf("Highest Conductor: (%i, %i) ", highest_conductor, column);
			// printf(" | %u mL ", liquidAmt);
			// printf(" | %u V \n", conductingVoltage);

			highest_conductor = 0x00;
		}

	}
}