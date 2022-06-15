# Makefile for Assignment-4 - CS 480
# @author Arthur Vo
# RedID - 823579426

CXX=g++
CXXFLAGS=-std=c++11 -g

rideshare : producer.o consumer.o broker.o main.o io.o
	$(CXX) $(CXXFLAGS) -pthread -o rideshare $^ 

producer.o : producer.h producer.cpp

consumer.o : consumer.h consumer.cpp

broker.o : broker.h broker.cpp

main.o : main.cpp execstatus.h 

io.o : io.h io.cpp

clean :
	rm *.o