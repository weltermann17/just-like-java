// Comparator.h

//********************************************************************

#ifndef jlj_util_Comparator_h
#define jlj_util_Comparator_h

//********************************************************************

#include <jlj/lang/Object.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class ComparatorI
	: public ObjectI
{
public:
	~ComparatorI() = 0;
	ComparatorI();

public:
	virtual int compare(const T&, const T&) const = 0;

};

//********************************************************************

template< typename T >
struct Comparator : StrongReference<ComparatorI<T> > 
{
	StrongReferenceTemplateDerivateImplementation(Comparator, T);
};

//********************************************************************
//********************************************************************

template< typename T > 
ComparatorI<T>::~ComparatorI() 
{}

//********************************************************************

template< typename T > 
ComparatorI<T>::ComparatorI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
