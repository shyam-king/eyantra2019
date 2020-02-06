/*
 * Robot.cpp
 *
 * Created: 2/5/2020 4:03:59 PM
 *  Author: Shyam
 */ 

#include "Robot.h"
#include <stdlib.h>
#include "../Motor/Motor.h"
#include "../PID/PID.h"
#include "../Sensors/Sensors.h"
#include "../configure.h"
#include "../UART/UART.h"
#include "../ADC/ADC.h"
#include <util/delay.h>
#include "../LCD/LCD.h"

State Robot::state = PLACED_MATERIAL;
uint8_t Robot::sensorReadings = 0;
Direction Robot::direction = UP;
Node* Robot::node = 0;

int Robot::line = 0;

Requirement *Robot::requirements = 0;
uint8_t Robot::nRequirements = 0;
uint8_t Robot::capacity = 0;

Coordinate Robot::position;

Robot::Task Robot::nextTask;

Requirement::Requirement() {
    material = JUNCTION;
    house = JUNCTION;
}

Requirement::Requirement(NODETYPE a, NODETYPE b) {
    material = a;
    house = b;
}

Direction addDirection(Direction dir, int a) {
    int final = dir + a;
    final = final % 4;
    if (final < 0)
        final += 4;
    return (Direction)final;
}

void Robot::init(uint8_t _nRequirements) {
    capacity = _nRequirements;
    requirements = (Requirement*) malloc(capacity * sizeof(Requirement));

    PID::init(&line, 100, 0.5f, 0.07f);
}

void Robot::setPosition(Coordinate _pos) {
    position = _pos;
}

void Robot::setDirection (Direction dir) {
    direction = dir;
}

void Robot::clearMem() {
    free (requirements);
}

uint8_t Robot::addRequirement(Requirement req) {
    requirements[nRequirements] = req;
    return nRequirements ++;
}

void Robot::updateSensorValues() {
    sensorReadings = (LineSensors::readSensor(0) << 2) |
                    (LineSensors::readSensor(1) << 1) | 
                    (LineSensors::readSensor(2) << 0);

    int _line = LineSensors::readLine();
    if (_line >= 0)
        line = _line;

    //TODO update distance sensors
}

Robot::Task::Task() {
    path = (char*)malloc(20);
}

void Robot::control() {
    updateSensorValues();
    if (sensorReadings == 0b111) {
        Motor::stopMotors();
        Motor::setLeftSpeed(200);
        Motor::setRightSpeed(200);

        Motor::setLeftDirection(Motor::FRONT);
        Motor::setRightDirection(Motor::BACK);

        _delay_ms(1000);

        Motor::stopMotors();

        

        while(1);
    }
    /*
    if (sensorReadings == 0b111) {
        switch (state)
        {
        case PICKED_MATERIAL:
            break;
        
        case PLACED_MATERIAL:
        {
            nextTask.action = PICK;    
            NODETYPE mat = requirements[nRequirements-1].material;
            nextTask.position = NodeMap::getClosestMaterial(mat, position);
        }
            break;
        
        case COMPLETED_REQIREMENTS:
            break;
        }
    }
    /**/

/**/
    Motor::setLeftDirection(Motor::FRONT);
    Motor::setRightDirection(Motor::FRONT);

    Motor::applyPIDCorrection(PID::calculate());
/**/

    //Serial::print((float)PID::calculate());
    //Serial::print("\n");
    //Serial::print(LineSensors::readSensor(0));
    //Serial::print("\t");
    //Serial::print(LineSensors::readSensor(1));
    //Serial::print("\t");
    //Serial::print(LineSensors::readSensor(2));
    //Serial::print("\t");
    //Serial::print(LineSensors::readLine());
    //Serial::print("\n");
    //PID::calculate();
    //_delay_ms(100);
//
/*
    Serial::print((int)ADCInterface::read(0));
    Serial::print("\t");
    Serial::print((int)ADCInterface::read(1));
    Serial::print("\t");
    Serial::print((int)ADCInterface::read(2));
    Serial::print("\t");
    Serial::print(line);
    Serial::print("\n");
    _delay_ms(100);
    */
}

void Robot::setCurrentNode(Node& a) {
    node = &a;
}