/*
 * PID.cpp
 *
 * Created: 2/4/2020 6:08:37 PM
 *  Author: Shyam
 */ 

#include "PID.h"

int* PID::currentValue = 0;
int PID::targetValue = 0;
float PID::kp = 0;
float PID::kd = 0;
float PID::lastError = 0;

void PID::init(int* _currentValue, int _targetValue, float _kp, float _kd) {
    currentValue = _currentValue;
    targetValue = _targetValue;
    kp = _kp;
    kd = _kd;
}

float PID::calculate() {
    float error = (*currentValue) - targetValue;
    float correction = kp * (error) + kd * (error - lastError);
    
    lastError = error;

    return correction;
}