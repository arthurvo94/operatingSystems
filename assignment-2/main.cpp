/*
 * main.cpp - Processes command line options/arguments and schedules worker threads
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

#include "execstatus.h"
#include "populatetree.h"
#include "countwords.h"
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char** argv)
{
    //declare worker threads
    pthread_t thread_populateTree, thread_countWords;

    //initialize shared data structure
    EXEC_STATUS *execData = new EXEC_STATUS();

    //initialize array pointers
    execData->numOfCharsProcessedFromFile[DICTSRCFILEINDEX] = new long;
    execData->numOfCharsProcessedFromFile[TESTFILEINDEX] = new long;

    //initialize parameters for printing progress bar to default values
    execData->numOfProgressMarks = DEFAULT_NUMOF_MARKS;
    execData->hashmarkInterval = DEFAULT_HASHMARKINTERVAL;
    execData->minNumOfWordsWithAPrefixForPrinting = DEFAULT_MINNUM_OFWORDS_WITHAPREFIX;

    //Process optional arguments
    int Option;
    while((Option = getopt(argc, argv, "p:h:n:")) != -1)
    {
        switch (Option)
        {
            /*
            * -p N 
            * N progress marks (either hyphen or #) for displaying 100% progress of a 
            * thread execution, default is 50 if not specified. Exits if N < 10.
            */
            case 'p':
                if(atoi(optarg) < MIN_NUMOF_MARKS)
                {
                    std::cout << "Number of progress marks must be a number and at least 10" << endl;
                    exit(0);
                }
                execData->numOfProgressMarks = atoi(optarg);
                break;

            /*
            * -h N 
            * place a hash mark “#” in the progress bar every N characters, default is 10 
            * if not specified. Exits if an out-of-range number (<=0 or > 10) is specified.
            */
            case 'h': 
                if(atoi(optarg) <= MIN_HASKMARKINTERVAL || atoi(optarg) > MAX_HASHMARKINTERVAL)
                {
                    std::cout << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10" << endl;
                    exit(0);
                }
                execData->hashmarkInterval = atoi(optarg);
                break;

            /*
            * -n N 
            * print word and its count to an output file (countwords_output.txt) only if the
            * number of dictionary words starting with the word is equal to or greater than N,
            * default is 1 if not specified. 
            */
            case 'n':
                execData->minNumOfWordsWithAPrefixForPrinting = atoi(optarg);
                break;

            default:
                break; 
        }
    }

    //Process mandatory arguments
    int idx = optind;

    if(idx + 2 == argc)
    {
        execData->filePath[DICTSRCFILEINDEX] = argv[optind];
        execData->filePath[TESTFILEINDEX] = argv[optind + 1];
    }
    else
        exit(0);    //exit gracefully if two mandatory arguments are missing

    //Test files before creating threads
    checkFile(execData->filePath[DICTSRCFILEINDEX]);
    checkFile(execData->filePath[TESTFILEINDEX]);

    //Simultaneously start populateTree and countWords worker threads
    pthread_create(&thread_populateTree, NULL, &populateTree, (void *) execData);
    pthread_create(&thread_countWords, NULL, &countWords, (void *) execData);

    //Print populateTree progress bar and mark thread as complete to prompt countwords to continue execution
    printProgressBar(DICTSRCFILEINDEX, (void *) execData);

    //Print countWords progress bar and mark thread as complete
    printProgressBar(TESTFILEINDEX, (void *) execData);

    //Wait for both worker threads to complete before terminating main thread
    while(!execData->taskCompleted[DICTSRCFILEINDEX] || !execData->taskCompleted[TESTFILEINDEX]){}

    return 0;
}

/**
 * printProgressBar - helper function to print a live progress bar to std out
 * 
 * @param fileIndex index of file for progress bar 
 * @param voidPtr void pointer to shared data structure
 */
void printProgressBar(const int fileIndex, void * voidPtr)
{
    EXEC_STATUS *execData = (EXEC_STATUS *) voidPtr;

    float dashInterval = 1.0 / (float) execData->numOfProgressMarks;
    float progressPercentage = 0.0;
    int count = 0;

    //Wait until thread updates totalNumOfCharsInFile to proceed
    while(execData->totalNumOfCharsInFile[fileIndex] == 0){}

    for(int i = 1; i <= execData->numOfProgressMarks; i++)
    {
        //Wait for progress percentage to reach reach the next dashInterval
        while(progressPercentage < i * dashInterval)
        {
            //continue updating progress percentage with live data from thread function
            progressPercentage = (double) *execData->numOfCharsProcessedFromFile[fileIndex] 
            / (double) execData->totalNumOfCharsInFile[fileIndex];
        }

        count++;

        //print '#' if hashmark interval is reached, '-' otherwise
        if(count == execData->hashmarkInterval)
        {
            cout << "#" << flush;
            count = 0;  //reset count to indicate hashmark interval has been reached
        }
        else
            cout << "-" << flush;
    }

    cout << endl << "There are " << execData->wordCountInFile[fileIndex] 
    << " words in " << execData->filePath[fileIndex] << "." << endl;
}

void checkFile(const char* fileName)
{
    struct stat buffer;
    if(stat(fileName, &buffer) != 0)    //exit gracefully if stat fails
    {
        std::cout << "Unable to open <<" << fileName << ">>" << endl;
        exit(1);
    }
}