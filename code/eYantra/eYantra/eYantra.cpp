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
#include "LCD/LCD.h"
#include "Motor/Motor.h"
#include "Servo/Servo.h"
#include "Sensors/Sensors.h"

void enableGlobalInterrupts() {
    SREG |= (1 << 7);
}

int main(void) {
    //initialize components
    enableGlobalInterrupts();
    Serial::init();
    ADCInterface::init();
    LCD::init();
    Motor::init();
    Servo::init();

    while(1) {
        for (int i = 0; i <= 180; i += 20) {
            Serial::print("Servo angle: ");
            Serial::print(i);
            Serial::print("\n");

            Servo::setAngle(Servo::RIGHT, i);
            Servo::setAngle(Servo::LEFT, i);
            Servo::setAngle(Servo::CENTER, i);
            _delay_ms(1000);
        }
    }
}