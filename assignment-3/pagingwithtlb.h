/*
 * countwords.h - Interface for countwords implementation
 * Arthur Vo
 * RedID - 823579426
 * CS 480
 * 3/1/2022
 */

#ifndef PAGINGWITHTLB_H
#define PAGINGWITHTLB_H

#include "pagetable.h"

void checkFile(const char* trace_file);
void readTraceFile(PageTable *pageTablePtr, TLB *tlbPtr, LRUPolicy *lruPtr, const char* trace_file, int numMemoryAccess, char *outputMode);
void readTraceOffset(PageTable *pageTablePtr, const char* trace_file, int numMemoryAccess);

#endif