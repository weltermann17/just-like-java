// LinkedListIterator.h

//********************************************************************

#ifndef jlj_util_LinkedListIterator_h
#define jlj_util_LinkedListIterator_h

//********************************************************************

#include <jlj/util/ListIterator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > class LinkedList;

//********************************************************************

template< typename T >
struct LinkedListNode 
{
	inline ~LinkedListNode();
	inline LinkedListNode(const T&, LinkedListNode<T>*, LinkedListNode<T>*);

	LinkedListNode<T>* next; 
	LinkedListNode<T>* prev;

	T data;
};

//********************************************************************

template< typename T >
class LinkedListIteratorI 
	: public ListIteratorI<T>
{
protected:
	~LinkedListIteratorI();
	friend class TypedReference<LinkedListIteratorI<T> >;

public:
	LinkedListIteratorI(LinkedListNode<T>*, LinkedList<T>);

public:	
	void add(const T&);
	bool hasNext();
	bool hasPrevious();
	T& next();
	int nextIndex(); 
	T& previous();
	int previousIndex();  
	void remove();
	void set(const T&);

private:
	LinkedListNode<T>* current;
	LinkedList<T> list;
	int index;
	bool canAdd;
	bool canRemove;
	bool canSet;
	bool afterNext;

};

//********************************************************************
//********************************************************************

template< typename T >
struct LinkedListIterator : StrongReference<LinkedListIteratorI<T> >
{
	StrongReferenceTemplateDerivateImplementation(LinkedListIterator, T);
};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/LinkedListIterator.i.h>

//********************************************************************

#endif

// eof
