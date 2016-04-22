// sensor.h
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

#define PWR_SEL_REG PORTD 		// register for column selection bits
#define PWR_SEL_0 PORTD2 
#define PWR_SEL_1 PORTD3
#define PWR_SEL_2 PORTD4

#define MUX_SEL_LVL1_REG PORTC 	// register for lower level multiplexers
#define MUX_SEL_LVL1_BIT0 PORTC0
#define MUX_SEL_LVL1_BIT1 PORTC1
#define MUX_SEL_LVL1_BIT2 PORTC2
#define MUX_SEL_LVL1_BIT3 PORTC3

#define MUX_SEL_LVL2_REG PORTB 	// register for higher level multiplexers
#define MUX_SEL_LVL2_BIT0 PORTB2
#define MUX_SEL_LVL2_BIT1 PORTB3
#define MUX_SEL_LVL2_BIT2 PORTB4
#define MUX_SEL_LVL2_BIT3 PORTB5

#define INPUT_PIN_REG DDRC		// register for input pin from MUX array
#define INPUT_PIN PINC4			

//performs any initialization code like setting registers and 
void initializeSensor();

//gets the highest row that is conducting
unsigned int highestConductorForColumn(unsigned int columnNumber);

//switches MUXs to lowest row in the designated column, and starts the ADC
unsigned int voltageFromLowestConductorInColumn(unsigned int columnNumber);