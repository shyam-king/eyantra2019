/*
 * configure.h
 *
 * Created: 2/4/2020 6:03:48 PM
 *  Author: Shyam
 */ 


#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#define LINE_SENSOR_THRESHOLD 400

// MOTOR PINS
#define MOTOR_R_FRONT 3
#define MOTOR_R_BACK 2

#define MOTOR_L_FRONT 1
#define MOTOR_L_BACK 0

#define MOTOR_L_SPEED OCR5A
#define MOTOR_R_SPEED OCR5B

#define SERVO_MIN 921u
#define SERVO_MAX 4424u
#define SERVO_TOTAL 36864u

#define LEFT_PICK 65
#define LEFT_PLACE 0
#define RIGHT_PICK 65
#define RIGHT_PLACE 120
#define CENTER_TOP 150
#define CENTER_BOTTOM 10


#define COUNT_1MS 230

#endif /* CONFIGURE_H_ */