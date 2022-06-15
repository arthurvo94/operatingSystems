/*
 * populatetree.h - Interface for populatetree implementation
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

#ifndef POPULATETREE_H
#define POPULATETREE_H

void * populateTree( void * voidPtr );

//methods to be implemented in main.cpp
void printProgressBar(const int fileIndex, void * voidPtr);
void checkFile(const char* fileName);

#endif