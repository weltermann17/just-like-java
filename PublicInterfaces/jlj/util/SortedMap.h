// SortedMap.h

//********************************************************************

#ifndef SortedMap_h
#define SortedMap_h

//********************************************************************

#include <jlj/util/Map.h>
#include <jlj/util/SortedSet.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V > struct SortedMap; 

//********************************************************************

template< typename K, typename V >
class SortedMapI 
	: public virtual MapI<K, V>
{
public:
	virtual ~SortedMapI() = 0;
	SortedMapI();

public:
	virtual const K& firstKey() const = 0;
	virtual SortedMap<K, V> headMap(const K& to) throw (IndexOutOfBoundsException) = 0;
	virtual const K& lastKey() const = 0;
	virtual SortedMap<K, V> subMap(const K& from, const K& to) throw (IndexOutOfBoundsException) = 0;
	virtual SortedMap<K, V> tailMap(const K& from) throw (IndexOutOfBoundsException) = 0;
	virtual Comparator<K> comparator() const = 0;

};

//********************************************************************

template< typename K, typename V >
struct SortedMap : StrongReference<SortedMapI<K, V> > 
{
	StrongReferenceTemplate2DerivateImplementation(SortedMap, K, V);
	operator Map<K, V>() { return get(); }
	operator Map<K, V>() const { return get(); }

};

//********************************************************************

template< typename K, typename V > 
SortedMapI<K, V>::~SortedMapI() 
{}

//********************************************************************

template< typename K, typename V > 
SortedMapI<K, V>::SortedMapI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
