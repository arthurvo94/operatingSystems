/*
 * producer.h - Interface for producer 
 * Arthur Vo
 * RedID - 823579426
 */
#ifndef PRODUCER_H
#define PRODUCER_H

/*
 * pthread function to produce and publish rider requests to the 
 * broker until reaching the limit of the production, then exit
 */
void * producer(void * voidPtr);

#endif