/*
 * consumer.cpp - Implementation of consumer 
 * Arthur Vo
 * RedID - 823579426
 */
#include "execstatus.h"
#include "io.h"

using namespace std;

/*
 * pthread function to consume requests all requests
 * from the broker before exiting
 */
void * consumer(void * voidPtr){
    EXEC_STATUS *execData = (EXEC_STATUS *) voidPtr;
    Broker *broker = execData->broker;

    int item;   //Stores consumed item type 
    int consumerType = execData->type;  //Identifies which consumer type current thread is simulating

    //Set up delay
    struct timespec SleepTime;
    SleepTime.tv_sec = execData->sleepTime / MSPERSEC;              //Seconds component
	SleepTime.tv_nsec = (execData->sleepTime % MSPERSEC) * NSPERMS; //Nanoseconds component

    //Loop until all produced requests have been consumed
    while (broker->consumedCount < broker->productionLimit) {
        //Block until something to consume
        sem_wait(&broker->full);

        /* ENTER CRITICAL SECTION */
        sem_wait(&broker->mutex);       //Acquire mutex
        item = broker->remove();        //Remove item from buffer

        //Update respective counters
        broker->consumedCount++;
        broker->inRequestQueue[item]--;
        broker->consumed[consumerType][item]++;

        //Print which thread consumed which item and the current state
        io_remove_type((Consumers) consumerType, (RequestType) item, broker->inRequestQueue, broker->consumed[consumerType]);

        sem_post(&broker->mutex);       //Relinquish mutex
        /* EXIT CRITICAL SECTION */

        //Signal Human Driver request producer
        if(item == HumanDriver){
            sem_post(&broker->humanRequests);
        }

        //Signal producer that there is a new available slot in buffer
        sem_post(&broker->empty);
        
        //Simulate consumption time
        nanosleep(&SleepTime, NULL);
    }

    //Signal main thread that current thread has completed execution
    sem_post(&broker->barrier);
}