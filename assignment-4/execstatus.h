/*
 * execstatus.h - Interface for execstatus 
 * Arthur Vo
 * RedID - 823579426
 */
#ifndef EXECSTATUS_H
#define EXECSTATUS_H

#include "broker.h"

/**
 * EXEC_STATUS
 * Data structure to be passed to pthread function.
 * Stores information related to producer/consumer
 * and pointer to shared buffer.
 */
typedef struct {
    Broker *broker; //Bounded buffer pointer

    int type;       //Flag respresenting producer/consumer type. 0 - HumanDriver producer or CostAlgo consumer. 1 - RoboDriver producer or FastAlgo consumer
    int sleepTime;  //Number of milliseconds to simulate producing/consuming a request
} EXEC_STATUS;

#endif