# implementation of AVL Tree
#
OBJS = main.o AVL_Tree.o
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

AVL_Tree.o: AVL_Tree.h

.PHONY: clean
clean:
	rm -f $(OBJS)
