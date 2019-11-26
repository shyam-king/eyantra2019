//You are allowed to define your own function to fulfill the requirement of tasks
//Dont change the name of following functions

#include "CB_Task_1_Sandbox.h"
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <cstring>
#include <stack>

//CONSTANTS
#define OVERSHOOT_DELAY 250
#define BASE_SPEED 255
#define TURN_OVERSHOOT 280
#define ADJUSTMENT_DELAY 200
#define TURN_SPEED 135

//PID CONSTANTS
#define KP 2.0f
#define KD -.55f


using namespace std;

typedef pair<int8_t, int8_t> Coordinate; //To store the position of various nodes

bool house5placement = false; //adjustment for house 5 since it is missing a node
bool complete = false; //if the goal has been reached
unsigned char sensor = 0b010; //The sensor readings in a better format
bool blackMode = false; //for the white line following 
uint8_t totalRequirements = 0; //number of requirements that are yet to be met

//ENUM TO IDENTIFY KIND OF NODES
enum NODETYPE {
	JUNCTION,
	START,
	BLACK,
	BLUE,
	RED,
	GREEN,
	BROWN,
	PINK,
	H1, H2, H3, H4, H5
};

//ENUM Direction denotes the direction that the bot is currently facing
enum Direction {
	UP, RIGHT, DOWN, LEFT
} currentDir; //currentDir is the direction that the bot is currently facing

//Status enum to denote the current status of the bot
//used to determine whether the next step is to get a block, place it or reach the start node.
enum Status {
	PickedUpBlock,
	PlacedBlock,
	done
};


//class Node to represent any node
class Node {
public:
	NODETYPE type; //junction, color, house
	vector<Node*> nodes; //nodes connected with this node
	
	//position used to measure distance between nodes and relative directions
	Coordinate position; 

	Node(NODETYPE _type = JUNCTION, Coordinate _position = Coordinate(0, 0));

	//will join two nodes (must be called once per pair)
	void join(Node &a) {
		a.nodes.push_back(this);
		nodes.push_back(&a);
	}

	//print the position of the node along with a label
	void print (const char* label = "") const {
		printf(label);
		printf("%d, %d", position.first, position.second);
	}

	//measures distance between this node and the given node x
	double distanceTo(Node x) {
		return pow(x.position.first - position.first, 2) + pow(x.position.second - position.second, 2);
	}
} currentNode; //currentNode is the node that the bot is currently present

vector<Node*> colorNodes; //All nodes that contain a cmaterial to be picked
map<NODETYPE, pair<Node*, vector<NODETYPE>>> requirements; //list of houses-indexed requirement list
stack<const Node*> pathToFollow; //list of nodes to be traversed by the bot (next node is always at top)

//constructor for Node
Node::Node(NODETYPE _type, Coordinate _position) {
	type = _type;
	position = _position;
	nodes = vector<Node*>();

	
	if (type >= 2 && type <= 7) { //add the node to colorNodes if it is a color type
		colorNodes.push_back(this);
	}
	else if (type >= H1 && type <= H5) { //create a requirements index if the node is a house type
		requirements[type].first = this;
	}
}

//used for the priority_queue sorting in pathGenerate()
struct NodeCostCompare {
	bool operator()(pair<Node*, double> a, pair<Node*, double> b) {
		return a.second > b.second;
	}
};


/*
*
* Function Name: changeDirection
* Input: amount: number to add (amount * 90 degrees will be added)
* Output: void
* Logic: cincrements the currentDir by amount
stores amount%4 instead (enum Direction contains only 4 constants 0-3)
* Example Call: changeDirection(2);
*/
void changeDirection(int amount) {
	int x = currentDir;
	x += amount;
	x %= 4;
	currentDir = (Direction)(x);
}


/*
*
* Function Name: closest_house_node
* Input: from_node : the node to measure the distance againsts
		 requirement : check if there is a requirement
* Output: Node* : closest house node matching the requirement
* Logic: calculates the distance between each available house node
(that meets the requirement) and the current position and 
returns the node which is closest
* Example Call: Node* nextNode = closest_house_node(currentNode, requirement);
*
*/
Node* closest_house_node(Node &fromNode, NODETYPE requirement) {
	Node* target = NULL;
	double minDistance;
	
	for (auto x : requirements) {
		bool f = false;
		//check for requirement
		for (auto i : x.second.second) {
			if (requirement == i)
			{
				f = true;
				break;
			}
		}

		//if requirement matches, prioritise least distance
		if (f) {
			if (target == NULL) {
				target = x.second.first;
				minDistance = x.second.first->distanceTo(fromNode);
			}
			else if (x.second.first->distanceTo(fromNode) < minDistance) {
				target = x.second.first;
				minDistance = x.second.first->distanceTo(fromNode);
			}
		}
	}
	return target;
}

/*
*
* Function Name: nextJunction
* Input: void
* Output: char : represents the action to be taken
* Logic: based on the nodes available in pathToFollow, return the action to be taken:
		 turning actions : L,R,S
		 picking action: P
		 placing action: p
* Example Call: char action = nextJunction();
*
*/
char nextJunction() {
	const Node* to = pathToFollow.top();
	Coordinate from = to->position;
	currentNode = *to;
	pathToFollow.pop();
	Coordinate pos;
	if (!pathToFollow.empty())
		pos = pathToFollow.top()->position;
	else {
		if (to->type >= H1 && to->type <= H5)
			return 'p';

		if (to->type >= BLACK && to->type <= PINK)
			return 'P';

		if (to->type == START)
			return 'E';
	}

	switch (currentDir) {
	case UP:
		if (pos.first < from.first) {
			return 'L';
		}
		else if (pos.first > from.first) {
			return 'R';
		}
		else if (pos.second > from.second) {
			return 'U';
		}
		else if (pos.second < from.second) {
			return 'S';
		}

	case DOWN:
		if (pos.first < from.first) {
			return 'R';
		}
		else if (pos.first > from.first) {
			return 'L';
		}
		else if (pos.second > from.second) {
			return 'S';
		}
		else if (pos.second < from.second) {
			return 'U';
		}

	case LEFT:
		if (pos.first > from.first) {
			return 'U';
		}
		else if (pos.first < from.first) {
			return 'S';
		}
		else if (pos.second > from.second) {
			return 'L';
		}
		else if (pos.second < from.second) {
			return 'R';
		}

	case RIGHT:
		if (pos.first > from.first) {
			return 'S';
		}
		else if (pos.first < from.first) {
			return 'U';
		}
		else if (pos.second > from.second) {
			return 'R';
		}
		else if (pos.second < from.second) {
			return 'L';
		}
	}

	currentNode = *to;
}

/*
*
* Function Name: closest_color_node
* Input: from_node : the node to measure the distance againsts
* Output: Node* : closest color node 
* Logic: calculates the distance between each available color node 
and the current position and returns the node which is closest
* Example Call: Node* nextColor = closest_color_node(currentNode);
*
*/
Node* closest_color_node(Node& from_node) {
	Node * target = NULL;
	double minDistance;

	for (auto n : colorNodes) {
		bool f = false;

		//check if the current node's block is required
		for (auto x : requirements) {
			for (auto i : x.second.second) {
				if (i == n->type) {
					f = true;
					break;
				}
			}
			if (f) break;
		}

		//prioritise minimum distance
		if (f) {
			if (target == NULL) {
				target = n;
				minDistance = target->distanceTo(from_node);
			}
			else if (n->distanceTo(from_node) < minDistance) {
				target = n;
				minDistance = n->distanceTo(from_node);
			}
		}
	}


	return target;
}

/*
*
* Function Name: pathGenerate
* Input: from_node, to_node
* Output: void 
* Logic: implements A* algorithm and fills the pathToFollow with the shortest
		path from from_node to to_node that the bot follows
* Example Call: pathGenerate(start_node, to_node);
*
*/
void pathGenerate(Node &from_node, Node &to_node) {
	priority_queue<pair<Node*, double>, vector<pair<Node*, double>>, NodeCostCompare> toTraverse; //next node is top
	map<Coordinate, const Node*> cameFrom; //to retrace steps
	vector<Coordinate> traversed; //to prevent checking the same nodes again
	const Node *traversedNode = NULL; //the current node under consideration

	toTraverse.push(pair<Node*, double>(&from_node, 0)); //push the root node
	
	while (!toTraverse.empty()) {
		traversedNode = (toTraverse.top().first);
		traversed.push_back(traversedNode->position);
		toTraverse.pop();

		if (traversedNode->position == to_node.position) {
			break;
		}
		
		for (auto n : traversedNode->nodes) {
			bool found = false;
			for (auto x : traversed) {
				if (n->position == x) {
					found = true;
					break;
				}
			}
			if (!found) {
				double cost = n->distanceTo(*traversedNode) + n->distanceTo(to_node);
				toTraverse.push(pair<Node*, double>(n, cost));
				cameFrom[n->position] = traversedNode;
			}
		}
	};

	//retracing the path using cameFrom values and pushing it to pathToFollow stack
	//stack operations reverses the path automatically
	Coordinate pos = traversedNode->position;
	while (pos != from_node.position) {
		pathToFollow.push(traversedNode);
		traversedNode = cameFrom[pos];
		pos = traversedNode->position;
	}
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
	uint8_t sensorRead = (ADC_Conversion(3) >= 100) |
		((ADC_Conversion(2) >= 100) << 1) |
		((ADC_Conversion(1) >= 100) << 2);

	if (blackMode) sensorRead = 0b111 - sensorRead; //invert in case following white line

	sensor = sensorRead;
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

	if (sensor == 0b101) {
		blackMode = !blackMode;
	}

	if (sensor == 0b111) {
		stop();
		char n = nextJunction();
		switch (n) {
		case 'R':
			changeDirection(1);
			forward();
			_delay_ms(OVERSHOOT_DELAY);
			right();
			velocity(TURN_SPEED, TURN_SPEED);
			_delay_ms(TURN_OVERSHOOT);
			while (sensor != 0b010) {
				readSensors();
			}
			break;

		case 'L':
			changeDirection(3);
			forward();
			_delay_ms(OVERSHOOT_DELAY);
			left();
			velocity(TURN_SPEED, TURN_SPEED);
			_delay_ms(TURN_OVERSHOOT);
			while (sensor != 0b010) {
				readSensors();
			}
			break;

		case 'S':
			forward();
			_delay_ms(500);
			break;

		case 'E':
			forward();
			_delay_ms(OVERSHOOT_DELAY);
			stop();
			complete = true;
			break;

		case 'p':
			changeDirection(2);
			stop();
			if (house5placement) {
				back();
				_delay_ms(50);
			}
			else {
				forward();
			}
			_delay_ms(ADJUSTMENT_DELAY);
			stop();
			place();

			if (house5placement) {
				forward();
				_delay_ms(50);
			}
			else {
				back();
			}
			_delay_ms(ADJUSTMENT_DELAY);
			stop();
			//TURN AROUND
			right();
			velocity(TURN_SPEED, TURN_SPEED);
			_delay_ms(TURN_OVERSHOOT);
			while (sensor != 0b010) {
				readSensors();
			}
			stop();
			complete = true;
			break;

		case 'P':
			changeDirection(2);
			stop();
			pick();

			//TURN AROUND
			right();
			velocity(TURN_SPEED, TURN_SPEED);
			_delay_ms(TURN_OVERSHOOT);
			while (sensor != 0b010) {
				readSensors();
			}
			stop();
			complete = true;
			break;
		}

		if (!complete) forward();
	}
}


/*
*
* Function Name: lineSensorRead
* Input: void
* Output: float: the value corresponding to the position of line (0 - 200)
* Logic: Reads the ADC values of the line sensors and returns a 
			equal weighted average
* Example Call: float line = lineSensorRead();
*/

float lineSensorRead() {
	float line = 0;
	float sensors[] = {
		ADC_Conversion(1),
		ADC_Conversion(2),
		ADC_Conversion(3)
	};

	if (blackMode) { //invert in case of white line following
		sensors[0] = 255 - sensors[0];
		sensors[1] = 255 - sensors[1];
		sensors[2] = 255 - sensors[2];
	}

	if (sensors[0] == sensors[1] && sensors[1] == sensors[2] && sensors[0] == 0)
		return 100.0f;

	line = sensors[1] * 100.0f + sensors[2] * 200.0f;
	line /= sensors[0] + sensors[1] + sensors[2];
	
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
	float error = lineSensorRead();
	error -= 100.0;

	int velL = BASE_SPEED, velR = BASE_SPEED;

	int correction = error * KP + (lastError) * KD;
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
	//State variables
	Status currentStatus = PlacedBlock;
	NODETYPE pickedUpBlock;

	//store the map
	Node START_NODE = Node(START, Coordinate(2, 5));
	Node junctions[] = {
		Node(JUNCTION, Coordinate(0,6)),
		Node(JUNCTION, Coordinate(0,5)),
		Node(BROWN, Coordinate(-1,5)),
		Node(BROWN, Coordinate(1,5)),
		Node(JUNCTION, Coordinate(0,4)),//4
		Node(H1, Coordinate(-1,4)),
		Node(JUNCTION, Coordinate(0,3)),
		Node(GREEN, Coordinate(-1,3)),
		Node(GREEN, Coordinate(1,3)),
		Node(JUNCTION, Coordinate(0,2)),
		Node(H3, Coordinate(-1,2)),
		Node(JUNCTION, Coordinate(0,1)),
		Node(BLUE, Coordinate(-1,1)),
		Node(BLUE, Coordinate(1,1)), //13
		Node(JUNCTION, Coordinate(0,0)),
		Node(JUNCTION, Coordinate(1,0)),
		Node(JUNCTION, Coordinate(2,0)),
		Node(H5, Coordinate(2,1)), //check if it's there
		Node(JUNCTION, Coordinate(3,0)),
		Node(JUNCTION, Coordinate(4,0)),
		Node(JUNCTION, Coordinate(4,1)),
		Node(PINK, Coordinate(3,1)),
		Node(PINK, Coordinate(5,1)),
		Node(JUNCTION, Coordinate(4,2)),
		Node(H4, Coordinate(5,2)),
		Node(JUNCTION, Coordinate(4,3)),
		Node(BLACK, Coordinate(3,3)),
		Node(BLACK, Coordinate(5,3)),
		Node(JUNCTION, Coordinate(4,4)),//28
		Node(H2, Coordinate(5,4)), //29
		Node(JUNCTION, Coordinate(4,5)),
		Node(RED, Coordinate(3,5)),
		Node(RED, Coordinate(5,5)),
		Node(JUNCTION, Coordinate(4,6)),
		Node(JUNCTION, Coordinate(2,6))
	};

	junctions[34].join(junctions[0]);
	junctions[0].join(junctions[1]);
	junctions[1].join(junctions[2]);
	junctions[1].join(junctions[3]);
	junctions[1].join(junctions[4]);
	junctions[4].join(junctions[5]);
	junctions[4].join(junctions[6]);
	junctions[6].join(junctions[7]);
	junctions[6].join(junctions[8]);
	junctions[6].join(junctions[9]);
	junctions[9].join(junctions[10]);
	junctions[9].join(junctions[23]);
	junctions[9].join(junctions[11]);
	junctions[11].join(junctions[12]);
	junctions[11].join(junctions[13]);
	junctions[11].join(junctions[14]);
	junctions[14].join(junctions[16]);
	junctions[16].join(junctions[17]); 
	junctions[16].join(junctions[19]);
	junctions[19].join(junctions[20]);
	junctions[20].join(junctions[21]);
	junctions[20].join(junctions[22]);
	junctions[20].join(junctions[23]);
	junctions[23].join(junctions[24]);
	junctions[23].join(junctions[25]);
	junctions[25].join(junctions[26]);
	junctions[25].join(junctions[27]);
	junctions[25].join(junctions[28]);
	junctions[28].join(junctions[29]);
	junctions[28].join(junctions[30]);
	//junctions[28].join(junctions[4]); (wall following not implemented)
	junctions[30].join(junctions[31]);
	junctions[30].join(junctions[32]);
	junctions[30].join(junctions[33]);
	junctions[33].join(junctions[34]);
	junctions[34].join(START_NODE);

	//fill requirements
	requirements[H1].second.push_back(RED);
	requirements[H1].second.push_back(PINK);
	requirements[H2].second.push_back(BLUE);
	requirements[H2].second.push_back(GREEN);
	requirements[H3].second.push_back(BLACK);
	requirements[H3].second.push_back(RED);
	requirements[H4].second.push_back(BROWN);
	requirements[H5].second.push_back(BROWN);
	requirements[H5].second.push_back(BLACK);

	totalRequirements = 9;
	currentNode = START_NODE;
	currentDir = DOWN;

	while (1) {
		if (currentStatus == PlacedBlock) {
			if (totalRequirements) {
				Node *colorNode = closest_color_node(currentNode);
				pickedUpBlock = colorNode->type;
				pathGenerate(currentNode, *colorNode);

				colorNodes.erase(std::remove(colorNodes.begin(), colorNodes.end(), colorNode), colorNodes.end());
			}
			else {
				pathGenerate(currentNode, START_NODE);
				currentStatus = done;
			}
		}
		else if (currentStatus == PickedUpBlock) {
			//find the closest house with that requirement
			
			Node *houseNode = closest_house_node(currentNode, pickedUpBlock);
			pathGenerate(currentNode, *houseNode);

			if (houseNode->type == H5) house5placement = true;
			else house5placement = false;

			vector<NODETYPE> &temp = requirements[houseNode -> type].second;
			temp.erase(remove(temp.begin(), temp.end(), pickedUpBlock), temp.end());
			totalRequirements--;
		}
		else if (currentStatus == done) {
			return;
		}
		complete = false;

		forward();
		while (!complete) {
			junctionDetect();
			pidControl();
		}

		//toggle between picked and placed states
		if (currentStatus == PlacedBlock) currentStatus = PickedUpBlock;
		else if (currentStatus == PickedUpBlock) currentStatus = PlacedBlock;
	}
}