/*
 * dicttree.h - Interface for dicttree implementation
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

#ifndef DICTTREE_H
#define DICTTREE_H 

const int NUMOFCHARS = 27; // a-z plus '
const static char *delimiters = "\n\r !\"#$%&()*+,-./0123456789:;<=>?@[\\]^_`{|}~";

//dictNode object
struct dictNode
{
    bool isWord;    // denotes if node contains the last character of a word
    struct dictNode *next[NUMOFCHARS];  // array of children nodes
};

//function member specifications
struct dictNode *makeNode(void);
bool add(dictNode* currNode, const char *word);
dictNode* search(dictNode* currNode, const char *word);
int getCount(dictNode* currNode);
int getIndex(char c);
long getTotalNumOfCharsInFile(const char *fileName);

#endif