// Iterator.h

//********************************************************************

#ifndef jlj_util_Iterator_h
#define jlj_util_Iterator_h

//********************************************************************

#include <jlj/lang/Object.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class IteratorI
	: public ObjectI
{
public:
	~IteratorI() = 0; 
	IteratorI();

public:
	virtual bool hasNext() = 0;
	virtual T& next() = 0;
	virtual void remove() = 0;

};

//********************************************************************

template< typename T >
struct Iterator : StrongReference<IteratorI<T> > 
{ 
	StrongReferenceTemplateDerivateImplementation(Iterator, T);
};

//********************************************************************

template< typename T > 
IteratorI<T>::~IteratorI() 
{}

//********************************************************************

template< typename T > 
IteratorI<T>::IteratorI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
