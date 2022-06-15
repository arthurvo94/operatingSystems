/*
 * broker.h - Interface for broker 
 * Arthur Vo
 * RedID - 823579426
 */
#ifndef BROKER_H
#define BROKER_H

#include <semaphore.h>
#include <time.h>
#include <queue>

#include "ridesharing.h"
#include "broker.h"

using namespace std;

//Constants
#define BUFFERCAPACITY          12      //Size of buffer
#define MAX_HDR                 4       //Maximum num of Human Driver Requests
#define	NSPERMS		            1000000 //One million nanoseconds per millisecond
#define MSPERSEC	            1000    //One thousand milliseconds per second
#define DEFAULT_PRODUCTIONLIMIT 120     //Default number of items to be produced
#define DEFAULT_TIME            0       //Default time to produce/consume item

/*
 * Broker
 * Shared data structure to allow inter-process communication. Provides
 * shared bounded buffer implemented as a queue that supports FIFO operations.
 */
class Broker{
    queue<int> buffer;

public: 
    //Function members
    /*
     * Inserts item to back of buffer.
     *
     * @param item Value representing item to be inserted
     */
    void insert(int item);

    /*
     * Removes item from front of buffer.
     *
     * @return value representing item that was removed
     */
    int remove();

    /*
     * Removes and prints all items in buffer. Used for testing purposes
     */
    void printQueue();

    /* SHARED DATA */
    //Semaphores
    sem_t mutex;            //Binary semaphore. Allows mutually exclusive access to buffer
    sem_t barrier;          //Precedence constraint, allows main thread to wait for all threads to complete
    sem_t humanRequests;    //Number of available HDR slots, ensures no more than 4 HDR in buffer at any time
    sem_t full;             //Number of unconsumed items in buffer
    sem_t empty;            //Number of available slots in buffer

    //Simulation parameter
    int productionLimit;    //Total number of requests

    //Counters
    int productionCount;                        //Count of total number of requests produced
    int consumedCount;                          //Count of total number of requests consumed
    int inRequestQueue[RequestTypeN];           //Count of requests currently in queue
    int produced[RequestTypeN];                 //Count for each RequestType
    int consumed[ConsumerTypeN][RequestTypeN];  //Array of pointers to consumed arrays for each consumer

};
#endif