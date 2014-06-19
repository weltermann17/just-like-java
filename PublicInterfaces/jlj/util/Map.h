// Map.h

//********************************************************************

#ifndef Map_h
#define Map_h

//********************************************************************
#include <jlj/lang/Throwable.h>

#include <jlj/util/Set.h>
#include <jlj/util/MapEntry.h>


//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V > struct Map;

//********************************************************************

template< typename K, typename V >
class MapI 
	: public virtual ObjectI
{
public:
	~MapI() = 0;
	MapI();

public:
	virtual void clear() = 0;
	virtual bool containsKey(const K&) const = 0;
	virtual bool containsValue(const V&) const = 0;
	virtual Set< MapEntry<K, V> > entrySet() const = 0;
	virtual V get(const K&) const throw (IndexOutOfBoundsException) = 0;
	virtual bool isEmpty() const = 0;
	virtual Set<K> keySet() const = 0;
	virtual bool put(const K&, const V&) = 0;
	virtual void putAll(const Map<K, V>&) = 0;
	virtual V remove(const K&) throw (IndexOutOfBoundsException) = 0;			
	virtual int size() const = 0;
	virtual Collection<V> values() const = 0;

};

//********************************************************************

template< typename K, typename V >
struct Map : StrongReference<MapI<K, V> >
{    
	StrongReferenceTemplate2DerivateImplementation(Map, K, V);
};

//********************************************************************

template< typename K, typename V > 
MapI<K, V>::~MapI() 
{}

//********************************************************************

template< typename K, typename V > 
MapI<K, V>::MapI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
