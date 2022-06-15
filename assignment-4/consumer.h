/*
 * consumer.h - Interface for consumer 
 * Arthur Vo
 * RedID - 823579426
 */
#ifndef CONSUMER_H
#define CONSUMER_H

/*
 * pthread function to consume requests all requests
 * from the broker before exiting
 */
void * consumer(void * voidPtr);

#endif