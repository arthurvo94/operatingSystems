/*
 * populateTree.cpp - Implementation of populateTree thread function
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

#include "execstatus.h"
#include "populatetree.h"
#include "dicttree.h"
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

/**
 * populateTree - pthread function to read in words from dictionary source text and add them to 
 * dictionary tree
 * 
 * @param voidPtr void pointer to shared data structure
 * @return void* 
 */
void * populateTree( void * voidPtr )
{
    EXEC_STATUS *execData = (EXEC_STATUS *) voidPtr;
    execData->dictRootNode = makeNode();
    struct dictNode *nodeptr;

    //open dictionary file
    ifstream dictstream(execData->filePath[DICTSRCFILEINDEX]);
    string line;

    //get total numbers of chars in file
    execData->totalNumOfCharsInFile[DICTSRCFILEINDEX] = getTotalNumOfCharsInFile(execData->filePath[DICTSRCFILEINDEX]);

    //iterate through every line of dictionary file
    while(getline(dictstream, line))
    {
        char *cline = &line[0]; //convert string  to char *

        char *word = strtok(cline, delimiters); //tokenize line
        while(word != nullptr)
        {
            // call add method to insert word to build the dictionary tree
            add(execData->dictRootNode, word);

            //increment wordCountInFile after word is processed
            execData->wordCountInFile[DICTSRCFILEINDEX]++;

            // read next word
            word = strtok(NULL, delimiters);
        }

        //increment numOfCharsProcessedFromFile by number of chars on current line (+1 for line feed)
        *execData->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] += line.length() + 1;
    }

    //Mark populateTree thread as complete to prompt countWords thread to proceed
    execData->taskCompleted[DICTSRCFILEINDEX] = true;
    
    return NULL;
}