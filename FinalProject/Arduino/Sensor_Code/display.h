// display.h
//
// ESE 350 Final Project
// by Phillip Trent & Sahithya Prakash
// University of Pennsylvania
//
// This is the display code implementation for the ST7565 LCD
// it uses the library from Adafruit to do things like line and pixel drawing.
// Please see the blog for more information about the project.
//
// author: ptrent
// email: ptrent@seas.upenn.edu
// blog: philliphtrentiii.info/embedded-systems-blog


// does all of the initialization work for the display
void initializeDisplay();

// sets the output since the last hour. For example, if it's 7:15, then this is the amount
// of liquid added to the container since 7:00.
void setOutputSinceTheLastHour(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace);

// sets the output from the last hour. For example, if it's 7:15, then this is the total amount of
// liquid that was added to the container from 6:00-7:00.
void setOutputFromTheLastHour(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace);

// sets the total amount of liquid currently in the container
void setContainerLiquidLevel(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace);

// sets the specificity of the data to be displayed 0 is low, 1 is med, 2 is high
void setDisplaySpecificity(int s);

// call when you want the entire display to be updated
void updateDisplay();

