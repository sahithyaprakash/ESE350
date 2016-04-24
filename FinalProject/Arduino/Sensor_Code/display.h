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

// 
void setOutputPerHour(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace);

// 
void setTotalOutput(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace);

// call when you want the entire display to be updated
void updateDisplay();

