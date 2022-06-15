# Makefile for Assignment-2 - CS 480
# @author Arthur Vo
# RedID - 823579426

CXX=g++
CXXFLAGS=-std=c++11 -g

pagingwithtlb : pagingwithtlb.o tracereader.o output_mode_helpers.o pagetable.o
	$(CXX) $(CXXFLAGS) -o pagingwithtlb $^

#populatetree.o : populatetree.h populatetree.cpp

pagetable.o : pagetable.h pagetable.cpp

output_mode_helpers.o : output_mode_helpers.h output_mode_helpers.cpp

tracereader.o : tracereader.h tracereader.cpp

pagingwithtlb.o : pagingwithtlb.cpp pagingwithtlb.h

clean :
	rm *.o