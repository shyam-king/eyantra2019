/*
 * Motor.cpp
 *
 * Created: 2/4/2020 6:11:57 PM
 *  Author: Akil
 */ 

#include "Motor.h"
#include <avr/io.h>
#include "../configure.h"

#define abs(x) ((x)>0) ? (x) : -(x)

//Using two channels of timer0 to control motor speed

void Motor::init() {
    //non inverting
    //fast pwm wgm = 3
    //COM 2
    DDRG |= (1 << 5);
    DDRB |= (1 << 7);

    DDRB |= (1 << MOTOR_L_FRONT) | (1 << MOTOR_L_BACK) | (1 << MOTOR_R_FRONT) | (1 << MOTOR_R_BACK);
    PORTB &= ~((1 << MOTOR_L_FRONT) | (1 << MOTOR_R_FRONT) | (1 << MOTOR_L_BACK) | (1 << MOTOR_R_BACK));

    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B = (1 << CS01);
    MOTOR_L_SPEED = 255;
    MOTOR_R_SPEED = 255;
}

void Motor::setRightDirection(Dir dir) {
    switch(dir) {
        case FRONT:
            PORTB |= (1 << MOTOR_R_FRONT);
            PORTB &= ~(1 << MOTOR_R_BACK);
        break;

        case BACK:
            PORTB |= (1 << MOTOR_R_BACK);
            PORTB &= ~(1 << MOTOR_R_FRONT);
        break;
    }
}

void Motor::setLeftDirection(Dir dir) {
    switch(dir) {
        case FRONT:
            PORTB |= (1 << MOTOR_L_FRONT);
            PORTB &= ~(1 << MOTOR_L_BACK);
        break;

        case BACK:
            PORTB |= (1 << MOTOR_L_BACK);
            PORTB &= ~(1 << MOTOR_L_FRONT);
        break;
    }
}

void Motor::setRightSpeed(uint8_t spd) {
    MOTOR_R_SPEED = spd;
}

void Motor::setLeftSpeed(uint8_t spd) {
    MOTOR_L_SPEED = spd;
}

void Motor::stopLeftMotor() {
    PORTB &= ~((1 << MOTOR_L_FRONT) | (1 << MOTOR_L_BACK));
}

void Motor::stopRightMotor() {
    PORTB &= ~((1 << MOTOR_R_FRONT) | (1 << MOTOR_R_BACK));
}

void Motor::applyPIDCorrection(float correction) {
    int16_t c = 255 - abs(correction);
    
    if (c > 255) c = 255;
    if (c < 0) c = 0;

    if (correction > 0) {
        Motor::setLeftSpeed(c);
        Motor::setRightSpeed(255);
    }
    else {
        Motor::setLeftSpeed(255);
        Motor::setRightSpeed(c);
    }
}