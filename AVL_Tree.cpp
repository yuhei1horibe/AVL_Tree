/**
*
* Implementation of AVL Tree
* @author Yuhei 
* 2016/10/23
*
*/


//implementation of AVL Tree
#include "AVL_Tree.h"
#include <cstddef>

using namespace myAVL;


//Constructor of AVL_NODE
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>::AVL_NODE()
{
	//Calling constructor of key and value
	m_key.KEY();
	m_value.VAL();

	//Initialization of pointer
	m_lpLeft	= NULL;
	m_lpRight	= NULL;

	//Internal inorder list
	m_lpNext	= NULL;
	m_lpPrev	= NULL;

	m_uHeight	= 0;
}

//Destractor of AVL_NODE
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>::~AVL_NODE()
{
	//It must be deleted by AVL_TREE Algorithm class
	//It musn't be executed this blacket
	if(m_lpLeft != NULL){
		delete	m_lpLeft;
		m_lpLeft	= NULL;
	}

	if(m_lpRight != NULL){
		delete	m_lpRight;
		m_lpRight	= NULL;
	}

	m_lpNext	= NULL;
	m_lpPrev	= NULL;
}

//*************************************************
//* AVL Tree algorithm
//*************************************************

//Constructor
template <class KEY, class VAL>
AVL_Tree <KEY, VAL>::AVL_Tree()
{
	m_ulCount		= 0;
	m_lpTreeTop		= NULL;
	m_lpListHead	= NULL;
	m_lpCurrentPt	= NULL;
}

//Destructor
template <class KEY, class VAL>
AVL_Tree<KEY, VAL>::~AVL_Tree()
{
	ReleaseAllTree();
}

//Release function
template <class KEY, class VAL>
void AVL_Tree<KEY, VAL>::ReleaseAllTree()
{
	ReleasePartialTree(m_lpTreeTop);
}

//Calculation of height
template <class KEY, class VAL>
unsigned char AVL_Tree<KEY, VAL>::FixHeight(AVL_NODE<KEY, VAL> *lpNode)
{
	unsigned char	hl;		//Height of left node
	unsigned char	hr;		//Height of right node

	if(lpNode == NULL)
		return 0;

	hl	= lpNode->m_lpLeft == NULL	? 0 : lpNode->m_lpLeft->m_lpHeight;
	hr	= lpNode->m_lpRight == NULL	? 0 : lpNode->m_lpRight->m_lpHeight;

	return lpNode->m_uHeight = (hl > hr ? hl : hr) + 1;
}

//Calculation of balance factor
template <class KEY, class VAL>
unsigned char AVL_Tree<KEY, VAL>::CalcBFactor(AVL_NODE<KEY, VAL> *lpNode)
{
	unsigned char	hl;		//Height of left node
	unsigned char	hr;		//Height of right node

	if(lpNode == NULL)
		return 0;

	hl	= FixHeight(lpNode->m_lpLeft);
	hr	= FixHeight(lpNode->m_lpRight);

	return hl - hr;
}

//Left directional rotation
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::LRotate(AVL_NODE<KEY, VAL>* lpTop, AVL_NODE<KEY, VAL>* lpRight)
{
	AVL_NODE<KEY, VAL>*	p;
	AVL_NODE<KEY, VAL>*	q;

	p	= lpRight->m_lpLeft;
	q	= lpTop;

	lpTop->m_lpRight	= p;
	lpRight->m_lpLeft	= q;

	return lpRight;
}

//Right directional rotation
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::RRotate(AVL_NODE<KEY, VAL>* lpTop, AVL_NODE<KEY, VAL>* lpLeft)
{
	AVL_NODE<KEY, VAL>*	p;
	AVL_NODE<KEY, VAL>*	q;

	p = lpLeft->m_lpRight;
	q = lpTop;

	lpTop->m_lpLeft = p;
	lpLeft->m_lpRight = q;

	return lpLeft;
}

//Balancing
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::balance(AVL_NODE<KEY, VAL>* lpNode)
{
	unsigned char	uBFactor;

	if(lpNode == NULL)
		return NULL;

	//Get balance factor
	uBFactor	= CalcBFactor(lpNode);

	//Left node is heavier
	if(uBFactor == 2){
		//The case requires double rotation
		if(CalcBFactor(lpNode->m_lpLeft) < 0){
			LRotate(lpNode->m_lpLeft, lpNode->m_lpLeft->m_lpRight);
		}
		RRotate(lpNode, lpNode->m_lpLeft);
	}

	//Right node is heavier
	else if(uBFactor == -2){
		//The case requires double rotation
		if(CalcBFactor(lpNode->m_lpRight) > 0){
			RRotate(lpNode->m_lpRight, lpNode->m_lpRight->m_lpLeft);
		}
		LRotate(lpNode, lpNode->lpRight);
	}
	//Recalculate height of both nodes
	FixHeight(lpNode->m_lpLeft);
	FixHeight(lpNode->m_lpRight);

	return lpNode;
}

//Release all of tree nodes
template <class KEY, class VAL>
void AVL_Tree<KEY, VAL>::release_all()
{
	ReleasePartialTree(m_lpTreeTop);

	m_lpTreeTop		= NULL;
	m_lpListHead	= NULL;
	m_lpCurrentPt	= NULL;

	m_ulCount		= 0;

	return ;
}

//Release partial tree
template <class KEY, class VAL>
void AVL_Tree<KEY, VAL>::ReleasePartialTree(AVL_NODE<KEY, VAL>* lpNode)
{
	if(lpNode == NULL)
		return ;

	ReleasePartialTree(lpNode->m_lpLeft);
	ReleasePartialTree(lpNode->m_lpRight);

	delete lpNode;
	lpNode	= NULL;

	return ;
}

//Accessors
//Insert
template <class KEY, class VAL>
bool AVL_Tree<KEY, VAL>::insert(KEY key, VAL value)
{
	//In case of memory allocation failure or same key exist, return false
	if(insert_internal(m_lpTreeTop, key, value) == NULL)
		return false;

	//Update inorder list
	if(m_lpListHead == NULL){
		m_lpListHead			= m_lpTreeTop;

		//Loop list
		m_lpListHead->m_lpNext	= m_lpTreeTop;
		m_lpListHead->m_lpPrev	= m_lpTreeTop;
	}

	//Update the size
	m_ulCount++;
	FixHeight(m_lpTreeTop);

	return true;
}

template <class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::insert_internal(AVL_NODE<KEY, VAL>* lpNode, KEY key, VAL value)
{
	AVL_NODE<KEY, VAL>*		lpNewNode;
	AVL_NODE<KEY, VAL>*		lpTemp;

	//If this node is NULL, insert here
	if(lpNode == NULL){
		//Set key and value
		//left and right pointers are initialized to "NULL"by constructor
		lpNewNode			= new AVL_NODE<KEY, VAL>;

		//If allocation failure, return NULL
		if(lpNewNode == NULL)
			return NULL;

		lpNewNode->m_key	= key;
		lpNewNode->m_value	= value;

		return lpNewNode;
	}

	//If the same key exist, return NULL
	if(key == lpNode->m_key){
		return NULL;
	}

	else if(key < lpNode->m_key){
		//Memory allocation failure or the same key exist
		if((lpNewNode = insert_internal(lpNode->m_lpLeft)) == NULL)
			return NULL;

		lpNode->m_lpLeft	= lpNewNode;

		//Update the inorder loop list
		if(lpNewNode->m_lpNext == NULL){
			if(m_lpListHead == lpNode)
				m_lpListHead	= lpNode->m_lpLeft;

			lpTemp						= lpNode->m_lpPrev;
			lpNode->m_lpPrev			= lpNode->m_lpLeft;
			lpNode->m_lpLeft->m_lpNext	= lpNode;
			lpNode->m_lpLeft->m_lpPrev	= lpTemp;
			lpTemp->m_lpNext			= lpNode->m_lpLeft;
		}

		//Balance the tree
		balance(lpNode);
	}

	else{
		//Memory allocation failure or the same key exist
		if ((lpNewNode = insert_internal(lpNode->m_lpRight)) == NULL)
			return NULL;

		lpNode->m_lpRight	= lpNewNode;

		//Update the inorder list
		if(lpNewNode->m_lpPrev == NULL){
			lpTemp						= lpNode->m_lpNext;
			lpNode->m_lpNext			= lpNode->m_lpRight;
			lpNode->m_lpRight->m_lpNext	= lpTemp;
			lpNode->m_lpRight->m_lpPrev	= lpNode;
			lpTemp->m_lpPrev			= lpNode->m_lpRight;
		}

		//Balance the tree
		balance(lpNode);
	}
	return lpTemp;
}

//size
template <class KEY, class VAL>
unsigned long AVL_Tree<KEY, VAL>::size()
{
	return m_ulCount;
}
