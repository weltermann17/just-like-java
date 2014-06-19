// ArrayListIteratorI.h

//********************************************************************

#ifndef ArrayListIteratorI_h
#define ArrayListIteratorI_h

//********************************************************************

#include <jlj/util/ListIterator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > class ArrayList;

//********************************************************************

template< typename T >
class ArrayListIteratorI 
	: public ListIteratorI<T>
{
protected:
	~ArrayListIteratorI();
	friend class TypedReference<ArrayListIteratorI<T> >;

public:
	ArrayListIteratorI(T*, bool tail, ArrayList<T>);

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
	T* array;
	int current;
	ArrayList<T> list;
	bool canAdd;
	bool canRemove;
	bool canSet;
	bool afterNext;

};

//********************************************************************
//********************************************************************

template< typename T >
struct ArrayListIterator : StrongReference<ArrayListIteratorI<T> >
{
	StrongReferenceTemplateDerivateImplementation(ArrayListIterator, T);
};

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/ArrayListIterator.i.h>

//********************************************************************

#endif

// eof
