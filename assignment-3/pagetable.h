/*
 * dicttree.h - Interface for dicttree implementation
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

#ifndef PAGETABLE_H
#define PAGETABLE_H 

#include <stdint.h>
#include <iostream>

class Level;    //Forward declaration to allow cyclic dependency
class TLBEntry;
class LRUEntry;

//PageTable class
class PageTable{
public:
    PageTable(int levelCount);

    //Pointer to root level node
    Level* rootLevelPtr;
    //Number of levels
    int levelCount;
    //Number of offset bits
    int offsetBits;
    //Bit mask for level i
    uint32_t* bitMask;
    //Bit mask for offset
    uint32_t offsetMask;
    //Num of bits to shift level i page bits
    int* bitShift;
    //Num of possible pages for level i
    int* entryCount;
    //Number of unique frames 
    int lastFrame;
};

//LeafMap class
class LeafMap{
public:
    LeafMap();
    LeafMap(bool isValid, int frame);

    bool isValid;
    int frame;
};

//Level class
class Level{
public:
    Level(PageTable* pageTablePtr, int depth);

    bool isLeaf;
    bool isValid = false;

    PageTable* pageTablePtr;
    Level** nextLevelPtr;
    LeafMap* mapPtr;

    int depth;
    int numChildren;

};

class TLB{
public:
    TLB(int capacity);

    int capacity;
    int currSize;
    TLBEntry* entries;
};

class LRUPolicy{
public:
    LRUPolicy();

    int currSize;
    LRUEntry* entries;
};

class TLBEntry{
public:
    TLBEntry(){ isValid = false; };
    TLBEntry(uint32_t vpn, uint32_t pfn);

    void setTLBEntry(uint32_t vpn, uint32_t pfn);

    uint32_t vpn;
    uint32_t pfn;

    bool isValid;
};

class LRUEntry{
public:
    LRUEntry(){ isValid = false; };
    LRUEntry(uint32_t vpn, int accessTime);

    void setLRUEntry(uint32_t vpn, int accessTime);
    void setTime(int accessTime);

    uint32_t vpn;
    int accessTime;

    bool isValid;
};

//function member specifications
bool tlbInsert(TLB *tlbPtr, LRUPolicy *lruPtr, uint32_t vpn, uint32_t pfn, int accessTime);

int tlbLookup(TLB *tlbPtr, uint32_t vpn);

bool lruInsert(TLB *tlbPtr, LRUPolicy *lruPtr, uint32_t vpn, uint32_t pfn, int accessTime);

int lruLookup(LRUPolicy *lruPtr, uint32_t vpn);

uint32_t virtualAddressToPageNum (uint32_t virtualAddress, uint32_t mask, uint32_t shift);

LeafMap * pageLookup(PageTable *pageTablePtr, uint32_t virtualAddress);

void pageInsert(PageTable *pagetablePtr, uint32_t virtualAddress, uint32_t frame);

#endif