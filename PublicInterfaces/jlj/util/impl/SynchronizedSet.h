// SynchronizedSet.h

//********************************************************************

#ifndef jlj_util_impl_SynchronizedSet_h
#define jlj_util_impl_SynchronizedSet_h

//********************************************************************

#include <jlj/util/impl/SynchronizedSet.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename V >
class SynchronizedSetI 
	: public virtual SetI<V>
	, public SynchronizedCollectionI<V>
{
protected:
	~SynchronizedSetI();
	friend class TypedReference<SynchronizedSetI<V> >;

public:
	SynchronizedSetI(Set<V>);

};

//********************************************************************

template< typename V > 
struct SynchronizedSet : StrongReference<SynchronizedSetI<V> >
{
	StrongReferenceTemplateDerivateImplementation(SynchronizedSet, V);
};

//********************************************************************
//********************************************************************

template< typename V > 
SynchronizedSetI<V>::~SynchronizedSetI() 
{}

//********************************************************************

template< typename V > 
SynchronizedSetI<V>::SynchronizedSetI(Set<V> set)
: SynchronizedCollectionI<V>(set)
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
