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

	for(i = 0; ATree.size() < 100; i++){
		//ATree.insert(i, (101 - i));
		//ATree.insert((99 - i), i);
		key	= rand() % 100;
		ATree.insert(key, (101 - key));
#ifdef DEBUG
		cout << "Generated key: " << key << endl;
		cout << "Size: " << ATree.size() << endl;
#endif
	}

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

	//Remove function test.
	for(i = 0; i < 100; i++){
		ATree.remove((50 + (99 - i)) % 100);
		maximum	= 0;

		for(int j = 0; j < (99 - i); j++){
			tmp	= ATree.find((50 + j) % 100);

			//Error. Node didn't find.
			if(tmp == NULL){
				cout << "Error. Node " << (50 + j) % 100 << "doesn't exits." << endl;
				return -1;
			}
			if(maximum < tmp->get_height()){
				maximum	= tmp->get_height();
			}
#ifdef DEBUG
			cerr << "    key :" << (50 + j) % 100 << " found." << endl;
#endif
		}
		if((tmp = ATree.find((50 + (99 - i)) % 100)) == NULL)
			cout << "Key: " << (50 + (99 - i)) % 100 << " successfully removed." << endl;
		else
			cout << "Key: " << (50 + (99 - i)) % 100 << " still exist" << endl;
		cout << "Max height: " << maximum << endl;
		cout << "Expected Height:" << (log((double)ATree.size()) / log(2.0)) << endl;
	}

	return 0;
}
