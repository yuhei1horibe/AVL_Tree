# implementation of AVL Tree
#
OBJS = main.o 
DBOBJS = ./debug/main.o
CXX = g++
CFLAGS = -std=c++11 -Wall -O2
DEBUGFLAGS = -g
OUTPUT = AVL_Tree.out

.PHONY: all
all: main

.SUFFIXES: .o.cpp
%.cpp%.o:
	g++ $(CFLAGS) -c $<

main: $(OBJS)
	rm -rf debug.flag
	cat AVL_Tree.h | sed 's/^\#define DEBUG/\/\/#define DEBUG/g' > tmp.h
	mv tmp.h AVL_Tree.h
	$(CXX) -o $(OUTPUT) $(OBJS) $(CFLAGS)

.PHONY: debug
debug: debug.flag $(OBJS)
	$(CXX) -o $(OUTPUT) $(OBJS) $(CFLAGS)

debug.flag:
	cat AVL_Tree.h | sed 's/^\/\/\#define DEBUG/\#define DEBUG/g' > tmp.h
	mv tmp.h AVL_Tree.h
	CFLAGS+=" $(DEBUGFLAGS)"
	touch debug.flag

main.o: AVL_Tree.h
main.o: AVL_Tree.cpp

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f *.out
	rm -f debug.flag
