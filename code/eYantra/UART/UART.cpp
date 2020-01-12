/*
 * UART.cpp
 *
 * Created: 13-Jan-20 12:59:59 AM
 *  Author: Shyam Murugan
 */ 

#include "UART.h"
#include <avr/io.h>
#include <string.h>

void Serial::begin() {
	//set baud rate to 9600
	UBRR0 = 95;
	
	UCSR0B |= (1 << TXEN0);
	
	//8-bit data
	UCSR0C |= (3 << UCSZ00); 
}

void Serial::print(const char* data) {
	for (const char *s = data; *s; ++s) {
		while (!(UCSR0A & (1 << UDRE0)));
		UDR0 = *s;
	}
}

void Serial::println(const char* data) {
	print(data);
	print("\n");
}