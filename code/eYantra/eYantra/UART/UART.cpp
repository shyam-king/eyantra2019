/*
 * UART.cpp
 *
 * Created: 2/1/2020 7:44:43 AM
 *  Author: Shyam
 */ 
#include "UART.h"
#include <avr/io.h>
#include <stdio.h>

void Serial::init() {
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

void Serial::print(int value) {
    char temp[15];
    sprintf(temp, "%d", value);
    print(temp);
}