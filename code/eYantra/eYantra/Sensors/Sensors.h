/*
 * Sensors.h
 *
 * Created: 2/4/2020 5:55:07 PM
 *  Author: Shyam
 */ 


#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>


class LineSensors {
    public:
    static int16_t readLine();
    static int16_t readSensor(int channel);
};

class ProxSensors {
    public:
    static float readSensor(int channel);
};


#endif /* SENSORS_H_ */