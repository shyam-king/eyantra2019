//You are allowed to define your own function to fulfill the requirement of tasks
//Dont change the name of following functions

#include "CB_Task_1_Sandbox.h"
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <cstring>

#define OVERSHOOT_DELAY 250
#define TURN_OVERSHOOT 280
#define KP 1.2f
#define KD -.5f
#define TURN_SPEED 150

using namespace std;

typedef pair<uint8_t, uint8_t> Coordinate;

char junctionsToTraverse[50] = "RSLSLRLSSRRLRSLE"; //The path to follow
bool complete = false; //if the goal has been reached
unsigned char currentJunction = 0; //The current junction being processed
unsigned char sensor = 0, prevValidSensor = 0; //The sensor readings in a better format
bool blackMode = false; //for the white line following in the last segment

//ENUM TO IDENTIFY KIND OF NODES
enum NODETYPE {
	JUNCTION,
	START,
	BROWN,
	GREEN,
	BLUE,
	PINK,
	BLACK,
	RED,
	H1, H2, H3, H4, H5
};

//ENUM Direction for path_generator
enum Direction {
	UP, DOWN, LEFT, RIGHT
};

//class Node to represent any node
class Node {
public:
	NODETYPE type;
	vector<Node*> nodes; //nodes connected with this node
	
	//position used to measure distance between nodes and relative directions
	Coordinate position; 

	Node(NODETYPE _type = JUNCTION, Coordinate _position = Coordinate(0,0) ){
		type = _type;
		position = _position;
		nodes = vector<Node*>();
	}

	//will join two nodes
	void join(Node &a) {
		a.nodes.push_back(this);
		nodes.push_back(&a);
	}

	//measures distance between this node and the given node x
	double distanceTo(Node x) {
		return pow(x.position.first - position.first, 2) + pow(x.position.second - position.second, 2);
	}
} goalNode; // goalNode is used for prioritizing nodes

bool operator<(Node a, Node b) {
	//we want the least distance to be prioritized
	return a.distanceTo(goalNode) > b.distanceTo(goalNode);
}

struct NodeCompare {
	Coordinate position;

	NodeCompare(Node* const a) {
		position = a->position;
	}

	bool operator()(Node const &a) {
		return position == a.position;
	}
};

/*
*
* Function Name: path_generator
* Input: from_node, to_node, path_buffer
* Output: void
* Logic: implements A* algorithm and fills the path_buffer with the shortest
		path from from_node to to_node that the bot follows
* Example Call: path_generator(start_node, brown_node, pathToTraverse);
*
*/
void path_generator(Node &from_node, Node &to_node, char* path_buffer, Direction dir) {
	priority_queue<Node*> toTraverse;
	map<Coordinate, Coordinate> cameFrom;
	vector<Coordinate> traversed;
	const Node* t = NULL, *tt = NULL;

	goalNode = to_node; //so that it is properly sorted
	toTraverse.push(&from_node);
	
	while (!toTraverse.empty()) {
		tt = (toTraverse.top());
		printf("\ntraversing: %d, %d", tt->position.first, tt->position.second);
		traversed.push_back(tt->position);
		toTraverse.pop();

		if (tt->type == to_node.type) {
			//finish searching
			printf("\nTRAVERSE COMPLETE");
			break;
		}
		printf("\n\tSearching child nodes (%d): ", tt->nodes.size());
		for (auto n : tt->nodes) {
			bool found = false;
			printf("\n\t\tchecking %d, %d", n->position.first, n->position.second);
			for (auto x : traversed) {
				if (n->position == x) {
					found = true;
					break;
				}
			}
			if (!found) {
				toTraverse.push(n);
				cameFrom[n->position] = tt->position;
			}
		}
	};

	//check the map
	printf("\nTRAVERSED MAP:");
	for (auto x : cameFrom) {
		printf("\n\t%d,%d -> %d,%d", x.second.first, x.second.second, x.first.first, x.first.second);
	}

	//construct the string corresponding to the path found
	path_buffer[0] = 0; //clear the string
	strcat(path_buffer, "S");
	/*
	Coordinate from, to;
	from = tt->position;
	do {
		to = cameFrom[from];

		switch (dir) {
		case UP:
			if (to.first < from.first) { 
				strcat(path_buffer, "L"); 
				dir = LEFT; 
			}
			else if (to.first > from.first) {
				strcat(path_buffer, "R"); 
				dir = RIGHT;
			}
			else if (to.second < from.second) { 
				strcat(path_buffer, "S"); 
				dir = UP;
			}
			else if (to.second > from.second) {
				strcat(path_buffer, "U");
				dir = DOWN;
			}
			break;

		case DOWN:
			if (to.first < from.first) {
				strcat(path_buffer, "R");
				dir = LEFT;
			}
			else if (to.first > from.first) {
				strcat(path_buffer, "L");
				dir = RIGHT;
			}
			else if (to.second < from.second) {
				strcat(path_buffer, "U");
				dir = UP;
			}
			else if (to.second > from.second) {
				strcat(path_buffer, "S");
				dir = DOWN;
			}
			break;

		case LEFT:
			if (to.first < from.first) {
				strcat(path_buffer, "S");
				dir = LEFT;
			}
			else if (to.first > from.first) {
				strcat(path_buffer, "U");
				dir = RIGHT;
			}
			else if (to.second < from.second) {
				strcat(path_buffer, "R");
				dir = UP;
			}
			else if (to.second > from.second) {
				strcat(path_buffer, "L");
				dir = DOWN;
			}
			break;

		case RIGHT:
			if (to.first < from.first) {
				strcat(path_buffer, "U");
				dir = LEFT;
			}
			else if (to.first > from.first) {
				strcat(path_buffer, "S");
				dir = RIGHT;
			}
			else if (to.second < from.second) {
				strcat(path_buffer, "L");
				dir = UP;
			}
			else if (to.second > from.second) {
				strcat(path_buffer, "R");
				dir = DOWN;
			}
			break;
		}
		
	} while (cameFrom[to] != from_node.position);
	/**/
}

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
	
	//store the map
	Node START_NODE = Node(START, Coordinate(0, 0));
	Node junctions[] = {
		Node(JUNCTION, Coordinate(1,0)),
		Node(JUNCTION, Coordinate(2,0)),
		Node(JUNCTION, Coordinate(3,0)),
		Node(JUNCTION, Coordinate(2,1)),
		Node(JUNCTION, Coordinate(3,1))
	};
	Node END_NODE = Node(BROWN, Coordinate(3, 2));

	START_NODE.join(junctions[0]);
	junctions[0].join(junctions[1]);
	junctions[1].join(junctions[2]);
	junctions[1].join(junctions[3]);
	junctions[2].join(junctions[4]);
	junctions[4].join(END_NODE);

	path_generator(START_NODE, END_NODE, junctionsToTraverse, UP);

	printf("\nfound path: %s", junctionsToTraverse);

	while (1) {
		int a = 10;
	}
}