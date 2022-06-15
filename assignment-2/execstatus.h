/*
 * execstatus.h - Interface for execstatus struct
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

#ifndef execData_H
#define execData_H

#include "dicttree.h"

//Shared constants and data structures among threads
#define MIN_ARGS 2
#define MAX_ARGS 9
#define NUMOFFILES 2
#define DICTSRCFILEINDEX 0
#define TESTFILEINDEX 1

#define DEFAULT_NUMOF_MARKS 50                  //default number of progress marks for representing 100% progress
#define MIN_NUMOF_MARKS 10                      //minimum number of progress marks for representing 100% progress
#define DEFAULT_HASHMARKINTERVAL 10             //place hash marks in the progress bar every N marks
#define MIN_HASKMARKINTERVAL 0                  //minimum harshmark interval
#define MAX_HASHMARKINTERVAL 10                 //maximum hashmark interval
#define DEFAULT_MINNUM_OFWORDS_WITHAPREFIX 1    //default minimum number for writing to the output

/**
 * struct EXEC_STATUS - shared data structure between main thread and worker threads
 */
typedef struct {
    dictNode *dictRootNode;                     //root node of the dictionary tree

    //Parameters for printing progress bar
    int numOfProgressMarks;                     //number of progress marks for representing 100% progress
    int hashmarkInterval;                       //place hash marks in the progress bar every N marks
    int minNumOfWordsWithAPrefixForPrinting;    //default minimum number of dictionary words starting from a prefix for writing to the output

    const char *filePath[NUMOFFILES];           //Array storing filenames of dictionary source file and test file
    long totalNumOfCharsInFile[NUMOFFILES];     //Array storing total number of chars in dictionary source file and test file
    long *numOfCharsProcessedFromFile[NUMOFFILES];  //Array storing number chars already processesd in dictionary source file and test file
    long wordCountInFile[NUMOFFILES];           //Array storing total number of words in dictionary source file and test file
    bool taskCompleted[NUMOFFILES];             //Array storing boolean flags for completion status of dictionary source file and test file

} EXEC_STATUS;

#endif