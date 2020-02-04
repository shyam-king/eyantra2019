/*
 * ADC.cpp
 *
 * Created: 2/1/2020 7:50:17 AM
 *  Author: Jayamathangi
 */ 

#include "ADC.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

bool ADCInterface::initialized = false;

uint16_t ADCInterface::read(uint8_t channel) {
    if (!initialized)
        init();

    ADCSRA &= ~(1 << ADEN);
    ADMUX = channel; // choose the channel
    ADCSRA |= (1 << ADEN);

    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1<<ADSC) == 0); //wait for conversion
    uint16_t val = ADC;
    ADCSRA |= (1 << ADIF);

    _delay_ms(100);

    ADCSRA |= (1 << ADSC);
    while(ADCSRA & (1<<ADSC) == 0); //wait for conversion
    val = ADC;
    ADCSRA |= (1 << ADIF);

    return val;
}

void ADCInterface::init() {
    //enable ADC
    ADCSRA = (1 << ADEN) | (1<<ADPS1) | (1<<ADPS2);
    ADCSRB = 0;
    //choose AVCC and right adjusted result
    ADMUX = 0;
    ACSR = 0X80;

    initialized = true;
}