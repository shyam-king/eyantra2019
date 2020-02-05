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
#include "Buzzer/Buzzer.h"
#include "Map/Map.h"

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

    _delay_ms(100);
    Servo::setAngle(Servo::LEFT, 90);
    Servo::setAngle(Servo::RIGHT, 90);
    Servo::setAngle(Servo::CENTER, 90);

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
        // for (int i = 0; i <= 180; i += 20) {
        //     Serial::print("Servo angle: ");
        //     Serial::print(i);
        //     Serial::print("\n");

        //     Servo::setAngle(Servo::RIGHT, i);
        //     Servo::setAngle(Servo::LEFT, i);
        //     Servo::setAngle(Servo::CENTER, i);
        //     _delay_ms(1000);
        // }
    }
}