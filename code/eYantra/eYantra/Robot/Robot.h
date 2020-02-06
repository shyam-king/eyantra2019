/*
 * Robot.h
 *
 * Created: 2/5/2020 4:04:08 PM
 *  Author: Shyam
 */ 


#ifndef ROBOT_H_
#define ROBOT_H_

#include "../Map/Map.h"
#include <stdint.h>

enum Direction {
    UP, LEFT, DOWN, RIGHT
};

Direction addDirection(Direction , int);

struct Requirement {
    NODETYPE material;
    NODETYPE house;

    Requirement();
    Requirement(NODETYPE, NODETYPE);
};

enum State{
    PICKED_MATERIAL,
    PLACED_MATERIAL,
    COMPLETED_REQIREMENTS
};

enum Action {
    IDLE, PICK, PLACE
};

class Robot {
    static State state;

    static Coordinate position;
    static Direction direction;
    static Node* node;

    static Requirement *requirements;
    static uint8_t nRequirements;
    static uint8_t capacity;

    static int line;

    static uint8_t sensorReadings;

    struct Task {
        Action action;
        Coordinate position;
        char *path;

        Task();
    };
    static Task nextTask;

    static bool checkForNode();
    static void updateSensorValues();

    public:
    
    static void init(uint8_t);
    static uint8_t addRequirement(Requirement);
    static void setDirection(Direction);
    static void setPosition(Coordinate);
    static void setCurrentNode(Node&);

    static void control();

    static void clearMem();
};




#endif /* ROBOT_H_ */