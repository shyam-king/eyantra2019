/*
 * Servo.h
 *
 * Created: 2/1/2020 11:22:55 AM
 *  Author: Jayamathangi
 */ 


#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

class Servo {
    static bool initialized;

    static uint16_t map(float angle);

    public:
    enum Channel{
        LEFT,
        RIGHT,
        CENTER
    };

    static void init();
    static void setAngle(Channel channel, float angle);
    static void pickSequence();
};



#endif /* SERVO_H_ */