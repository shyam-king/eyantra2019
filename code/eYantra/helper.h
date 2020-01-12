/*
 * helper.h
 *
 * Created: 12-Jan-20 11:41:07 PM
 *  Author: Shyam Murugan
 */ 


#ifndef HELPER_H_
#define HELPER_H_

#define HIGH 1
#define LOW 0
#define OUTPUT 1
#define INPUT 0

#define SET_PIN_AS_OUTPUT(port, pin) DDR##port |= (1 << pin)
#define SET_PIN_AS_INPUT(port, pin) DDR##port &= !(1 << pin)


#endif /* HELPER_H_ */