/*
 * dicttree.h - Interface for dicttree implementation
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 1/26/2022
 */

 //define contents of header if not already defined.
 #ifndef DICTTREE_H
 #define DICTTREE_H 

const int NUMOFCHARS = 27; // a-z plus '

struct dictNode
{
    // denotes if node contains the last character of a word
    bool isWord;

    // array of children nodes
    struct dictNode *next[NUMOFCHARS];
};

//function member specifications
struct dictNode *makeNode(void);
bool add(dictNode* currNode, const char *word);
dictNode* search(dictNode* currNode, const char *word);
int countWords(dictNode* currNode);
int getIndex(char c);

 #endif