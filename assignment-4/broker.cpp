/*
 * broker.cpp - Implementation of broker class 
 * Arthur Vo
 * RedID - 823579426
 */
#include <iostream>

#include "broker.h"

using namespace std;

/*
 * Inserts item to back of buffer.
 *
 * @param item Value representing item to be inserted
 */
void Broker::insert(int item){
    buffer.push(item);  //Push item to back of buffer
}

/*
 * Removes item from front of buffer.
 *
 * @return value representing item that was removed
 */
int Broker::remove(){
    int temp = buffer.front();  //Store item to be removed

    buffer.pop();   //Remove first item 
    return temp;    //Return removed item
}

/*
 * Removes and prints all items in buffer. Used for testing purposes
 */
void Broker::printQueue(){
    //Iterate until no items are remaining
    while (!buffer.empty()) {
        cout << ' ' << buffer.front();  //Print item
        buffer.pop();                   //Remove item
    }
}