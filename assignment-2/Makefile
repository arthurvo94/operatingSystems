# Makefile for Assignment-2 - CS 480
# @author Arthur Vo
# RedID - 823579426


CXX=g++
CXXFLAGS=-std=c++11 -g

dicttest : populatetree.o countwords.o dicttree.o main.o
	$(CXX) $(CXXFLAGS) -o dicttest $^ -lpthread

populatetree.o : populatetree.h populatetree.cpp

countwords.o : countwords.h countwords.cpp

dicttree.o : dicttree.h dicttree.cpp

main.o : execstatus.h main.cpp

clean :
	rm *.o