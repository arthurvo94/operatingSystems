/*
 * dicttree.cpp - Implementation of dicttree Trie data structure. Provides functions to add, search, and count words in the dictionary tree
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

 #include "pagetable.h"
 #include <limits.h>

using namespace std;

//PageTable constructor definition
PageTable::PageTable(int levelCount){
    this->levelCount = levelCount;
    bitMask = new uint32_t[levelCount];
    bitShift = new int[levelCount];
    entryCount = new int [levelCount];
    lastFrame = 0;
}

//Level constructor definition
Level::Level(PageTable* pageTablePtr, int depth){
    this->pageTablePtr = pageTablePtr;
    this->depth = depth;
    numChildren = pageTablePtr->entryCount[depth];
    //interior level
    if(depth == 0 || depth <= pageTablePtr->levelCount - 1){
        isLeaf = false;


        nextLevelPtr = new Level*[pageTablePtr->entryCount[depth]];
        
        //Initialize pointers to null
        for(int i = 0; i < pageTablePtr->entryCount[depth]; i++){
            nextLevelPtr[i] = 0;
        }
    }
    //leaf level
    else{
        if(depth == pageTablePtr->levelCount)
            depth--;
        isLeaf = true;
        mapPtr = new LeafMap[pageTablePtr->entryCount[depth]];
    }
    isValid = true;
}

//LeafMap default constructor
LeafMap::LeafMap(){
    isValid = false;
    frame = -1;
}

//Lap constructor definition
LeafMap::LeafMap(bool isValid, int frame){
    this->isValid = isValid;
    this->frame = frame;
}
//TLB Entry constructor
TLBEntry::TLBEntry(uint32_t vpn, uint32_t pfn){
    this->vpn = vpn;
    this->pfn = pfn;
}
// LRU Entry constructor
LRUEntry::LRUEntry(uint32_t vpn, int accessTime){
    this->vpn = vpn;
    this->accessTime = accessTime;
}
//TLB constructor
TLB::TLB(int capacity){
    this->capacity = capacity;
    entries = new TLBEntry[capacity];
    currSize = 0;
}
//LRU Table constructor
LRUPolicy::LRUPolicy(){
    entries = new LRUEntry[10];
    currSize = 0;
}

void TLBEntry::setTLBEntry(uint32_t vpn, uint32_t pfn){
    this->vpn = vpn;
    this->pfn = pfn;
    this->isValid = true;
}

void LRUEntry::setLRUEntry(uint32_t vpn, int accessTime){
    this->vpn = vpn;
    this->accessTime = accessTime;
    this->isValid = true;
}

void LRUEntry::setTime(int accessTime){
    this->accessTime = accessTime;
}


//Function member definitions
bool tlbInsert(TLB *tlbPtr, LRUPolicy *lruPtr, uint32_t vpn, uint32_t pfn, int accessTime){
    lruInsert(tlbPtr, lruPtr, vpn, pfn, accessTime);

    //Check if frame already in tlb
    if(tlbLookup(tlbPtr, vpn) != -1){
        return true;
    }
    

    //If TLB is not full, insert at first available space
    if(tlbPtr->currSize < tlbPtr->capacity){
        for(int i = 0; i < tlbPtr->capacity; i++){
            if(!tlbPtr->entries[i].isValid){
                tlbPtr->entries[i].setTLBEntry(vpn, pfn);
                tlbPtr->currSize++;
                return true;
            }
        }
    }

    //Replace element using LRU policy
    int oldestAccessTime = INT_MAX;
    int oldestIn;
    int tempIn = -1;
    //find least recently used entry
    for(int i = 0; i < tlbPtr->capacity; i++){
        tempIn = lruLookup(lruPtr, tlbPtr->entries[i].vpn);
        if(lruPtr->entries[tempIn].accessTime < oldestAccessTime){
            oldestIn = tempIn;
            oldestAccessTime = lruPtr->entries[tempIn].accessTime;
        }
    }

    //update TLB and LRU table
    for(int i = 0; i < tlbPtr->capacity; i++){
        tempIn = lruLookup(lruPtr, tlbPtr->entries[i].vpn);
        if(lruPtr->entries[tempIn].accessTime == oldestAccessTime){
            tlbPtr->entries[i].setTLBEntry(vpn, pfn);
            lruInsert(tlbPtr, lruPtr, vpn, pfn, accessTime);
            return true;
        }
    }

    return false;
}


int tlbLookup(TLB *tlbPtr, uint32_t vpn){
    //iterate through TLB
    for(int i = 0; i < tlbPtr->capacity; i++){
        if(tlbPtr->entries[i].vpn == vpn){
            //TLB hit
            return tlbPtr->entries->pfn;
        }
    }
    
    //TLB miss
    return -1;
}


bool lruInsert(TLB *tlbPtr, LRUPolicy *lruPtr, uint32_t vpn, uint32_t pfn, int accessTime){
    int temp = lruLookup(lruPtr, vpn);
    //check if frame already in LRU
    if(temp != -1){
        lruPtr->entries[temp].setTime(accessTime);
        return true;
    }


    //If LRU is not full, insert at first available space
    if(lruPtr->currSize < 10){
        for(int i = 0; i < 10; i++){
            if(!lruPtr->entries[i].isValid){
                lruPtr->entries[i].setLRUEntry(vpn, accessTime);
                lruPtr->currSize++;
                return true;
            }
        }
    }

    //Replace element using LRU policy
    int oldestAccessTime = INT_MAX;
    //find least recently used entry
    for(int i = 0; i < 10; i++){
        if(lruPtr->entries[i].accessTime < oldestAccessTime){
            oldestAccessTime = lruPtr->entries[i].accessTime;
        }
    }

    for(int i = 0; i < 10; i++){
        if(lruPtr->entries[i].accessTime == oldestAccessTime){
            lruPtr->entries[i].setLRUEntry(vpn, accessTime);
            return true;
        }
    }

    return false;
}

int lruLookup(LRUPolicy *lruPtr, uint32_t vpn){
    //iterate through LRU
    for(int i = 0; i < 10; i++){
        if(lruPtr->entries->vpn == vpn && lruPtr->entries[i].isValid){
            //lru hit, return index
            return i;
        }
    }
    
    //lru miss
    return -1;
}

/*  
 *  virtualAddressToPageNum
 *  Given a virtual address, apply the given bit mask and shift right by the given number of
 *  bits. Returns the virtual page number. This function can be used to access the page
 *  number of any level by supplying the appropriate parameters. 
 */
uint32_t virtualAddressToPageNum (uint32_t virtualAddress, uint32_t mask, uint32_t shift){
    return (virtualAddress & mask) >> shift;
}

/*
 *  pageLookup
 *  Iterate through Page Table to find Map and return pointer to map
 *  Return NULL if page not found, i.e page table miss
 */
LeafMap * pageLookup(PageTable *pageTablePtr, uint32_t virtualAddress){
    int depth = 0;
    uint32_t pageNum;
    Level* currentLevelPtr = pageTablePtr->rootLevelPtr;

    //Page table walk
    while(!currentLevelPtr->isLeaf){
        pageNum = virtualAddressToPageNum(virtualAddress, pageTablePtr->bitMask[depth], pageTablePtr->bitShift[depth]);
        //page table miss
        if(currentLevelPtr->nextLevelPtr[pageNum] == 0)
            return NULL;
        
        currentLevelPtr = currentLevelPtr->nextLevelPtr[pageNum];
        depth++;
    }
    //page table hit
    return currentLevelPtr->mapPtr;
}

/*
 *  pageInsert
 *  
 */
void pageInsert(PageTable *pageTablePtr, uint32_t virtualAddress, uint32_t frame){
    int depth = 0;
    uint32_t pageNum;
    Level* currentLevelPtr = pageTablePtr->rootLevelPtr;

    //page table walk
    while(depth < pageTablePtr->levelCount){
        pageNum = virtualAddressToPageNum(virtualAddress, pageTablePtr->bitMask[depth], pageTablePtr->bitShift[depth]);

        //initialize page if nonexistant
        if(currentLevelPtr->nextLevelPtr[pageNum] == 0 || !currentLevelPtr->nextLevelPtr[pageNum]->isValid){
            currentLevelPtr->nextLevelPtr[pageNum] = new Level(pageTablePtr, depth+1);
        }
  
        currentLevelPtr = currentLevelPtr->nextLevelPtr[pageNum];
        
        if(currentLevelPtr->isLeaf){
            pageNum = virtualAddressToPageNum(virtualAddress, pageTablePtr->bitMask[depth], pageTablePtr->bitShift[depth]);
            // cout << "GP: " << pageNum << endl;

            //initialize map if nonexistant
            if(!currentLevelPtr->mapPtr[pageNum].isValid){
                currentLevelPtr->mapPtr[pageNum].isValid = true;
                currentLevelPtr->mapPtr[pageNum].frame = frame;
                // cout << "newframe: " << currentLevelPtr->mapPtr[pageNum].frame << endl;
            }
            return;
        }
        depth++;
    }
}