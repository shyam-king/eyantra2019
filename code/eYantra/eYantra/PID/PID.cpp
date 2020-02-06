/*
 * PID.cpp
 *
 * Created: 2/4/2020 6:08:37 PM
 *  Author: Shyam
 */ 

#include "PID.h"
#include "../UART/UART.h"
#include <stdint.h>

int* PID::currentValue = 0;
int PID::targetValue = 0;
float PID::kp = 0;
float PID::kd = 0;
int32_t PID::lastError = 0;

void PID::init(int* _currentValue, int _targetValue, float _kp, float _kd) {
    currentValue = _currentValue;
    targetValue = _targetValue;
    kp = _kp;
    kd = _kd;
}

int32_t PID::calculate() {
    int32_t error = (*currentValue) - targetValue;
    int32_t correction = kp * (error) + kd * (error - lastError);
    
    lastError = error;

    return correction;
}