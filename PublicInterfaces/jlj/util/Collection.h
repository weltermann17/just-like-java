// Collection.h

//********************************************************************

#ifndef jlj_util_Collection_h
#define jlj_util_Collection_h

#include <jlj/util/Iterator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > struct Collection;

//********************************************************************

template< typename T >
class CollectionI
	: public virtual ObjectI
{
public:
	~CollectionI() = 0;
	CollectionI();

public:
	virtual bool add(const T&) = 0;
	virtual bool addAll(const Collection<T>&) = 0;
	virtual void clear() = 0;
	virtual bool contains(const T&) const = 0;
	virtual bool containsAll(const Collection<T>&) const = 0;
	virtual bool isEmpty() const = 0;
	virtual Iterator<T> iterator() = 0;
	virtual Iterator<T> iterator() const = 0;
	virtual bool remove(const T&) = 0;
	virtual bool removeAll(const Collection<T>&) = 0;
	virtual bool retainAll(const Collection<T>&) = 0;
	virtual int size() const = 0;

};

//********************************************************************

template< typename T > 
struct Collection : StrongReference<CollectionI<T> >
{
	StrongReferenceTemplateDerivateImplementation(Collection, T);
};


//********************************************************************

template< typename T > 
CollectionI<T>::~CollectionI() 
{}

//********************************************************************

template< typename T > 
CollectionI<T>::CollectionI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
