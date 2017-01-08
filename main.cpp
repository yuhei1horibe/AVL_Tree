#include <iostream>
#include "AVL_Tree.h"
#include "AVL_Tree.cpp"
#include "DataStructureTest.cpp"
#include <stdlib.h>
#include <time.h>

const unsigned int	MIN			= 100;
const unsigned int	MAX			= 10100;
const unsigned int	ITERATION	= 10;

#ifdef DEBUG
#include <math.h>
#endif

using namespace std;
using namespace myAVL;

int main(int argc, char *argv[])
{
	DataStrTest<AVL_Tree<unsigned int, unsigned int>, AVL_NODE<unsigned int, unsigned int> >	Test;

	cout << "Start testing sequential removal (in order)." << endl;
	//Test.GenerateAndRelease(SEQ_NORMAL, MIN, MAX, 1);
	Test.GenerateAndRemove(SEQ_NORMAL, MIN, MAX, 1);

	cout << "Start testing sequential removal (reversed order)." << endl;
	//Test.GenerateAndRelease(SEQ_REVERSE, MIN, MAX, 1);
	Test.GenerateAndRemove(SEQ_REVERSE, MIN, MAX, 1);

	cout << "Start testing random removal." << endl;
	//Test.GenerateAndRelease(RANDOM, MIN, MAX, ITERATION);
	Test.GenerateAndRemove(RANDOM, MIN, MAX, ITERATION);

	return 0;
}
