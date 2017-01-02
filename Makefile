# implementation of AVL Tree
#
OBJS = main.o
CXX = g++
CFLAGS = -std=c++11 -std=gnu++11 -Wall -O2
DEBUGFLAGS = -g
OUTPUT = AVL_Tree.out

.PHONY: all
all: main

main: $(OBJS)
	rm -rf debug.flag
	cat AVL_Tree.h | sed 's/^\#define DEBUG/\/\/#define DEBUG/g' > tmp.h
	mv tmp.h AVL_Tree.h
	$(CXX) $(CFLAGS) -o $(OUTPUT) $(OBJS)

.PHONY: debug
debug: debug.flag $(OBJS)
	$(CXX) $(CFLAGS) -o $(OUTPUT) $(OBJS) $(DEBUGFLAGS)

debug.flag:
	cat AVL_Tree.h | sed 's/^\/\/\#define DEBUG/\#define DEBUG/g' > tmp.h
	mv tmp.h AVL_Tree.h
	CFLAGS+=" $(DEBUGFLAGS)"
	touch debug.flag

.SUFFIXES: .o.cpp
.cpp.o:
	$(CXX) $(CFLAGS) -c $<

main.o: AVL_Tree.h
main.o: AVL_Tree.cpp
main.o: DataStructure.h
main.o: DataStructure.cpp

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f *.out
	rm -f debug.flag
