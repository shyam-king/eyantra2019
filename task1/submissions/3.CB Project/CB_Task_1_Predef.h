#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>

#define LIN_MAX 10
#define FRONT_IR_ADC_CHANNEL 4
#define LEFT_IR_ADC_CHANNEL 5
#define RIGHT_IR_ADC_CHANNEL 6
#define VIS_SEN_INIT_VAL 1000
#define LEFT_PIXEL 0
#define MIDDLE_PIXEL 3
#define RIGHT_PIXEL 6
extern "C" {
#include "extApi.h"
}


int initial(void);
void threadCalls(void);
void forward(void);
void back(void);
void left(void);
void right(void);
void soft_left(void);
void soft_right(void);
void stop(void);
void velocity(int, int);
unsigned char ADC_Conversion(unsigned char);
void initSensors(void);
void _delay_ms(unsigned int);
void filter_red(void);
void filter_blue(void);
void filter_green(void);
void filter_clear(void);
void pick(void);
void place(void);
int init(void);
void cleanUp(void);
