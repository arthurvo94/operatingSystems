/*
 * pagingwithtlb.cpp - Processes command line options/arguments simulates MMU
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/22/2022
 */

#include <iostream>
#include <cmath>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "pagingwithtlb.h"
#include "tracereader.h"
#include "output_mode_helpers.h"

using namespace std;

int main(int argc, char** argv){
    const uint32_t FULLMASK = 0xFFFFFFFF;

    //Initialize default values for optional arguments
    int numMemoryAccess = -1;
    int cachecapacity = 0;
    char *outputmode = (char*) "summary";
    char *trace_file = NULL;

    //Process optional arguments
    int Option;
    extern int opterr;
    opterr = 0;
    while((Option = getopt(argc, argv, "n:c:o:")) != -1){
        switch(Option){
            /*
            * -n N 
            * Process only the first N memory accesses / references. Processes all addresses if not present.
            */
            case 'n':
                //Check if option >= 0, exit otherwise
                if(atoi(optarg) < 0)
                    exit(1);
                else
                    numMemoryAccess = atoi(optarg);

                break;

            /*
            * -c N Cache capacity of the TLB, i.e., max number of page mapping entries (N) in TLB. 
            * Default is 0 if not specified, meaning NO TLB caching
            */
            case 'c':
                //Check if option >= 0, exit otherwise
                if(atoi(optarg) < 0){
                    cerr << "Cache capacity must be a number, greater than or equal to 0" << endl;
                    exit(1);
                }
                else
                    cachecapacity = atoi(optarg);
                break;

            /*
            * -o mode output mode. Mode is a string that specifies what to be printed to the standard output
            * If no output mode is specified, use default- "summary"
            */
            case 'o':
                //check if argument is valid, exit otherwise
                if(!strcmp(optarg, "bitmasks") || !strcmp(optarg, "virtual2physical") || !strcmp(optarg, "v2p_tlb_pt") || !strcmp(optarg, "vpn2pfn") || !strcmp(optarg, "offset") || !strcmp(optarg, "summary"))
                    outputmode = optarg;
                else
                    exit(1);
                break;

            default:
                //Handle use of negative numbers
                if(Option == 63){
                    cerr << "Level 0 page table must be at least 1 bit" << endl;    //FIXME level i
                    exit(1);
                }
                break; 
        }
    }

    //Initialize page table, TLB, and LRU table
    PageTable *pageTable = new PageTable(argc - optind - 1);    //Call constructor with num of remaining args
    TLB *tlb = new TLB(cachecapacity);
    LRUPolicy *lru = new LRUPolicy();
    int remainingBits = 32; //Magic number for size of each address
    
    //Process mandatory arguments
    if(optind < argc){
        trace_file = argv[optind++];
        checkFile(trace_file);
        //Process remaining level args
        for(int i = 0; i < pageTable->levelCount; i++){
            //Check if level bits > 0, exit otherwise
            if(atoi(argv[optind]) < 1){
                cerr << "Level " << i << " page table must be at least 1 bit" << endl;
                exit(1);
            }
            remainingBits -= atoi(argv[optind]);
            //Check if total level bits <= 28, exit otherwise
            if(remainingBits < 4){ //Magic number 4 = 32 bit - 28 bit
                cerr << "Too many bits used in page tables" << endl;
                exit(1);
            }
            pageTable->bitShift[i] = remainingBits;

            pageTable->entryCount[i] = pow(2.0, atoi(argv[optind++]));

            //Generate bitmask
            if(i == 0)
                pageTable->bitMask[i] = (FULLMASK << pageTable->bitShift[i]);
            else
                pageTable->bitMask[i] = (FULLMASK << pageTable->bitShift[i]) ^ (FULLMASK << pageTable->bitShift[i -1]);
        }
    }
    //Set offsetMask
    pageTable->offsetMask = FULLMASK >> (32 - remainingBits);
    pageTable->offsetBits = remainingBits;

    //Initialize root level in page table
    pageTable->rootLevelPtr = new Level(pageTable, 0);

    //Output modes that do not require TLB simulation
    if(!strcmp(outputmode, "bitmasks")){
        report_bitmasks(pageTable->levelCount, pageTable->bitMask);
        exit(0);
    }if(!strcmp(outputmode, "offset")){
        readTraceOffset(pageTable, trace_file, numMemoryAccess);
        exit(0);
    }

    //Simulate TLB for all other output modes
    readTraceFile(pageTable, tlb, lru, trace_file, numMemoryAccess, outputmode);
    
    //Cleanup
    delete(pageTable);
    return 0;
}

/*
 * readTraceFile - reads in addresses from trace file and simulates MMU with page table and TLB
 */
void readTraceFile(PageTable *pageTablePtr, TLB *tlbPtr, LRUPolicy *lruPtr, const char* trace_file, int numMemoryAccess, char *outputMode){
    p2AddrTr mtrace;
    FILE * tracef_h = fopen(trace_file, "r");
    LeafMap* mapPtr;
    uint32_t* pages = new uint32_t[pageTablePtr->levelCount];
    uint32_t vAddr;
    uint32_t vPageNum;
    uint32_t pAddr = 0x0;
    int pfn;
    int temp;
    int linesAccessed = 0;
    int tlbHitCounter = 0;
    int ptHitCounter = 0;
    int ptMissCounter = 0;
    bool tlbHit;
    bool ptHit;
    
    //Iterate through numMemoryAccess lines in tracefile. If numMemoryAccess = -1, iterate through entire file
    while(NextAddress(tracef_h, & mtrace) && (linesAccessed < numMemoryAccess || numMemoryAccess == -1)){    //tracef_h - file handle from fopen
        tlbHit = false;
        ptHit = false;
        vAddr = mtrace.addr;
        vPageNum = vAddr >> pageTablePtr->offsetBits;
        
        //Search TLB
        pfn = tlbLookup(tlbPtr, vPageNum);
        if(pfn == -1){  //TLB MISS
            tlbHit = false;

            //Search Page Table
            mapPtr = pageLookup(pageTablePtr, vAddr);
            if(!mapPtr){    //pageTable miss
                ptHit = false;
                ptMissCounter++;
                pageInsert(pageTablePtr, vAddr, pageTablePtr->lastFrame);
                pageTablePtr->lastFrame += 1;   //Update most recently used frame number
                mapPtr = pageLookup(pageTablePtr, vAddr);
            }
            else{           //pageTable hit
                ptHit = true;
                ptHitCounter++;
            }
            temp = virtualAddressToPageNum(vAddr, pageTablePtr->bitMask[pageTablePtr->levelCount - 1], pageTablePtr->bitShift[pageTablePtr->levelCount - 1]);
            pfn = mapPtr[temp].frame;

            //Add page to TLB
            tlbInsert(tlbPtr, lruPtr, vPageNum, pfn, linesAccessed);
        }
        else{   //TLB HIT
            tlbHit = true;
            tlbHitCounter++;
        }

        //fill pages array
        for(int i = 0; i < pageTablePtr->levelCount; i++){
            pages[i] = virtualAddressToPageNum(vAddr, pageTablePtr->bitMask[i], pageTablePtr->bitShift[i]);
        }

        temp = virtualAddressToPageNum(vAddr, pageTablePtr->bitMask[pageTablePtr->levelCount - 1], pageTablePtr->bitShift[pageTablePtr->levelCount - 1]);
        pAddr = (pfn << pageTablePtr->offsetBits) | (vAddr & pageTablePtr->offsetMask);

        
        //Print depending on which output mode is selected
        if(!strcmp(outputMode, "offset")){
            hexnum(vAddr & pageTablePtr->offsetMask);
        }
        if(!strcmp(outputMode, "virtual2physical")){
            report_virtual2physical(vAddr, pAddr);
        }
        if(!strcmp(outputMode, "vpn2pfn")){
            report_pagemap(pageTablePtr->levelCount, pages, pfn);
        }
        if(!strcmp(outputMode, "v2p_tlb_pt")){
            report_v2pUsingTLB_PTwalk(vAddr, pAddr, tlbHit, ptHit);
        }
        linesAccessed++;
    } 
    //Print summary after processing is completed 
    if(!strcmp(outputMode, "summary")){
        uint32_t totalEntries = 0;
        for(int i = 0; i < pageTablePtr->levelCount; i++){
            totalEntries += pageTablePtr->entryCount[i];
        }
        report_summary(pow(2.0, pageTablePtr->offsetBits), tlbHitCounter, ptHitCounter, linesAccessed, pageTablePtr->lastFrame, totalEntries * sizeof(Level));
    }
}

//helper method for output mode Offset
void readTraceOffset(PageTable *pageTablePtr, const char* trace_file, int numMemoryAccess){
    p2AddrTr mtrace;
    uint32_t vAddr;
    FILE * tracef_h = fopen(trace_file, "r");
    int linesAccessed = 0;

    //iterate through trace file
    while(NextAddress(tracef_h, & mtrace) && (linesAccessed < numMemoryAccess || numMemoryAccess == -1)){    //tracef_h - file handle from fopen
        vAddr = mtrace.addr;
        hexnum(vAddr & pageTablePtr->offsetMask);

        linesAccessed++;
    } 
}

//helper method to check if file can be opened
void checkFile(const char* trace_file){
    struct stat buffer;
    //exit gracefully if stat fails
    if(stat(trace_file, &buffer) != 0){
        cerr << "Unable to open <<" << trace_file << ">>" << endl;
        exit(1);
    }
}