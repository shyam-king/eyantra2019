/*
 * Map.cpp
 *
 * Created: 2/5/2020 10:51:04 AM
 *  Author: Shyam
 */ 

#include "Map.h"
#include "../UART/UART.h"
#include <stdlib.h>

#define abs(x) (((x) > 0) ? (x) : -(x))

Node* NodeMap::nodes = 0;
uint8_t NodeMap::size = 0;
int NodeMap::capacity = 0;
uint8_t* NodeMap::nodeLinks = 0;
bool NodeMap::constructedLinks = false;

Coordinate::Coordinate(int _x, int _y) {
    x = _x;
    y = _y;
}

uint8_t Coordinate::calculateDistance(Coordinate b) {
    return abs(this->x - b.x) + abs(this->y - b.y);
}

Node::Node() {
    type = JUNCTION;
    index = 0;
}

Node::Node(NODETYPE _type, uint8_t i, Coordinate c) {
    index = i;
    type = _type;
    position = c;
}

void NodeMap::init(int _capacity) {
    capacity = _capacity;
    nodes = (Node*)malloc(sizeof(Node) * capacity);
    size = 0;
}

uint8_t NodeMap::addNode(NODETYPE type, Coordinate pos) {
    if (constructedLinks)
        return size;

    nodes[size].index = size;
    nodes[size].position = pos;
    nodes[size].type = type;
    return size++;
}

void NodeMap::constructNodes() {
    nodeLinks = (uint8_t*)malloc(sizeof(uint8_t) * capacity * capacity);
    constructedLinks = true;
}

void NodeMap::connectNodes(uint8_t i, uint8_t j) {
    *(nodeLinks + (i*size+j)) = 1;
    *(nodeLinks + (j*size+i)) = 1;
}

void NodeMap::clearMem() {
    free(nodes);
    free(nodeLinks);
}

void NodeMap::printAllConnections() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int k = *(nodeLinks + (i*size + j));
            Serial::print(k);
            Serial::print("\t");
        }
        Serial::print("\n");
    }
}