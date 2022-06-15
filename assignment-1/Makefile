# Makefile for Assignment-1 - CS 480
# @author Arthur Vo
# RedID - 823579426


CXX=g++
CXXFLAGS=-std=c++11 -g


countwords : dicttree.o countwords.o
	$(CXX) $(CXXFLAGS) -o countwords $^

dicttree.o : dicttree.h dicttree.cpp

countwords.o : dicttree.h countwords.cpp

clean :
	rm *.o