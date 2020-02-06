/*
 * Motor.cpp
 *
 * Created: 2/4/2020 6:11:57 PM
 *  Author: Akil
 */ 

#include "Motor.h"
#include <avr/io.h>
#include "../configure.h"
#include "../LCD/LCD.h"

#include <stdint.h>

#define abs(x) (((x)>0) ? (x) : -(x))

//Using two channels of timer0 to control motor speed

void Motor::init() {
    //non inverting
    //fast pwm wgm = 3
    //COM 2
    DDRL |= (1 << 3) | (1 << 4);

    DDRB |= (1 << MOTOR_L_FRONT) | (1 << MOTOR_L_BACK) | (1 << MOTOR_R_FRONT) | (1 << MOTOR_R_BACK);
    PORTB &= ~((1 << MOTOR_L_FRONT) | (1 << MOTOR_R_FRONT) | (1 << MOTOR_L_BACK) | (1 << MOTOR_R_BACK));

    TCCR5A = (1 << COM5A1) | (1 << COM5B1)  | (1 << WGM51);
    TCCR5B = (1 << CS50)  |  (1 << WGM53);
    ICR5 = 255;

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

void Motor::applyPIDCorrection(int32_t correction) {
    int32_t c = 255 - abs(correction);

    if (c > 255) c = 255;
    if (c < 200) c = 200;

    LCD::setCursor(0,0);
    LCD::display(c);

    if (correction > 0) {
        Motor::setLeftSpeed(c);
        Motor::setRightSpeed(255);
    }
    else {
        Motor::setLeftSpeed(255);
        Motor::setRightSpeed(c);
    }
}

void Motor::stopMotors() {
    stopRightMotor();
    stopLeftMotor();

    setLeftSpeed(0);
    setRightSpeed(0);
}