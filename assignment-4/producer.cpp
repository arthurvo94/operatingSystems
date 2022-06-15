/*
 * producer.cpp - Implementation of producer 
 * Arthur Vo
 * RedID - 823579426
 */
#include "execstatus.h"
#include "io.h"

using namespace std;

/*
 * pthread function to produce and publish rider requests to the 
 * broker until reaching the limit of the production, then exit
 */
void * producer(void * voidPtr){
    EXEC_STATUS *execData = (EXEC_STATUS *) voidPtr;
    Broker *broker = execData->broker;

    int item = execData->type;  //Type of requests to produce corresponding to producer type

    //Set up delay
    struct timespec SleepTime;
    SleepTime.tv_sec = execData->sleepTime / MSPERSEC;              //Seconds component
	SleepTime.tv_nsec = (execData->sleepTime % MSPERSEC) * NSPERMS; //Nanoseconds component

    while (true) {
        //Simulate production time
        nanosleep(&SleepTime, NULL);

        //Check if available HDR slots if producing HDR
        if(item == HumanDriver)
            sem_wait(&broker->humanRequests);

        //Make sure there is room in buffer, block if full
        sem_wait(&broker->empty);

        //Exit if production limit has been reached
        if(broker->productionCount >= broker->productionLimit){
            break;
        }
            
        /* ENTER CRITICAL SECTION */
        sem_wait(&broker->mutex);       //Acquire mutex
        broker->insert(item);           //Add item to buffer

        //Update respective counters
        broker->productionCount++;
        broker->inRequestQueue[item]++;
        broker->produced[item]++;
        
        //Print which thread produced which item and the current state
        io_add_type((RequestType) item, broker->inRequestQueue, broker->produced);

        sem_post(&broker->mutex);         //Relinquish mutex
        /* EXIT CRITICAL SECTION */

        //Signal consumer
        sem_post(&broker->full);
    }

    //Signal main thread that current thread has completed execution
    sem_post(&broker->barrier);
}