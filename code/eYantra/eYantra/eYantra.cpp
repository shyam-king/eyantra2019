/*
 * eYantra.cpp
 *
 * Created: 2/1/2020 7:42:31 AM
 *  Author: Shyam
 */ 

#define F_CPU 14745600

#include <avr/io.h>
#include <util/delay.h>

#include "UART/UART.h"
#include "ADC/ADC.h"

void enableGlobalInterrupts() {
    SREG |= (1 << 7);
}

int main(void) {
    //initialize components
    enableGlobalInterrupts();
    Serial::init();
    ADCInterface::init();

    while(1) {
        Serial::print(ADCInterface::read(0));
        Serial::print("\t");
        Serial::print(ADCInterface::read(1));
        Serial::print("\t");
        Serial::print(ADCInterface::read(2));
        Serial::print("\n");

        _delay_ms(100);
    }
}