// Collections.h

//********************************************************************

#ifndef jlj_util_Collections_h
#define jlj_util_Collections_h

//********************************************************************

#include <jlj/util/Collection.h>
#include <jlj/util/Set.h>
#include <jlj/util/List.h>
#include <jlj/util/Map.h>
#include <jlj/util/impl/SynchronizedCollection.h>
#include <jlj/util/impl/SynchronizedList.h>
#include <jlj/util/impl/SynchronizedSet.h>
#include <jlj/util/impl/SynchronizedMap.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

struct Collections
{
	/**
	* from JAVADOC: It is imperative that the user manually synchronizes on the returned collection when iterating over it
	*
	* eg:
	* SynchronizedCollection<int> c = Collections::synchronizedCollection(new LinkedListI<int>());
	* ...
	* synchronized(c->getLock())
	* {
	*   Iterator<int> i = c->iterator();
	*   while ...
	* }
	*
	* If you iterator of the collection without the lock your program is bound to crash sometime!
	*
	*/

	template< typename V >
	static SynchronizedCollection<V> synchronizedCollection(Collection<V>);

	template< typename V >
	static SynchronizedList<V> synchronizedList(List<V>);
	
	template< typename K >
	static SynchronizedSet<K> synchronizedSet(Set<K>); 
	
	template< typename K, typename V >
	static SynchronizedMap<K, V> synchronizedMap(Map<K, V>);
};

//********************************************************************
//********************************************************************

template< typename V > 
SynchronizedCollection<V> Collections::synchronizedCollection(Collection<V> collection) 
{
	return new SynchronizedCollectionI<V>(collection);
}

//********************************************************************

template< typename V > 
SynchronizedList<V> Collections::synchronizedList(List<V> list) 
{
	return new SynchronizedListI<V>(list);
}

//********************************************************************

template< typename K > 
SynchronizedSet<K> Collections::synchronizedSet(Set<K> set) 
{
	return new SynchronizedSetI<K>(set);
}

//********************************************************************

template< typename K, typename V > 
SynchronizedMap<K, V> Collections::synchronizedMap(Map<K, V> map) 
{
	return new SynchronizedMapI<K, V>(map);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
