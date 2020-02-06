/*
 * eYantra.cpp
 *
 * Created: 2/1/2020 7:42:31 AM
 *  Author: Shyam
 */ 

#include "configure.h"

#define F_CPU 14745600

#include <avr/io.h>
#include <util/delay.h>


#include "UART/UART.h"
#include "ADC/ADC.h"
#include "LCD/LCD.h"
#include "Motor/Motor.h"
#include "Servo/Servo.h"
#include "Sensors/Sensors.h"
#include "Buzzer/Buzzer.h"
#include "Map/Map.h"
#include "Robot/Robot.h"

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
    Buzzer::init();
    Robot::init(3);
    LCD::setCursor(0,0);


/*
    Motor::setLeftDirection(Motor::FRONT);
    Motor::setRightDirection(Motor::FRONT);
    Motor::setLeftSpeed(255);
    for (int i = 255; i >= 0; i -= 5) {
        Motor::setRightSpeed(i);
        Serial::print(i);
        Serial::print("\n");
        LCD::setCursor(0,0);
        LCD::display(i);
        _delay_ms(1000);
    }

    while(1);

    /**/

    /*

    Servo::setAngle(Servo::CENTER, CENTER_TOP);
    _delay_ms(1000);
    for (float ang = 90; ang <= 180; ang += 10) {
        Servo::setAngle(Servo::RIGHT, ang);
        Servo::setAngle(Servo::LEFT, 180 - ang);
        Serial::print(ang);
        Serial::print("\n");
        _delay_ms(2000);
    }
    while (1);
    /**/

    Servo::setAngle(Servo::CENTER, CENTER_TOP);
    Servo::setAngle(Servo::LEFT, LEFT_PLACE);
    Servo::setAngle(Servo::RIGHT, RIGHT_PLACE);

    _delay_ms(10000);


    Motor::setLeftDirection(Motor::FRONT);
    Motor::setRightDirection(Motor::FRONT);

    Motor::setLeftSpeed(255);
    Motor::setRightSpeed(255);
    float x;

    do {
        x = ProxSensors::readSensor(0);
        Serial::print(x);
        Serial::print("\n");
    } while (x >= 16);

    Motor::setLeftDirection(Motor::BACK);
    Motor::setRightDirection(Motor::BACK);
    _delay_ms(100);

    Motor::stopMotors();

    _delay_ms(5000);
    Servo::pickSequence();
    while(1);


/*
    NodeMap::init(5);

    NodeMap::addNode(JUNCTION, Coordinate(0, 0));
    NodeMap::addNode(JUNCTION, Coordinate(1, 0));
    NodeMap::addNode(JUNCTION, Coordinate(2, 0));
    NodeMap::addNode(JUNCTION, Coordinate(0, 3));
    NodeMap::addNode(JUNCTION, Coordinate(1, 3));
    
    NodeMap::constructNodes();

    NodeMap::connectNodes(0, 1);
    NodeMap::connectNodes(0, 4);
    NodeMap::connectNodes(1, 2);

    NodeMap::printAllConnections();
/**/
    while(1) {
        //Robot::control();
    }
}

