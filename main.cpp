#include<iostream>
#include "AVL_Tree.h"
#include "AVL_Tree.cpp"
#include<stdlib.h>
#include<time.h>

using namespace std;
using namespace myAVL;

int main(int argc, char *argv[])
{
	unsigned int	i;
	unsigned int	key;
	AVL_NODE<unsigned int, unsigned int>*	tmp;

	//AVL Treeを作成
	AVL_Tree<unsigned int, unsigned int>	ATree;

	srand((unsigned int)time(NULL));

	for(i = 0; i < 100; i++){
		ATree.insert(i, (101 - i));
	}

	//Search test
	for(i = 0; i < 1000; i++){
		tmp	= ATree.find((unsigned int)(key = rand() % 100));
		cout << "Key:" << key << endl << "Value:" << tmp->get_val() << endl;
	}

	return 0;
}
