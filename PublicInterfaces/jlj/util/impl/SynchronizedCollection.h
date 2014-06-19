// SynchronizedCollection.h

//********************************************************************

#ifndef jlj_util_impl_SynchronizedCollection_h
#define jlj_util_impl_SynchronizedCollection_h

//********************************************************************

#include <jlj/util/Collections.h>
#include <jlj/util/concurrent/locks/ReentrantLock.h>
#include <jlj/util/concurrent/locks/synchronized.h>
using namespace jlj::util::concurrent::locks;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename V >
class SynchronizedCollectionI 
	: public virtual CollectionI<V>
{
protected:
	~SynchronizedCollectionI();
	friend class TypedReference<SynchronizedCollectionI<V> >;

public:
	SynchronizedCollectionI(Collection<V>);

public:	
	bool add(const V&);
	bool addAll(const Collection<V>&);
	void clear();
	bool contains(const V&) const;
	bool containsAll(const Collection<V>&) const;
	bool equals(const Object&) const;
	bool isEmpty() const;
	Iterator<V> iterator(); // call this only in a "synchronized" block
	Iterator<V> iterator() const;
	bool remove(const V&);
	bool removeAll(const Collection<V>&);
	bool retainAll(const Collection<V>&);
	int size() const;

	Lock getLock() const; // use this to begin a "synchronized" block 

protected:
	Collection<V> collection;
	mutable Lock lock; 

};

//********************************************************************

template< typename V > 
struct SynchronizedCollection : StrongReference<SynchronizedCollectionI<V> >
{
	StrongReferenceTemplateDerivateImplementation(SynchronizedCollection, V);
};

//********************************************************************
//********************************************************************

template< typename V > 
SynchronizedCollectionI<V>::~SynchronizedCollectionI() 
{
	synchronized(lock);
}

//********************************************************************

template< typename V > 
SynchronizedCollectionI<V>::SynchronizedCollectionI(Collection<V> collection)
: collection(collection)
, lock(new ReentrantLockI()) 
{}

//********************************************************************

template< typename V > 
void SynchronizedCollectionI<V>::clear() 
{
	synchronized(lock) collection->clear();
}

//********************************************************************

template< typename V > 
bool SynchronizedCollectionI<V>::add(const V& v) 
{
	bool ok;
	synchronized(lock) ok = collection->add(v);
	return ok;
}

//********************************************************************

template< typename V > 
bool SynchronizedCollectionI<V>::addAll(const Collection<V>& other)  
{
	bool ok;
	synchronized(lock) ok = collection->addAll(other);
	return ok;
}

//********************************************************************

template< typename V > 
bool SynchronizedCollectionI<V>::contains(const V& v) const
{
	bool ok;
	synchronized(lock) ok = collection->contains(v);
	return ok;
}

//********************************************************************

template< typename V > 
bool SynchronizedCollectionI<V>::containsAll(const Collection<V>& other) const 
{
	bool ok;
	synchronized(lock) ok = collection->containsAll(other);
	return ok;
}

//********************************************************************

template< typename V > 
bool SynchronizedCollectionI<V>::equals(const Object& object) const
{
	bool ok = false;
	Collection<V> b;
	synchronized(lock)
	{
		if (object->instanceOf(b))
		{
			ok = collection->equals(b);
		}
	}
	return ok;
}

//********************************************************************

template< typename V > 
bool SynchronizedCollectionI<V>::isEmpty() const 
{
	bool ok;
	synchronized(lock) ok = collection->isEmpty();
	return ok;
}

//********************************************************************

template< typename V > 
bool SynchronizedCollectionI<V>::remove(const V& v) 
{
	bool ok;
	synchronized(lock) ok = collection->remove(v);
	return ok;
}

//********************************************************************

template< typename V > 
bool SynchronizedCollectionI<V>::removeAll(const Collection<V>& other) 
{
	bool ok;
	synchronized(lock) ok = collection->removeAll(other);
	return ok;
}

//********************************************************************

template< typename V > 
bool SynchronizedCollectionI<V>::retainAll(const Collection<V>& other) 
{
	bool ok;
	synchronized(lock) ok = collection->retainAll(other);
	return ok;
}

//********************************************************************

template< typename V > 
int SynchronizedCollectionI<V>::size() const 
{
	int s;
	synchronized(lock) s = collection->size();
	return s;
}

//********************************************************************

template< typename V > 
Iterator<V> SynchronizedCollectionI<V>::iterator()
{
	Iterator<V> i;
	synchronized(lock) i = collection->iterator();
	return i;
}

//********************************************************************

template< typename V > 
Iterator<V> SynchronizedCollectionI<V>::iterator() const
{
	Iterator<V> i;
	synchronized(lock) i = collection->iterator();
	return i;
}

//********************************************************************

template< typename V > 
Lock SynchronizedCollectionI<V>::getLock() const
{
	return lock;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
