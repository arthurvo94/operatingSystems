# operatingSystems
Collection of course projects demonstrating core concepts and simulating operations performed by operating systems

Note: each project contains a corresponding **_makefile_** to compile program with all dependencies
 
## assignment-1 – Dictionary Tree
#### Objective: Demonstrate prerequisite knowledge of C++ and fundamental programming concepts i.e. data structures.
* Implemented a **pointer tree data structure** with the following operations:
  * Insert word to the tree
  * Count all words in the tree starting with a specified string, or prefix
* Functionality:
  * Read in and insert each word to tree from source text file specified in command line agument
  * Read in each word from secondary source text file and count the number of instances that occur in the tree with the read-in prefix word
  * Outputs each word from secondary source along with number of times that word appears as a prefix in the tree

## assignment-2 – Dictionary Tree – Thread Communication
#### Objective: Gain familiarity with POSIX threads and using shared data to facilitate thread communication
* Implemented the pointer tree data structure and operations from **assignment-1** using POSIX threads to carry out operations in a concurrent fashion
* Threads are required to communicate and coordinate with one another via a shared data structure, passed to each thread through pointers
* Functionality:
  * **_populatetree_** and **_countwords_** threads are spawned simultaneously by the **_main_** thread. Execution coordinated through shared data
  * While worker threads execute, main thread displays execution progress of each worker thread with real-time progress bar
* User interface:
  * Provides optional command line arguments (options) to customize visual representation of progress bar and output count threshhold

## assignment-3 – Demand Paging with Multi-level Page Table and TLB
#### Objective: Get hands-on, low-level experience and understanding of how the OS implements memory management schemes and page replacement policies as part of the page fault handling  
* Simulation of the _Demand Paging_ virtual memory management scheme using a multi-level page table with address translation caching (Translation Lookaside Buffer, TLB)
* Simulated the functionality of the Memory Management Unit (MMU) to translate Virtual addresses to Physical addresses
* Functionality:
  * Read in virtual addresses from trace file and convert to physical address. Search TLB for virtual page number (VPN) to physical frame number (PFN) mapping information
    * In case of TLB miss (mapping not found in TLB), walk the page table to find mapping (or create mapping if page table misses) and add mapping to TLB. If TLB is full, apply LRU (Least Recently Used) Policy

## assignment-4 – Real-Time Messaging for Ridesharing app
#### Understand the fundamentals of inter-process communication by solving the famous Producer/Consumer problem
