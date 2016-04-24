// sensor.c
//
// ESE 350 Final Project
// by Phillip Trent & Sahithya Prakash
// University of Pennsylvania
//
// This file manipulates the sensor on the back of the container by switching the
// multiplexers and runs the ADC
//
// author: ptrent
// email: ptrent@seas.upenn.edu
// blog: philliphtrentiii.info/embedded-systems-blog

#include "sensor.h"
#include <unistd.h>
#include <avr/io.h>

// sets the given registers given bit number to the given value
unsigned int setOnlyBitInRegisterToValue(unsigned int reg, unsigned int bit, unsigned int value) {
	//get a bit value with all 1s except for a zero in the designated bit's place.
	unsigned int clearedBit =  reg & 0xFF << (bit + 1) | 0xFF >> (8 - bit);
	//return the original register cleared and then set to the value given by the value
	return ((reg & clearedBit) | ((value & 0x01) << bit));
}

//gets the conductivity (in V) using the ADC
double getConductivity() {
	//start ADC read
	ADCSRA |= (1 << ADSC);
	while ((ADCSRA & (1 << ADIF)) != (1 << ADIF));
	double analogValue = ((double) 1 / (double) 0xFF) * ADC;
	return analogValue;
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

//performs any initialization code like setting registers and
void initializeSensor() {
	setupADC();

	//setup outputs for select lines on the designated registers
	PWR_SEL_REG |= _BV(PWR_SEL_0);
	PWR_SEL_REG |= _BV(PWR_SEL_1);
	PWR_SEL_REG |= _BV(PWR_SEL_2);

	MUX_SEL_LVL1_REG |= _BV(MUX_SEL_LVL1_BIT0);
	MUX_SEL_LVL1_REG |= _BV(MUX_SEL_LVL1_BIT1);
	MUX_SEL_LVL1_REG |= _BV(MUX_SEL_LVL1_BIT2);
	MUX_SEL_LVL1_REG |= _BV(MUX_SEL_LVL1_BIT3);

	MUX_SEL_LVL2_REG |= _BV(MUX_SEL_LVL2_BIT0);
	MUX_SEL_LVL2_REG |= _BV(MUX_SEL_LVL2_BIT0);
	MUX_SEL_LVL2_REG |= _BV(MUX_SEL_LVL2_BIT0);
	MUX_SEL_LVL2_REG |= _BV(MUX_SEL_LVL2_BIT0);

	//set input pin to inptu
	INPUT_PIN_REG &= ~_BV(INPUT_PIN);
}

//gets the highest row that is conducting
unsigned int highestConductorForColumn(unsigned int columnNumber) {
	unsigned int highestConductor = 0x00;
	switchPowerToColumnNumber(columnNumber);
	for (unsigned int row = 0x00; row < 169; row ++) {
		switchMuxesToHeight(row);

		//wait for a little bit
		//for (unsigned int a = 0x00; a < 0xFF; a ++);

		if ((INPUT_PIN_REG & _BV(INPUT_PIN)) > 0x00) {
			highestConductor = row + 1;
		}
	}
	return highestConductor;
}

//switches MUXs to lowest row in the designated column, and starts the ADC
double voltageFromLowestConductorInColumn(unsigned int columnNumber) {
	switchPowerToColumnNumber(columnNumber);
	switchMuxesToHeight(0x00);
	for (unsigned int a = 0x00; a < 0xFF; a ++);
	getConductivity();
}

