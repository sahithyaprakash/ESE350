#include <stdio.h>
//#include "../Arduino_ATMega/uart.h"
#include <avr/io.h>

int counter = 0;
int recurse() {
	counter++;
	printf("counter: %d\tSP: %d\n", counter, SP);
	return recurse();
}

void main(void) {
	uart_init();
	recurse();
	for(;;);
}