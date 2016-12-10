# implementation of AVL Tree
#
OBJS = main.o 
CXX = g++
CFLAGS = -std=c++11 -Wall -O2 -g
OUTPUT = AVL_Tree.out

.PHONY: all
all: main

main: $(OBJS)
	$(CXX) -o $(OUTPUT) $(OBJS)

.SUFFIXES: .o.cpp
.cpp.o:
	g++ $(CFLAGS) -c $<

main.o: AVL_Tree.h
main.o: AVL_Tree.cpp

.PHONY: clean
clean:
	rm -f $(OBJS)
