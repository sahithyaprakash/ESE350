#include <stdio.h>
#include <string.h>
#include "../Arduino_ATMega/uart.h"



int main (void) {
	uart_init();

	unsigned int a;
	unsigned int b;

	printf("Please input an integer value: ");
	scanf("%u %u", &a, &b);
	printf("You entered: %u %u\n", a, b);

	return 0;
	
}