// LinkedHashSetI.h

//********************************************************************

#ifndef LinkedHashSet_h
#define LinkedHashSet_h

//********************************************************************

#include <jlj/util/HashSet.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class LinkedHashSetI 
	: public HashSetI<T>
{
protected:
	~LinkedHashSetI();
	friend class TypedReference<LinkedHashSetI<T> >;

public:
	LinkedHashSetI();	

	LinkedHashSetI(const Collection<T>&);
	LinkedHashSetI(int initialcapacity, double loadfactor);
	LinkedHashSetI(int initialcapacity);

public:

};

//********************************************************************

template< typename T >
struct LinkedHashSet : StrongReference<HashSetI<T> > 
{
	StrongReferenceTemplateDerivateImplementation(LinkedHashSet, T);
};

//********************************************************************
//********************************************************************

template< typename T > 
LinkedHashSetI<T>::~LinkedHashSetI() 
{}

//********************************************************************

template< typename T > 
LinkedHashSetI<T>::LinkedHashSetI() 
: HashSetI<T>(16, .75, true)
{}

//********************************************************************

template< typename T > 
LinkedHashSetI<T>::LinkedHashSetI(const Collection<T>& c) 
: HashSetI<T>((c->size() / .75 + 1) > 16 ? (c->size() / .75 + 1) : 16, .75, true)
{
	addAll(c);
}

//********************************************************************

template< typename T > 
LinkedHashSetI<T>::LinkedHashSetI(int initialcapacity,
								  double loadfactor) 
								  : HashSetI<T>(initialcapacity, loadfactor, true)
{}

//********************************************************************

template< typename T > 
LinkedHashSetI<T>::LinkedHashSetI(int initialcapacity) 
: HashSetI<T>(initialcapacity, .75, true)
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
