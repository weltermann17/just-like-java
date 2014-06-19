// SynchronizedMapI.h

//********************************************************************

#ifndef SynchronizedMap_h
#define SynchronizedMap_h

//********************************************************************

#include <jlj/util/impl/SynchronizedSet.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
class SynchronizedMapI 
	: public virtual MapI<K, V>
{
protected:
	~SynchronizedMapI();
	friend class TypedReference<SynchronizedMapI<K, V> >;

public:
	SynchronizedMapI(Map<K, V>);
	bool equals(const Object&) const;

public:	
	void clear();
	bool containsKey(const K&) const;
	bool containsValue(const V&) const;
	V get(const K&) const throw (IndexOutOfBoundsException);
	bool isEmpty() const;
	bool put(const K&, const V&);
	void putAll(const Map<K, V>&);
	V remove(const K&) throw (IndexOutOfBoundsException);
	int size() const;

	Set< MapEntry<K, V> > entrySet() const;
	Set<K> keySet() const;
	Collection<V> values() const;

	Lock getLock() const; // use this to begin a "synchronized" block 


private:
	Map<K, V> map;
	mutable Lock lock; 

};

//********************************************************************

template< typename K, typename V >
struct SynchronizedMap : StrongReference<SynchronizedMapI<K, V> >
{
	StrongReferenceTemplate2DerivateImplementation(SynchronizedMap, K, V);
};

//********************************************************************
//********************************************************************

template< typename K, typename V > 
SynchronizedMapI<K, V>::~SynchronizedMapI() 
{
	synchronized(lock);
}

//********************************************************************

template< typename K, typename V > 
SynchronizedMapI<K, V>::SynchronizedMapI(Map<K, V> map)
: map(map)
, lock(new ReentrantLockI()) 
{}

//********************************************************************

template< typename K, typename V > 
void SynchronizedMapI<K, V>::clear() 
{
	synchronized(lock) map->clear();
}

//********************************************************************

template< typename K, typename V > 
bool SynchronizedMapI<K, V>::containsKey(const K& key) const
{
	bool ok;
	synchronized(lock) ok = map->containsKey(key);
	return ok;
}

//********************************************************************

template< typename K, typename V > 
bool SynchronizedMapI<K, V>::containsValue(const V& value) const 
{
	bool ok;
	synchronized(lock) ok = map->containsValue(value);
	return ok;
}

//********************************************************************

template< typename K, typename V > 
bool SynchronizedMapI<K, V>::equals(const Object& object) const
{
	bool ok = false;
	Map<K, V> b;
	synchronized(lock)
	{
		if (object->instanceOf(b))
		{
			ok = map->equals(b);
		}
	}
	return ok;
}

//********************************************************************

template< typename K, typename V > 
V SynchronizedMapI<K, V>::get(const K& key) const throw (IndexOutOfBoundsException) 
{
	V v;
	synchronized(lock) v = map->get(key);
	return v;
}

//********************************************************************

template< typename K, typename V > 
bool SynchronizedMapI<K, V>::isEmpty() const 
{
	bool ok;
	synchronized(lock) ok = map->isEmpty();
	return ok;
}

//********************************************************************

template< typename K, typename V > 
bool SynchronizedMapI<K, V>::put(const K& key, const V& value) 
{
	bool ok;
	synchronized(lock) ok = map->put(key, value);
	return ok;
}

//********************************************************************

template< typename K, typename V > 
void SynchronizedMapI<K, V>::putAll(const Map<K, V>& other) 
{
	synchronized(lock) map->putAll(other);
}

//********************************************************************

template< typename K, typename V > 
V SynchronizedMapI<K, V>::remove(const K& key) throw (IndexOutOfBoundsException)  
{
	V v;
	synchronized(lock) v = map->remove(key);
	return v;
}

//********************************************************************

template< typename K, typename V > 
int SynchronizedMapI<K, V>::size() const 
{
	int s;
	synchronized(lock) s = map->size();
	return s;
}

//********************************************************************

template< typename K, typename V > 
Set< MapEntry<K, V> > SynchronizedMapI<K, V>::entrySet() const
{
	Set< MapEntry<K, V> > entries;
	synchronized(lock) entries = Collections::synchronizedSet(map->entrySet());
	return entries;
}

//********************************************************************

template< typename K, typename V > 
Set<K> SynchronizedMapI<K, V>::keySet() const
{
	Set<K> keys;
	synchronized(lock) keys = Collections::synchronizedSet(map->keySet());
	return keys;
}

//********************************************************************

template< typename K, typename V > 
Collection<V> SynchronizedMapI<K, V>::values() const
{
	Collection<V> values;
	synchronized(lock) values = Collections::synchronizedCollection(map->values());
	return values;
}

//********************************************************************

template< typename K, typename V > 
Lock  SynchronizedMapI<K, V>::getLock() const
{
	return lock;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
