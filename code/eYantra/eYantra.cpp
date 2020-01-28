/*
 * eYantra.cpp
 *
 * Created: 12-Jan-20 5:41:06 PM
 *  Author: Shyam Murugan
 */ 
#define F_CPU 14745600

#include <avr/io.h>

#include "UART/UART.h"
#include "helper.h"
#include <util/delay.h>
#include<stdio.h>

int val = 0;

void enableGlobalInterrupts() {
	SREG |= (1 << 7);
}

void disableGlobalInterrupts() {
	SREG &= !(1 << 7);
}


float line_Sensor_Read(int pin)
{
	switch (pin)

	{ 
		case 0:  
		ADMUX = 11000000; // Reading from ADC0
		ADCSRA |= (1<< ADSC); // for starting a conversion
		while(ADCSRA & (1<<ADIF) == 0);
		val = ADC;
		return val;
		break;

		case 1: ADMUX = 11000001; // Reading from ADC1
		ADCSRA |= (1<< ADSC); // for starting a conversion
		while(ADCSRA & (1<<ADIF) == 0);
		val = ADC;
		return val;
		break;
		
		case 2:  ADMUX = 11000010;  // ADC2
		ADCSRA |= (1<< ADSC); // for starting a conversion
		while(ADCSRA & (1<<ADIF) == 0);
		val = ADC;
		return val;
		break;
	}
}



int main(void) {
	enableGlobalInterrupts();
	Serial::begin();
	int result;
	char temp[15];
	
	ADCSRA |= (1<<ADEN) | (1<<ADIE);
	
	Serial::println("the code starts here!");
	
	for (int i =0; i<10;i++) {
		Serial::println("i");
		_delay_ms(1000);
	}
	
	Serial::println("bhf");
	
    while(1) {
		Serial::println("while");
		result = line_Sensor_Read(0);
		Serial::println("count:");
		sprintf(temp, "%d", result);
		Serial::println(temp);
		
		result = line_Sensor_Read(1);
		Serial::println("count:");
		sprintf(temp, "%d", result);
		Serial::println(temp);
		
		result = line_Sensor_Read(2);
		Serial::println("count:");
		sprintf(temp, "%d", result);
		Serial::println(temp);
		_delay_ms(1000);
    }
}