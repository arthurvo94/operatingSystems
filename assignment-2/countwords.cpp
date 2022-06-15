/*
 * countwords.cpp - Implementation of countWords pthread function
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

#include "execstatus.h"
#include "countwords.h"
#include "dicttree.h"
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

/**
 * countWords - pthread function to read in words from test file and count the number of words in 
 * dictionary tree that begin with said word. Outputs word and count to file "countwords_output.txt"
 * 
 * @param voidPtr void pointer to shared data structure
 * @return void* 
 */
void * countWords( void * voidPtr )
{
    EXEC_STATUS *execData = (EXEC_STATUS *) voidPtr;
    struct dictNode *nodeptr;
    int count = 0;

    //open testfile
    ifstream teststream(execData->filePath[TESTFILEINDEX]);
    string line;

    //open output file
    std::ofstream outfile;
    outfile.open("countwords_output.txt");

    //Wait for countWords thread to complete before proceeding with execution
    while(!execData->taskCompleted[DICTSRCFILEINDEX]){}

    //get total numbers of chars in file
    execData->totalNumOfCharsInFile[TESTFILEINDEX] = getTotalNumOfCharsInFile(execData->filePath[TESTFILEINDEX]);

    //iterate through every line of test file
    while(std::getline(teststream, line))
    {
        char *cline = &line[0]; //convert string  to char *

        char *word = strtok(cline, delimiters); //tokenize line
        while(word != nullptr)
        {
            //search for node that ends string
            nodeptr = search(execData->dictRootNode, word);

            //count words starting from node returned from search
            count = getCount(nodeptr);

            //increment wordCountInFile after word is processed
            execData->wordCountInFile[TESTFILEINDEX]++;

            //If count is >= specified minimum number of words with a prefix for printing,
            //print word and count to output file.
            if(count >= execData->minNumOfWordsWithAPrefixForPrinting)
                outfile << word << " " << count << endl;

            word = strtok(NULL, delimiters);
        }

        //increment numOfCharsProcessedFromFile by number of chars on current line (+1 for line feed)
        *execData->numOfCharsProcessedFromFile[TESTFILEINDEX] += line.length() + 1;
    }

    outfile.close();

    // //Mark countwords thread as completed
    execData->taskCompleted[TESTFILEINDEX] = true;

    return NULL;
}