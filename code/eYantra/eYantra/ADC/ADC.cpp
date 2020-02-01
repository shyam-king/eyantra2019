/*
 * ADC.cpp
 *
 * Created: 2/1/2020 7:50:17 AM
 *  Author: Jayamathangi
 */ 

#include "ADC.h"
#include <avr/io.h>
#include <avr/delay.h>

uint16_t ADCInterface::read(uint8_t channel) {
    ADMUX = (ADMUX & ~0b111) | channel; // choose the channel
    ADCSRA |= (1<< ADSC); // for starting a conversion

    while(ADCSRA & (1<<ADSC) == 0); //wait for conversion
    uint16_t val = ADC; //read converted value

    ADCSRA |= (1 << ADIF); //clear the flag for next conversion
    _delay_us(100);
    return val;
}

void ADCInterface::init() {
    //enable ADC
    ADCSRA = (1 << ADEN) | (1<<ADPS1) | (1<<ADPS2);
    ADCSRB = 0;
    //choose AVCC and right adjusted result
    ADMUX = 0;
    ACSR = 0X80;
}