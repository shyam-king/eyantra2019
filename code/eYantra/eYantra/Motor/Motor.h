/*
 * Motor.h
 *
 * Created: 2/4/2020 6:12:04 PM
 *  Author: Akil
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

class Motor {
    public:
    enum Dir {
        FRONT, BACK
    };
    static void init();

    static void setLeftDirection(Dir);
    static void setRightDirection(Dir);

    static void setLeftSpeed(uint8_t);
    static void setRightSpeed(uint8_t);

    static void stopLeftMotor();
    static void stopRightMotor();
    static void stopMotors();

    static void applyPIDCorrection(int32_t);
};


#endif /* MOTOR_H_ */