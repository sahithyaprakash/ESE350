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
#include <String.h>
#include "../ST7565-LCD/c/glcd.h"
#include "../ST7565-LCD/c/stlcd.h"

#define CHAR_PADDING 6

uint8_t buffer[128 * 64 / 8];

// does all of the initialization work for the display
void initializeDisplay() {
	setup();
	*buffer = get_buffer();
	clear_buffer(buffer);
	write_buffer(buffer);
}

void drawString(char* string, uint8_t length, uint8_t line, uint8_t shift) {
	printf("drawing %s\n", string);
	for (int index = 0; index < length; index ++) {
		drawchar(buffer, shift + index * CHAR_PADDING, line, string[index]);
	}
}

//
void setOutputPerHour(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace) {
	int line = 6;
	int shift = 0;
	char label[] = "LAST HOUR";

	char sigDigs[8];
	sigDigs[0] = (((beforeDecimalPlace/100) % 10) + '0');
	sigDigs[1] = (((beforeDecimalPlace/10) % 10) + '0');
	sigDigs[2] = ((beforeDecimalPlace % 10) + '0');
	sigDigs[3] = '.';
	sigDigs[4] = (((afterDecimalPlace/10) % 10) + '0');
	sigDigs[5] = ((afterDecimalPlace % 10) + '0');
	sigDigs[6] = 'm';
	sigDigs[7] = 'L';

	if (beforeDecimalPlace < 100) {
		sigDigs[0] = ' ';
	}

	if (beforeDecimalPlace < 10) {
		sigDigs[1] = ' ';
	}

	drawString(sigDigs, strlen(sigDigs), line, shift);
	drawString(label, strlen(label), line + 1, shift);
}

//
void setTotalOutput(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace) {
	printf("set total output to %i.%i \n", beforeDecimalPlace, afterDecimalPlace);

	int shift = 0;
	int line = 2;
	char label[] = "  TOTAL";
	char sigDigs[9];
	sigDigs[0] = (((beforeDecimalPlace / 1000) % 10) + '0');
	sigDigs[1] = (((beforeDecimalPlace / 100) % 10) + '0');
	sigDigs[2] = (((beforeDecimalPlace / 10) % 10) + '0');
	sigDigs[3] = ((beforeDecimalPlace % 10) + '0');
	sigDigs[4] = '.';
	sigDigs[5] = (((afterDecimalPlace / 10) % 10) + '0');
	sigDigs[6] = ((afterDecimalPlace % 10) + '0');
	sigDigs[7] = 'm';
	sigDigs[8] = 'L';

	if (beforeDecimalPlace < 1000) {
		sigDigs[0] = ' ';
	}

	if (beforeDecimalPlace < 100) {
		sigDigs[1] = ' ';
	}
	if (beforeDecimalPlace < 10) {
		sigDigs[2] = ' ';
	}

	drawString(sigDigs, sizeof(sigDigs), line, shift);
	drawString(label, strlen(label), line + 1, shift);
	drawrect(buffer, 0, 0, 10, 10, 0x00);

	return;
}

// call when you want the entire display to be updated
void updateDisplay() {
	printf("update display \n");
	write_buffer(buffer);
}