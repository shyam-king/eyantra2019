/*
 * PID.h
 *
 * Created: 2/4/2020 6:08:30 PM
 *  Author: Shyam
 */ 


#ifndef PID_H_
#define PID_H_

#include <stdint.h>

class PID {
    static int* currentValue;
    static int targetValue;
    static float kp, kd; 

    static int32_t lastError;
    public:
    
    static void init(int*, int, float, float);
    static int32_t calculate();
};



#endif /* PID_H_ */