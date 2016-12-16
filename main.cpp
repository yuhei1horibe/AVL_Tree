#include<iostream>
#include "AVL_Tree.h"
#include "AVL_Tree.cpp"
#include<stdlib.h>
#include<time.h>

#ifdef DEBUG
#include<math.h>
#endif

using namespace std;
using namespace myAVL;

int main(int argc, char *argv[])
{
	unsigned int	i;
	unsigned int	key;

#ifdef DEBUG
	unsigned int	average	= 0;
	unsigned int	maximum	= 0;
#endif

	AVL_NODE<unsigned int, unsigned int>*	tmp;

	//AVL Treeを作成
	AVL_Tree<unsigned int, unsigned int>	ATree;

	srand((unsigned int)time(NULL));

	for(i = 0; i < 100; i++){
		//ATree.insert(i, (101 - i));
		ATree.insert((99 - i), i);
	}

#ifdef DEBUG
	cout << "Size of the tree:" << ATree.size() << endl;
#endif

	//Search test
	for(i = 0; i < 1000; i++){
		tmp	= ATree.find((unsigned int)(key = rand() % 100));
#ifdef DEBUG
		if(tmp == NULL){
			cout << "key:" << key << " doesn't exist." << endl;
			//ATree.ReleaseAllTree();
			return -1;
		}
#endif
		cout << "Key:" << key << endl << "Value:" << tmp->get_val() << endl;

#ifdef DEBUG
		//Calculate Average Height
		cout << "Height:" << tmp->get_height() << endl;
		average	+= tmp->get_height();
		if(maximum < tmp->get_height())
			maximum	= tmp->get_height();
#endif
		cout << endl;
	}
#ifdef DEBUG
	cout << "Average Height:" << (average / (double)1000.0) << endl;
	cout << "Maximum Height:" << maximum << endl;
	cout << "Expected Height:" << (log(100.0) / log(2.0)) << endl;
#endif
	return 0;
}
