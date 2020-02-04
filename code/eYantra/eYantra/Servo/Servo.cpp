/*
 * Servo.cpp
 *
 * Created: 2/1/2020 11:23:02 AM
 *  Author: Jayamathangi
 */ 

#include "Servo.h"
#include <avr/io.h>

/*
    using Timer4
*/

#include "../configure.h"

bool Servo::initialized = false;

void Servo::init() {

    DDRH |= (1<<3) | (1 << 4) | (1 << 5);
    TCCR4A = (1<<WGM41) | (1<<COM4A1) | (1 << COM4B1) | (1 << COM4C1); // Fast PWM Mode, non-inverting
    TCCR4B = (1<<WGM43) | (1<<WGM42) | (1 << CS41); // 8 prescaling
    ICR4 = SERVO_TOTAL; //setting period as 20ms

    initialized = true;
}

uint16_t Servo::map(float angle) {
    return SERVO_MIN + (angle/180) * (SERVO_MAX - SERVO_MIN);
}

void Servo::setAngle(Channel channel, float angle) {
    if (!initialized)
        init();
    
    switch (channel)
    {
    case LEFT:
        OCR4A = map(angle);
        break;
        
    case RIGHT:
        OCR4B = map(angle);
        break;

    case CENTER:
        OCR4C = map(angle);
        break;
    
    default:
        break;
    }
}