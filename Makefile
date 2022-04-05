CXX=g++
CFLAGS=-Wall --std=c++11

first: Source.cpp NDArray.hpp
	$(CXX) $(CFLAGS) Source.cpp NDArray.hpp -o Source

all: first

clear:
	del *.0 *.gch *.txt

run: all
	Source