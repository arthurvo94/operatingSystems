/*
 * main.cpp - Processes command line arguments and 
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 4/21/2022
 */
#include <unistd.h>
#include <pthread.h>
#include <iostream>

#include "producer.h"
#include "consumer.h"
#include "execstatus.h"
#include "io.h"

using namespace std;

/* 
 * Program creates two producers and two consumers of rider requests as pthreads.
 * Producer threads will produce and publish rider requests to the broker until reaching the
 * limit of the production, then exit.
 * Consumer threads will consume all requests from the broker before exiting.
 * Main thread will wait for the consumer threads to complete consuming the last request
 * before exiting
 */
int main(int argc, char** argv){
    //Declare worker threads
    pthread_t thread_producerHuman, thread_producerRobot, thread_consumerCost, thread_consumerFast;

    //Instantiate shared Broker object
    Broker *broker = new Broker();

    //Initialize Broker counters to 0
    broker->productionCount = 0;
    broker->consumedCount = 0;
    for(int i = 0; i < RequestTypeN; i++){
        broker->produced[i] = 0;
        broker->inRequestQueue[i] = 0;
    }
    for(int i = 0; i < ConsumerTypeN; i++){
        for(int j = 0; j < RequestTypeN; j++){
            broker->consumed[i][j] = 0;
        }
    } 

    //Initialize semaphores, exit if unsuccessful
    if (sem_init(&broker->mutex, 0, 1) == -1 ||   sem_init(&broker->barrier, 0, 0) == -1 ||
        sem_init(&broker->humanRequests, 0, MAX_HDR) == -1 || sem_init(&broker->full, 0, 0) == -1 || 
        sem_init(&broker->empty, 0, BUFFERCAPACITY) == -1)
    {
        cerr << "Unable to initialize semaphore" << endl;
        exit(0);
    }

    //Instantiate and initialize structures for each thread
    EXEC_STATUS *dataPH = new EXEC_STATUS();
    EXEC_STATUS *dataPA = new EXEC_STATUS();
    EXEC_STATUS *dataCC = new EXEC_STATUS();
    EXEC_STATUS *dataCF = new EXEC_STATUS();

    dataPH->type = HumanDriver;
    dataPA->type = RoboDriver;
    dataCC->type = CostAlgoDispatch;
    dataCF->type = FastAlgoDispatch;

    //Initialize broker pointers for structs
    dataPH->broker = broker;
    dataPA->broker = broker;
    dataCC->broker = broker;
    dataCF->broker = broker;

    //Initialize simulation parameters
    broker->productionLimit = DEFAULT_PRODUCTIONLIMIT;
    dataPH->sleepTime = DEFAULT_TIME;
    dataPA->sleepTime = DEFAULT_TIME;
    dataCC->sleepTime = DEFAULT_TIME;
    dataCF->sleepTime = DEFAULT_TIME;
    
    
    //Process optional command line arguments
    int Option;
    while((Option = getopt(argc, argv, "n:c:f:h:a:")) != -1)
    {
        switch (Option)
        {
            /*
            * -n N 
            * N Total number of requests (production limit). Default is 120 if not specified.
            */
            case 'n':
                if(atoi(optarg) < 0)
                {
                    std::cout << "Production limit must be a positive number" << endl;
                    exit(0);
                }
                broker->productionLimit = atoi(optarg);
                break;

            /*
            * -c N 
            * Specifies the number of milliseconds N that the cost-saving dispatcher
            * (consumer) requires dispatching a request
            */
            case 'c': 
                if(atoi(optarg) < 0)
                {
                    std::cout << "Simulated consumption time must be a positive number" << endl;
                    exit(0);
                }
                dataCC->sleepTime = atoi(optarg);
                break;
            /*
            * -f N 
            * Specifies the number of milliseconds N that the fast-matching dispatcher
            * (consumer) requires dispatching a request
            */
            case 'f': 
                if(atoi(optarg) < 0)
                {
                    std::cout << "Simulated consumption time must be a positive number" << endl;
                    exit(0);
                }
                dataCF->sleepTime = atoi(optarg);
                break;
            /*
            * -h N 
            * Specifies the number of milliseconds N required to produce a ride request for
            * a human driver
            */       
            case 'h': 
                if(atoi(optarg) < 0)
                {
                    std::cout << "Simulated consumption time must be a positive number" << endl;
                    exit(0);
                }
                dataPH->sleepTime = atoi(optarg);
                break;
            /*
            * -f N 
            * Specifies the number of milliseconds N required to produce a ride request for
            * an autonomous car
            */
            case 'a': 
                if(atoi(optarg) < 0)
                {
                    std::cout << "Simulated consumption time must be a positive number" << endl;
                    exit(0);
                }
                dataPA->sleepTime = atoi(optarg);
                break;

            default:
                break; 
        }
    }

    //Simultaneously start all producer and consumer threads
    pthread_create(&thread_producerHuman, NULL, &producer, (void *) dataPH);
    pthread_create(&thread_producerRobot, NULL, &producer, (void *) dataPA);
    pthread_create(&thread_consumerCost, NULL, &consumer, (void *) dataCC);
    pthread_create(&thread_consumerFast, NULL, &consumer, (void *) dataCF);

    //Precedence constraint
    sem_wait(&broker->barrier);    
    sem_wait(&broker->barrier);
    sem_wait(&broker->barrier);

    //Convert 2D-Array to array of pointers
    int *cArr[ConsumerTypeN];
    for(int i = 0; i < ConsumerTypeN; i++){
        cArr[i] = broker->consumed[i];
    }

    /*
     * Print out how many of each type of request was produced and how many 
     * of each type were processed by each of the consumer threads.
     */
    io_production_report(broker->produced, cArr);
}