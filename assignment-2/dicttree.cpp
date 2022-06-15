/*
 * dicttree.cpp - Implementation of dicttree Trie data structure. Provides functions to add, search, and count words in the dictionary tree
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

 #include "dicttree.h"
 #include <iostream>
 #include <stdio.h>
 #include <stdlib.h>
 #include <cctype>
 #include <sys/stat.h>

using namespace std;

//makeNode - creates and returns a new dictNode initializing isWord to false and elements of children array to NULL
struct dictNode *makeNode(void)
{
    struct dictNode *newNode = new dictNode;
    newNode->isWord = false;

    //initialize children array to NULL
    for(int i = 0; i < NUMOFCHARS; i++)
        newNode->next[i] = NULL;

    return newNode;
}

//add - adds word to dictionary tree, return true if new word is successfully added
bool add(dictNode* currNode, const char *word = nullptr)
{
    int index;

    //iterate through each char of word to add
    for(int i = 0; word[i] != '\0'; i++)
    {
        index = getIndex(tolower(word[i]));

        //create new node if next node does not exist
        if(!currNode->next[index])
            currNode->next[index] = makeNode();
        
        //move to next node
        currNode = currNode->next[index];
    }

    //return false if word already added
    if(currNode->isWord)
    {
        return false;
    }

    //mark end of word
    currNode->isWord = true;
    return true;
}

//search - searches dictionary tree, returning a node pointer to last node of word
dictNode* search(dictNode* currNode, const char *word)
{
    int index;

    //iterate through each char of word to search
    for(int i = 0; word[i] != '\0'; i++)
    {
        index = getIndex(tolower(word[i]));

        //return nullptr if next char is not found
        if(currNode->next[index] == NULL)
            return nullptr;
        
        //move to next node
        currNode = currNode->next[index];
    }

    //return node pointer to last node of word
    return currNode;
}

//getCount - counts number of words in dictionary tree starting from inputted node
int getCount(dictNode* currNode)
{
    int count = 0;

    //return 0 if null
    if(currNode)
    {
        //iterate through children
        for(int i = 0; i < NUMOFCHARS; i++)
        {
            count += getCount(currNode->next[i]); //recursively add count of children nodes
        }

        //count word
        if(currNode->isWord)
            count++;
    }
    
    return count;
}

//getIndex - helper funciton to find index corresponding to inputted char
int getIndex(char c)
{
    //apostrophe returns last element of array
    if(c == '\'')
        return NUMOFCHARS - 1;
    return c - 'a';
}

//getTotalNumOfCharsInFile - calculates the total number of chars in given file
long getTotalNumOfCharsInFile(const char *fileName)
{
    struct stat buffer;
    stat(fileName, &buffer);

    return buffer.st_size;  //Char is one byte, so return total number of bytes
}