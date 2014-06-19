// TreeSetIterator.h

//********************************************************************

#ifndef TreeSetIterator_h
#define TreeSetIterator_h

//********************************************************************

#include <jlj/util/Iterator.h>
#include <jlj/util/Set.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > struct TreeSetBase;

//********************************************************************

template< typename T >
struct TreeNode 
{
	inline ~TreeNode();
	inline TreeNode(const T&, TreeNode<T>*);
	inline TreeNode(const T&, TreeNode<T>*, int);

	inline TreeNode<T>* next();
	inline TreeNode<T>* prev();

	TreeNode<T>* left; 
	TreeNode<T>* right;
	TreeNode<T>* up;
	int balance;
	T data;
};

//********************************************************************

template< typename T >
class TreeSetIteratorI 
	: public IteratorI<T>
{
public:
	~TreeSetIteratorI();
	TreeSetIteratorI(const TreeSetBase<T>&);

public:	
	bool hasNext();
	T& next();
	void remove();

protected:
	TreeNode<T>* current;
	TreeSetBase<T> set;
	bool canRemove;

};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/TreeSetIterator.i.h>

//********************************************************************

#endif

// eof
