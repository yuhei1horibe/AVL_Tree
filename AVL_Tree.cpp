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
AVL_NODE<KEY, VAL>::AVL_NODE(KEY key, VAL value)
{
	//Calling constructor of key and value
	m_key		= key;
	m_value		= value;

	//Initialization of pointer
	//After start using smart pointers,
	//I shouldn't initialize pointers.
	m_lpLeft	= nullptr;
	m_lpRight	= nullptr;

	//Internal inorder list
	m_lpNext	= nullptr;
	m_lpPrev	= nullptr;

	m_uHeight	= 0;
}

//Accessor
template <class KEY, class VAL>
VAL AVL_NODE<KEY, VAL>::get_val()
{
	return m_value;
}

//Return height
#ifdef DEBUG
template <class KEY, class VAL>
unsigned int AVL_NODE<KEY, VAL>::get_height()
{
	return m_uHeight;
}
#endif

//Destractor of AVL_NODE
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>::~AVL_NODE()
{
	//It must be deleted by AVL_TREE Algorithm class
	//It musn't be executed this blacket
	if(m_lpLeft != nullptr){
		m_lpLeft	= nullptr;
	}

	if(m_lpRight != nullptr){
		m_lpRight	= nullptr;
	}

	m_lpNext	= nullptr;
	m_lpPrev	= nullptr;
}

//*************************************************
//* AVL Tree algorithm
//*************************************************

//Constructor
template <class KEY, class VAL>
AVL_Tree <KEY, VAL>::AVL_Tree()
{
	m_ulCount		= 0;
	m_lpTreeTop		= nullptr;
	m_lpListHead	= nullptr;
	m_lpCurrentPt	= nullptr;
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

	if(lpNode == nullptr)
		return 0;

	hl	= lpNode->m_lpLeft == nullptr	? 0 : lpNode->m_lpLeft->m_uHeight;
	hr	= lpNode->m_lpRight == nullptr	? 0 : lpNode->m_lpRight->m_uHeight;

	return lpNode->m_uHeight = (hl > hr ? hl : hr) + 1;
}

//Calculation of balance factor
template <class KEY, class VAL>
signed char AVL_Tree<KEY, VAL>::CalcBFactor(AVL_NODE<KEY, VAL> *lpNode)
{
	unsigned char	hl;		//Height of left node
	unsigned char	hr;		//Height of right node

	if(lpNode == nullptr)
		return 0;

	hl	= FixHeight(lpNode->m_lpLeft);
	hr	= FixHeight(lpNode->m_lpRight);

	return (signed char)hl - (signed char)hr;
}

//Left directional rotation
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::LRotate(AVL_NODE<KEY, VAL>* lpTop, AVL_NODE<KEY, VAL>* lpRight)
{
	AVL_NODE<KEY, VAL>*	p;
	AVL_NODE<KEY, VAL>*	q;

	p	= lpRight->m_lpLeft;
	q	= lpTop;

	lpRight->m_lpLeft			= q;
	lpTop						= lpRight;
	lpTop->m_lpLeft->m_lpRight	= p;

	return lpTop;
}

//Right directional rotation
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::RRotate(AVL_NODE<KEY, VAL>* lpTop, AVL_NODE<KEY, VAL>* lpLeft)
{
	AVL_NODE<KEY, VAL>*	p;
	AVL_NODE<KEY, VAL>*	q;

	p = lpLeft->m_lpRight;
	q = lpTop;

	lpLeft->m_lpRight			= q;
	lpTop						= lpLeft;
	lpTop->m_lpRight->m_lpLeft	= p;

	return lpTop;
}

//Balancing
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::balance(AVL_NODE<KEY, VAL>* lpNode)
{
	signed char	cBFactor;

	if(lpNode == nullptr)
		return nullptr;

	//Get balance factor
	cBFactor	= CalcBFactor(lpNode);

	//Left node is heavier
	if(cBFactor == 2){
		//The case requires double rotation
		if(CalcBFactor(lpNode->m_lpLeft) < 0){
			lpNode->m_lpLeft	= LRotate(lpNode->m_lpLeft, lpNode->m_lpLeft->m_lpRight);
		}
		lpNode	= RRotate(lpNode, lpNode->m_lpLeft);
	}

	//Right node is heavier
	else if(cBFactor == -2){
		//The case requires double rotation
		if(CalcBFactor(lpNode->m_lpRight) > 0){
			lpNode->m_lpRight	= RRotate(lpNode->m_lpRight, lpNode->m_lpRight->m_lpLeft);
		}
		lpNode	= LRotate(lpNode, lpNode->m_lpRight);
	}
	//Recalculate height of both nodes
	FixHeight(lpNode->m_lpLeft);
	FixHeight(lpNode->m_lpRight);
	FixHeight(lpNode);

	return lpNode;
}

//Release all of tree nodes
template <class KEY, class VAL>
void AVL_Tree<KEY, VAL>::release()
{
	ReleasePartialTree(m_lpTreeTop);

	m_lpTreeTop		= nullptr;
	m_lpListHead	= nullptr;
	m_lpCurrentPt	= nullptr;

	m_ulCount		= 0;

	return ;
}

//Release partial tree
template <class KEY, class VAL>
void AVL_Tree<KEY, VAL>::ReleasePartialTree(AVL_NODE<KEY, VAL>* lpNode)
{
	if(lpNode == nullptr)
		return ;

	ReleasePartialTree(lpNode->m_lpLeft);
	ReleasePartialTree(lpNode->m_lpRight);

	delete lpNode;
	lpNode	= nullptr;

	return ;
}

//Accessors
//Insert
template <class KEY, class VAL>
bool AVL_Tree<KEY, VAL>::insert(KEY key, VAL value)
{
	AVL_NODE<KEY, VAL>*	tmp = nullptr;

	//In case of memory allocation failure or same key exist, return false
	if((tmp = insert_internal(m_lpTreeTop, key, value)) == nullptr){
		return false;
	}
	m_lpTreeTop	= tmp;

	//Update inorder list
	if(m_lpListHead == nullptr){
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
	AVL_NODE<KEY, VAL>*		lpNewNode	= nullptr;
	//AVL_NODE<KEY, VAL>*		lpTemp;

	//lpTemp	= nullptr;

	//If this node is nullptr, insert here
	if(lpNode == nullptr){
		//Set key and value
		//left and right pointers are initialized to "nullptr"by constructor
		try{
			lpNewNode	= new AVL_NODE<KEY, VAL>(key, value);
		}
		catch(std::bad_alloc &e){
			std::cerr << "Node allocation failure. " << e.what() << std::endl;
		}
		
		//If allocation failure, return nullptr
		if(lpNewNode == nullptr){
			return nullptr;
		}

		return lpNewNode;
	}

	//If the same key exist, return nullptr
	if(key == lpNode->m_key){
#ifdef DEBUG
		std::cerr << "The Key:" << key << " already exist." << std::endl;
#endif
		return nullptr;
	}

	else if(key < lpNode->m_key){
		//Memory allocation failure or the same key exist
		if((lpNewNode = insert_internal(lpNode->m_lpLeft, key, value)) == nullptr){
#ifdef DEBUG
			std::cerr << "Allocation to left node failure." << std::endl;
			std::cerr << "lpNode:" << lpNode << std::endl;
#endif
			return nullptr;
		}

		lpNode->m_lpLeft	= lpNewNode;
/*
		//Update the inorder loop list
		if(lpNewNode->m_lpNext == nullptr){
			if(m_lpListHead == lpNode)
				m_lpListHead	= lpNode->m_lpLeft;
			lpTemp						= lpNode->m_lpPrev;
			lpNode->m_lpPrev			= lpNode->m_lpLeft;
			lpNode->m_lpLeft->m_lpNext	= lpNode;
			lpNode->m_lpLeft->m_lpPrev	= lpTemp;
			lpTemp->m_lpNext			= lpNode->m_lpLeft;
		}
*/
		//Balance the tree
		lpNode	= balance(lpNode);
	}

	else{
		//Memory allocation failure or the same key exist
		if ((lpNewNode = insert_internal(lpNode->m_lpRight, key, value)) == nullptr){
#ifdef DEBUG
			std::cerr << "Allocation to right node failure." << std::endl;
			std::cerr << "lpNode:" << lpNode << std::endl;
#endif
			return nullptr;
		}

		lpNode->m_lpRight	= lpNewNode;
/*
		//Update the inorder list
		if(lpNewNode->m_lpPrev == nullptr){
			lpTemp						= lpNode->m_lpNext;
			lpNode->m_lpNext			= lpNode->m_lpRight;
			lpNode->m_lpRight->m_lpNext	= lpTemp;
			lpNode->m_lpRight->m_lpPrev	= lpNode;
			lpTemp->m_lpPrev			= lpNode->m_lpRight;
		}
*/
		//Balance the tree
		lpNode	= balance(lpNode);
	}
	return lpNode;
}

//find
template <class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::find(KEY key)
{
	AVL_NODE<KEY, VAL>*	wp;

	wp	= m_lpTreeTop;

	while(1){
		if(wp == nullptr){
			return nullptr;
		}

		else if(wp->m_key == key){
			return wp;
		}

		else if(wp->m_key > key){
			wp	= wp->m_lpLeft;
		}

		else{
			wp	= wp->m_lpRight;
		}
	}

	return nullptr;
}

//Remove node from the Tree
template<class KEY, class VAL>
void AVL_Tree<KEY, VAL>::remove(KEY key)
{
	m_lpTreeTop	= remove_internal(m_lpTreeTop, key);
}

//Remove node (internal use)
template<class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::remove_internal(AVL_NODE<KEY, VAL>* lpNode, KEY key)
{
	AVL_NODE<KEY, VAL>*	lpTmp;

	if(lpNode == nullptr){
		lpTmp	= nullptr;
	}

	//If this node is the target
	else if(key == lpNode->m_key){
		//Find the minimum node from right sub-tree.
		lpTmp	= find_min(lpNode->m_lpRight);

		//If it's nullptr, that means, this node is the leaf.
		//Delete this Node.
		if(lpTmp == nullptr){
			lpTmp	= lpNode->m_lpLeft;
		}

		else{
			//Remove the node has minimum value from right sub-tree.
			//Replace this node to the minimum node from right.
			lpTmp->m_lpRight	= remove_min(lpNode->m_lpRight);
			lpTmp->m_lpLeft		= lpNode->m_lpLeft;
		}
		delete lpNode;
		m_ulCount--;
	}

	//Find and delete
	else if(lpNode->m_key > key){
		//Replace left side node to new node.
		lpNode->m_lpLeft	= remove_internal(lpNode->m_lpLeft, key);
		lpTmp	= balance(lpNode);
	}

	else{
		//Replace Right side node to new node.
		lpNode->m_lpRight	= remove_internal(lpNode->m_lpRight, key);
		lpTmp	= balance(lpNode);
	}
	return lpTmp;
}

//Find the node has minimum value
template<class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::find_min(AVL_NODE<KEY, VAL>* lpNode)
{
	AVL_NODE<KEY, VAL>*	lpTmp;

	if(lpNode == nullptr)
		return nullptr;

	lpTmp	= lpNode;

	while(1){
		if(lpTmp->m_lpLeft == nullptr)
			break;

		lpTmp	= lpTmp->m_lpLeft;
	}
	return lpTmp;
}

//Remove the node has minimum value
template<class KEY, class VAL>
AVL_NODE<KEY, VAL>* AVL_Tree<KEY, VAL>::remove_min(AVL_NODE<KEY, VAL>* lpNode)
{
	
	AVL_NODE<KEY, VAL>*	lpTmp	= nullptr;

	//Only one special case
	if(lpNode == nullptr)
		lpTmp	= nullptr;

	//If this node has the minimum.
	else if(lpNode->m_lpLeft == nullptr){
		lpTmp	= lpNode->m_lpRight;
	}

	else{
		lpNode->m_lpLeft	= remove_min(lpNode->m_lpLeft);
		lpTmp				= balance(lpNode);
	}
	return lpTmp;
}

//size
template <class KEY, class VAL>
unsigned long AVL_Tree<KEY, VAL>::size()
{
	return m_ulCount;
}

//Debug mode only
//Return the height of the tree
#ifdef DEBUG
template <class KEY, class VAL>
unsigned int AVL_Tree<KEY, VAL>::get_height()
{
	if(m_lpTreeTop == nullptr)
		return 0;

	return m_lpTreeTop->m_uHeight;
}
#endif

