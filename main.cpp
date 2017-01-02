#include <iostream>
#include "AVL_Tree.h"
#include "AVL_Tree.cpp"
#include "TreeTest.cpp"
#include <stdlib.h>
#include <time.h>

const unsigned int	MIN			= 100;
const unsigned int	MAX			= 10100;
const unsigned int	ITERATION	= 1000;

#ifdef DEBUG
#include <math.h>
#endif

using namespace std;
using namespace myAVL;

int main(int argc, char *argv[])
{
	DataStrTest<AVL_Tree<unsigned int, unsigned int>, AVL_NODE<unsigned int, unsigned int> >	Test;

	cout << "Start testing sequential generation (in order)." << endl;
	Test.GenerateAndRelease(SEQ_NORMAL, MIN, MAX, ITERATION);

	cout << "Start testing sequential generation (reversed order)." << endl;
	Test.GenerateAndRelease(SEQ_REVERSE, MIN, MAX, ITERATION);

	cout << "Start testing random generation." << endl;
	Test.GenerateAndRelease(RANDOM, MIN, MAX, ITERATION);

	return 0;
}
