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
	setContainerLiquidLevel(0, 0);
	setOutputSinceTheLastHour(0, 0);
	setOutputFromTheLastHour(0, 0);
	setDisplaySpecificity(0);
	write_buffer(buffer);
}

void drawString(char* string, uint8_t length, uint8_t line, uint8_t shift) {
	for (int index = shift; index - shift < length; index ++) {
		drawchar(buffer, index * CHAR_PADDING, line, string[index - shift]);
	}
}
void setPercentageOfContainerFull(unsigned int percent) {
	int line = 3;
	int shift = 15;
	char drawing[] = "   ";
	drawing[0] = (((percent / 10) % 10) + '0');
	drawing[1] = ((percent % 10) + '0');
	drawing[2] = '%';

	char label[] = "FULL";

	drawString(drawing, sizeof(drawing), line, shift);
	drawString(label, sizeof(label), line + 1, shift);

}
// sets the output since the last hour. For example, if it's 7:15, then this is the amount
// of liquid added to the container since 7:00.
void setOutputSinceTheLastHour(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace) {
	int line = 6;
	int shift = 0;
	char label[] = "SINCE LAST HOUR";

	char sigDigs[8];
	sigDigs[0] = (((beforeDecimalPlace / 100) % 10) + '0');
	sigDigs[1] = (((beforeDecimalPlace / 10) % 10) + '0');
	sigDigs[2] = ((beforeDecimalPlace % 10) + '0');
	sigDigs[3] = '.';
	sigDigs[4] = (((afterDecimalPlace / 10) % 10) + '0');
	sigDigs[5] = ((afterDecimalPlace % 10) + '0');
	sigDigs[6] = 'm';
	sigDigs[7] = 'L';

	if (beforeDecimalPlace < 100) {
		sigDigs[0] = ' ';
	}

	if (beforeDecimalPlace < 10) {
		sigDigs[1] = ' ';
	}

	drawString(sigDigs, sizeof(sigDigs), line, shift);
	drawString(label, strlen(label), line + 1, shift);
}

// sets the output from the last hour. For example, if it's 7:15, then this is the total amount of
// liquid that was added to the container from 6:00-7:00.
void setOutputFromTheLastHour(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace) {
	int line = 3;
	int shift = 0;
	char label[] = "LAST HOUR";

	char sigDigs[8];
	sigDigs[0] = (((beforeDecimalPlace / 100) % 10) + '0');
	sigDigs[1] = (((beforeDecimalPlace / 10) % 10) + '0');
	sigDigs[2] = ((beforeDecimalPlace % 10) + '0');
	sigDigs[3] = '.';
	sigDigs[4] = (((afterDecimalPlace / 10) % 10) + '0');
	sigDigs[5] = ((afterDecimalPlace % 10) + '0');
	sigDigs[6] = 'm';
	sigDigs[7] = 'L';

	if (beforeDecimalPlace < 100) {
		sigDigs[0] = ' ';
	}

	if (beforeDecimalPlace < 10) {
		sigDigs[1] = ' ';
	}

	drawString(sigDigs, sizeof(sigDigs), line, shift);
	drawString(label, strlen(label), line + 1, shift);
}

// sets the total amount of liquid currently in the container
void setContainerLiquidLevel(unsigned int beforeDecimalPlace, unsigned int afterDecimalPlace) {
	setPercentageOfContainerFull(beforeDecimalPlace / 25);

	int shift = 0;
	int line = 0;
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
}

void setDisplaySpecificity(int s) {
	int line = 0;
	int shift = 18;
	switch (s) {
	case 0:
		drawString("LOW", sizeof("LOW"), line, shift);
		break;
	case 1:
		drawString("MED", sizeof("MED"), line, shift);
		break;
	case 2:
		drawString("HI ", sizeof("HI "), line, shift);
		break;
	}
}
// call when you want the entire display to be updated
void updateDisplay() {
	//printf("update display \n");
	write_buffer(buffer);
}