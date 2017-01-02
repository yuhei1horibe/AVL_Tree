/**
* implementation of AVL Tree
*
* @author Yuhei
* 2016/10/23
*
*/

#ifndef _AVL_TREE_H_

#define	_AVL_TREE_H_

#include <memory>
#include <iostream>

#define DEBUG

namespace myAVL{

//Forward declaration
template <class KEY, class VAL>
class AVL_Tree;

template <class KEY, class VAL>
class AVL_NODE;



//Definition of AVL Tree node
template <class KEY, class VAL>
class AVL_NODE{
friend class AVL_Tree <KEY, VAL>;

private:
	//Key and Value of this node
	KEY					m_key;
	VAL					m_value;

	//Height of this node
	unsigned char		m_uHeight;

	AVL_NODE<KEY, VAL>*	m_lpLeft;
	AVL_NODE<KEY, VAL>*	m_lpRight;

	//Internal inorder list
	AVL_NODE<KEY, VAL>*	m_lpNext;
	AVL_NODE<KEY, VAL>*	m_lpPrev;

public:
	//Constructor / Destructor
	AVL_NODE(KEY key, VAL value);
	~AVL_NODE();

	//Accessors
	VAL		get_val();
	bool	update_val(VAL value);

#ifdef DEBUG
	unsigned int	get_height();
#endif
};

//Definition of AVL Tree Algorithm
template <class KEY, class VAL>
class AVL_Tree{
private:
	//Tree
	AVL_NODE<KEY, VAL>*	m_lpTreeTop;

	//Connect every node in order
	AVL_NODE<KEY, VAL>*	m_lpListHead;
	AVL_NODE<KEY, VAL>*	m_lpCurrentPt;

	//Member
	unsigned long		m_ulCount;		//Number of nodes

	//Release Tree Nodes
	void ReleasePartialTree(AVL_NODE<KEY, VAL>* lpNode);

	//Release All Tree Nodes
	void ReleaseAllTree();

	//Algorithm
	AVL_NODE<KEY, VAL>*	balance(AVL_NODE<KEY, VAL>* lpNode);								//balancing
	AVL_NODE<KEY, VAL>*	RRotate(AVL_NODE<KEY, VAL>* lpTop, AVL_NODE<KEY, VAL>* lpLeft);		//Rotation
	AVL_NODE<KEY, VAL>*	LRotate(AVL_NODE<KEY, VAL>* lpTop, AVL_NODE<KEY, VAL>* lpRight);
	AVL_NODE<KEY, VAL>*	remove_min(AVL_NODE<KEY, VAL>* lpNode);								//find and remove the minimum key from partial tree
	AVL_NODE<KEY, VAL>*	remove_internal(AVL_NODE<KEY, VAL>* lpNode, KEY key);				//find and remove the node has the same key.
	AVL_NODE<KEY, VAL>*	find_min(AVL_NODE<KEY, VAL>* lpNode);								//find the node has minimum key
	AVL_NODE<KEY, VAL>*	insert_internal(AVL_NODE<KEY, VAL>* lpNode, KEY key, VAL value);	//insert new node

	//Member methods
	//Calculation of height and Balance Factor
	unsigned char		FixHeight(AVL_NODE<KEY, VAL>* lpNode);
	signed char			CalcBFactor(AVL_NODE<KEY, VAL>* lpNode);

public:
	//Constructor and Destructor
	AVL_Tree();
	~AVL_Tree();

	//Accesors
	AVL_NODE<KEY, VAL>*	find(KEY key);
	bool				insert(KEY key, VAL val);
	void				remove(KEY key);
	void				release();
	unsigned long		size();

#ifdef DEBUG
	unsigned int		get_height();
#endif

	//Accessors for internal list
	//Internaly, the list is looped,
	//but it doesn't behave as looped list
	AVL_NODE<KEY, VAL>&	rewind_to_head();
	AVL_NODE<KEY, VAL>&	rewind_to_tail();
	AVL_NODE<KEY, VAL>&	next();
	AVL_NODE<KEY, VAL>&	prev();
};

}

#endif
