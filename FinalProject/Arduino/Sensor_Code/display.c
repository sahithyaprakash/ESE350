// display.c 
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

#include "display.h"
#include <unistd.h>
#include <avr/io.h>
#include "../ST7565-LCD/c/glcd.h"
#include "../ST7565-LCD/c/stlcd.h"

#define CHAR_PADDING 6

uint8_t buffer[128 * 64 / 8];

// does all of the initialization work for the display
void initializeDisplay() {
	setup();
	*buffer = get_buffer();
	updateDisplay();
}

// 
void setOutputPerHour(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace) {
	int shift = 100;
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
}

// 
void totalOutput(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace) {

}

// call when you want the entire display to be updated
void updateDisplay() {
	write_buffer(buffer);
}