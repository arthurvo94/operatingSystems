
/*
 * countwords.cpp - contains main method for inserting and counting words in tree
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 1/26/2022
 */

#include "dicttree.h"
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

const char *delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";

int main(int argc, char** argv)
{
    struct dictNode *root = makeNode();
    struct dictNode *nodeptr;
    int count = 0;


    //open dictionary file
    std::ifstream dictstream(argv[1]);
    std::string line;

    //iterate through every line of dictionary file
    while(std::getline(dictstream, line))
    {
        char *cline = &line[0]; //convert string  to char *

        char *word = strtok(cline, delimiters); //tokenize line
        while(word != nullptr)
        {
            // call add method to insert word to build the dictionary tree
            add(root, word);

            // read next word
            word = strtok(NULL, delimiters);
        }
    }

    //open testfile
    std::ifstream teststream(argv[2]);


    //iterate through every line of test file
    while(std::getline(teststream, line))
    {
        char *cline = &line[0]; //convert string  to char *

        char *word = strtok(cline, delimiters); //tokenize line
        while(word != nullptr)
        {
            //search for node that ends string
            nodeptr = search(root, word);

            //count words starting from node returned from search
            count = countWords(nodeptr);

            //print word and count to stdout
            cout << word << " " << count << endl;

            word = strtok(NULL, delimiters);
            
        }
    }

    return 0;
}