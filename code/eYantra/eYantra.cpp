/*
 * eYantra.cpp
 *
 * Created: 12-Jan-20 5:41:06 PM
 *  Author: Shyam Murugan
 */ 
#define F_CPU 14745600

#include <avr/io.h>
#include "MotorControl/MotorControl.h"
#include "UART/UART.h"
#include "helper.h"
#include "Encoders/Encoders.h"
#include <util/delay.h>
#include<stdio.h>

void enableGlobalInterrupts() {
	SREG |= (1 << 7);
}

void disableGlobalInterrupts() {
	SREG &= !(1 << 7);
}

int main(void) {
	enableGlobalInterrupts();
	initEncoders();
	Serial::begin();
	
	volatile unsigned int count = 0;
	encoderCount0 = &count;
	encoderCount1 = &count;
	char temp[15];
	
    while(1) {
		Serial::println("count:");
		sprintf(temp, "%d", count);
		Serial::println(temp);
		_delay_ms(100);
    }
}