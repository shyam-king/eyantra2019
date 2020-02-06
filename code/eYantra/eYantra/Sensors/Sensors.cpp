/*
 * Sensors.cpp
 *
 * Created: 2/4/2020 5:54:59 PM
 *  Author: Shyam
 */ 

#include "Sensors.h"
#include "../ADC/ADC.h"
#include "../configure.h"

int16_t LineSensors::readSensor(int channel) {
    return (ADCInterface::read(channel) > LINE_SENSOR_THRESHOLD)?1:0;
}

int16_t LineSensors::readLine() {
    int readings[3] = {
        readSensor(0),
        readSensor(1),
        readSensor(2)
    };

    int sum = readings[1] * 100 + readings[2] * 200;
    sum /= readings[0] + readings[1] + readings[2];

    return sum;
}

float ProxSensors::readSensor(int channel) {
    float adcValue = ADCInterface::read(channel + 3);
    return 5500/(adcValue - 3);
}