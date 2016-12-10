# implementation of AVL Tree
#
OBJS = main.o 
CXX = g++
CFLAGS = -std=c++11 -Wall -O2
DEBUGFLAGS = -std=c++11 -Wall -O0 -g
OUTPUT = AVL_Tree.out

.PHONY: all
all: main

main: $(OBJS)
	cat AVL_Tree.h | sed 's/\#define DEBUG/\/\/#define DEBUG/g' > AVL_Tree.h
	$(CXX) -o $(OUTPUT) $(OBJS) $(CFLAGS)

debug: $(OBJS)
	cat AVL_Tree.h | sed 's/\/\/\#define DEBUG/\#define DEBUG/g' > AVL_Tree.h
	$(CXX) -o $(OUTPUT) $(OBJS) $(DEBUGFLAGS)

.SUFFIXES: .o.cpp
.cpp.o:
	g++ $(CFLAGS) -c $<

main.o: AVL_Tree.h
main.o: AVL_Tree.cpp

.PHONY: clean
clean:
	rm -f $(OBJS)
