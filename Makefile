CXX=g++
CFLAGS=-Wall --std=c++11

first: Source.cpp
	$(CXX) $(CFLAGS) -c Source.cpp -o Source.o

second: NDArray.hpp
	$(CXX) $(CFLAGS) -c NDArray.hpp -o NDArray.o

all: first second
	$(CXX) $(CFLAGS) Source.o -o Source

clear:
	del *.0 *.gch *.txt *.o

run: all clear
	Source