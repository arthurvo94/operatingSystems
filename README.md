# operatingSystems
Collection of course projects demonstrating core concepts and simulating operations performed by operating systems

Note: each project contains a corresponding **_makefile_** to compile program with all dependencies

## assignment-1 – Dictionary Tree
* Implemented a **pointer tree data structure** with the following operations:
  * Insert word to the tree
  * Count all words in the tree starting with a specified string, or prefix
* Functionality:
  * Read in and insert each word to tree from source text file specified in command line agument
  * Read in each word from secondary source text file and count the number of instances that occur with the read prefix in the tree
  * Outputs each word from secondary source along with number of times that word appears as a prefix in the tree

## assignment-2 – Thread Communication
* Implemented the pointer tree data structure and operations from **assignment-1** using POSIX threads to carry out operations in a concurrent fashion
* Threads are required to communicate and coordinate with one another via a shared data structure, passed to each thread through pointers
* Functionality:
  * **_populatetree_** and **_countwords_** threads are spawned simultaneously by the **_main_** thread, coordinating execution through shared data
  * While worker threads execute, main thread displays execution progress of each worker thread with real-time progress bar
* User interface:
  * Provides optional command line arguments (options) to customize visual representation of progress bar and output count threshhold

## assignment-3 –

## assignment-4 –
