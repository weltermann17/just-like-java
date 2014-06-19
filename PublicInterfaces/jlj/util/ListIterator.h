// ListIterator.h

//********************************************************************

#ifndef jlj_util_ListIterator_h
#define jlj_util_ListIterator_h

//********************************************************************

#include <jlj/util/Iterator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class ListIteratorI 
	: public IteratorI<T>
{
protected:
	~ListIteratorI();
	friend class TypedReference<ListIteratorI<T> >;

public:
	ListIteratorI();

public:	
	virtual void add(const T&) = 0;
	virtual bool hasPrevious() = 0;
	virtual int nextIndex() = 0;
	virtual T& previous() = 0;
	virtual int previousIndex() = 0;
	virtual void set(const T&) = 0;

};

//********************************************************************

template< typename T >
struct ListIterator : StrongReference<ListIteratorI<T> >
{
	StrongReferenceTemplateDerivateImplementation(ListIterator, T);
};

//********************************************************************

template< typename T > 
ListIteratorI<T>::~ListIteratorI() 
{}

//********************************************************************

template< typename T > 
ListIteratorI<T>::ListIteratorI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
