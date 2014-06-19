// Set.h

//********************************************************************

#ifndef Set_h
#define Set_h

//********************************************************************

#include <jlj/util/Collection.h>
#include <jlj/util/AbstractCollection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class SetI 
	: public virtual CollectionI<T>
{
public:
	virtual ~SetI() = 0;
	SetI();

public:
	/**
	*
	* nothing added except the "stipulation" that
	* a Set is a Collection with no duplicates
	*
	*/

};

//********************************************************************

template< typename T >
struct Set : StrongReference<SetI<T> >
{
	StrongReferenceTemplateDerivateImplementation(Set, T);
};

//********************************************************************
//********************************************************************

template< typename T > 
SetI<T>::~SetI() 
{}

//********************************************************************

template< typename T > 
SetI<T>::SetI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
