//You are allowed to define your own function to fulfill the requirement of tasks
//Dont change the name of following functions

#include "CB_Task_1_Sandbox.h"

#define OVERSHOOT_DELAY 250
#define TURN_OVERSHOOT 280
#define KP 1.2f
#define KD -.5f
#define TURN_SPEED 150


char junctionsToTraverse[] = "RSLSLRLSSRRLRSLE"; //The path to follow
bool complete = false; //if the goal has been reached
unsigned char currentJunction = 0; //The current junction being processed
unsigned char sensor = 0, prevValidSensor = 0; //The sensor readings in a better format
bool blackMode = false; //for the white line following in the last segment

/*
*
* Function Name: forward_wls
* Input: node
* Output: void
* Logic: Uses white line sensors to go forward by the number of nodes specified
* Example Call: forward_wls(2); //Goes forward by two nodes
*
*/
void forward_wls(unsigned char node)
{

}
/*
*
* Function Name: left_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn left until black line is encountered
* Example Call: left_turn_wls(); //Turns right until black line is encountered
*
*/
void left_turn_wls(void)
{

}

/*
*
* Function Name: right_turn_wls
* Input: void
* Output: void
* Logic: Uses white line sensors to turn right until black line is encountered
* Example Call: right_turn_wls(); //Turns right until black line is encountered
*/
void right_turn_wls(void)
{

}

/*
*
* Function Name: e_shape
* Input: void
* Output: void
* Logic: Use this function to make the robot trace a e shape path on the arena
* Example Call: e_shape();
*/
void e_shape(void)
{

}

/*
*
* Function Name: readSensors
* Input: void
* Output: void
* Logic: Reads the sensor values and sets the sensor value accordingly
* Example Call: readSensors();
*/
void readSensors() {
	sensor = (ADC_Conversion(3) == 255) |
		((ADC_Conversion(2) == 255) << 1) |
		((ADC_Conversion(1) == 255) << 2);

	if (sensor > 0)
		prevValidSensor = sensor;
}

/*
*
* Function Name: junctionDetect
* Input: void
* Output: void
* Logic: Uses the sensor readings and detects whether there is a junction
* Example Call: junctionDetect();
*/
void junctionDetect() {
	readSensors();
	if (blackMode && sensor == 0b010) {
		blackMode = false;
	}

	if (sensor == 0b101 && !blackMode) {
		blackMode = true;
	}

	if (sensor == 0b111 && !blackMode) {
		stop();
		switch (junctionsToTraverse[currentJunction]) {
		case 'R':
			forward();
			_delay_ms(OVERSHOOT_DELAY);
			right();
			velocity(TURN_SPEED, TURN_SPEED);
			_delay_ms(TURN_OVERSHOOT);
			while (sensor != 0b010) {
				readSensors();
			}
			currentJunction++;
			break;

		case 'L':
			forward();
			_delay_ms(OVERSHOOT_DELAY);
			left();
			velocity(TURN_SPEED, TURN_SPEED);
			_delay_ms(TURN_OVERSHOOT);
			while (sensor != 0b010) {
				readSensors();
			}
			currentJunction++;
			break;

		case 'S':
			forward();
			while (sensor == 0b111) {
				readSensors();
			}
			currentJunction++;
			break;
		
		case 'E':
			stop();
			complete = true;
			break;
		}

		forward();
	}
}


/*
*
* Function Name: lineSensorRead
* Input: void
* Output: float: the value corresponding to the position of line (0 - 200)
* Logic: Reads the ADC values of the line sensors and returns a 
			equal weighted average
* Example Call: floatr line = lineSensorRead();
*/

float lineSensorRead() {
	float line = 0;
	float sensors[] = {
		ADC_Conversion(1),
		ADC_Conversion(2),
		ADC_Conversion(3)
	};

	if (sensors[0] == sensors[1] && sensors[1] == sensors[2] && sensors[0] == 0)
		return 100.0f;

	if (!blackMode) {
		line = sensors[1] * 100.0f + sensors[2] * 200.0f;
		line /= sensors[0] + sensors[1] + sensors[2];
	}
	else {
		line = (255 - sensors[1]) * 100.0f + (255 - sensors[2]) * 200.0f;
		line /= 765 - (sensors[0] + sensors[1] + sensors[2]);
	}
		

	return line;
}

/*
*
* Function Name: pidControl
* Input: void
* Output: void
* Logic: This function sets the velocity of the motor based on the sensor readings
		implementing a PD controller
* Example Call: pidControl();
*/

void pidControl() {
	static float lastError = 0;
	float error = lineSensorRead() - 100.0f;

	//override lineSensorValue if in white area
	if (sensor == 0b000) {
		if (prevValidSensor == 0b001) error = 100.0f;
		else if (prevValidSensor == 0b100) error = -100.0f;
	}

	int velL = 255, velR = 255;

	int correction = error * KP + (error - lastError) * KD;
	lastError = error;
	
	if (error != 0) {
		velL = 125;
		velR = 125;
	}
	
	if (correction > 0)
	{
		velR -= correction;
	}
	else
	{
		velL += correction;
	}

	velocity(velL, velR);
	
	
}

/*
*
* Function Name: Task_1_1
* Input: void
* Output: void
* Logic: Use this function to encapsulate your Task 1.1 logic
* Example Call: Task_1_1();
*/
void Task_1_1(void)
{
	// Write your task 1.1 Logic here
	forward();
	while (!complete) {
		junctionDetect();
		pidControl();
	}
}

/*
*
* Function Name: Task_1_2
* Input: void
* Output: void
* Logic: Use this function to encapsulate your Task 1.2 logic
* Example Call: Task_1_2();
*/
void Task_1_2(void)
{
	//write your task 1.2 logic here
}