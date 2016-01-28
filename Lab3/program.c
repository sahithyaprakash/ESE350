#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"


int main (void) {
	uart_init();
	DDRB = 0xFF; //make all pins outputs
	PORTB = 0x02; // set PIN 9 to high voltage.

}
