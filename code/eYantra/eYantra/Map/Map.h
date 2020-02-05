/*
 * Map.h
 *
 * Created: 2/5/2020 10:50:27 AM
 *  Author: Shyam
 */ 


#ifndef MAP_H_
#define MAP_H_

#include <stdint.h>

enum NODETYPE {
	JUNCTION,
	START,
	MAT_BLACK,
	MAT_BLUE,
	MAT_RED,
	MAT_GREEN,
	MAT_BROWN,
	MAT_PINK,
	H1, H2, H3, H4, H5
 
};

struct Coordinate {
    int x, y;
    Coordinate(int _x = 0, int _y = 0);
    uint8_t calculateDistance(Coordinate);
};

class Node {
    NODETYPE type;
    uint8_t index;
    Coordinate position;
    friend class NodeMap;

    Node (NODETYPE, uint8_t, Coordinate);
    Node();
};

class NodeMap {
    static Node* nodes;
    static uint8_t size;
    static int capacity;
    static uint8_t* nodeLinks;
    static Node* colorNodes;

    static bool constructedLinks;

    public:

    static void init(int);
    static uint8_t addNode(NODETYPE, Coordinate);
    static void constructNodes();
    static void connectNodes(uint8_t, uint8_t);

    static void clearMem();


    static void printAllConnections();
};

#endif /* MAP_H_ */