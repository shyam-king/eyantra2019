/*
 * Buzzer.cpp
 *
 * Created: 2/5/2020 2:36:56 PM
 *  Author: Shyam
 */ 

#include "Buzzer.h"
#include <avr/io.h>

void Buzzer::init() {
    DDRH |= (1 << 0);
    //PORTH &= ~(1 << 0);
    PORTH |= (1 << 0);
}

void Buzzer::stopBuzz() {
    PORTH |= (1 << 0);
}

void Buzzer::buzz() {
    PORTH &= ~(1 << 0);
}