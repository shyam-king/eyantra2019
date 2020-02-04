/*
 * ADC.h
 *
 * Created: 2/1/2020 7:50:09 AM
 *  Author: Jayamathangi
 */ 

#include <stdint.h>

#ifndef ADC_H_
#define ADC_H_

class ADCInterface {
    static bool initialized;
    public:

    static uint16_t read(uint8_t channel);
    static void init();
};

#endif /* ADC_H_ */