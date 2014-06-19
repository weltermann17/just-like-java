// AbstractMap.h

//********************************************************************

#ifndef jlj_util_AbstractMap_h
#define jlj_util_AbstractMap_h

//********************************************************************

#include <jlj/util/Map.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
class AbstractMapI 
	: public virtual MapI<K, V>
{
public:
	~AbstractMapI();
	AbstractMapI();

public:	
	void clear();
	bool containsKey(const K&) const;
	bool containsValue(const V&) const;
	bool equals(const Object&) const;
	bool isEmpty() const;
	void putAll(const Map<K, V>&);
	V remove(const K&) throw (IndexOutOfBoundsException);
	int size() const;

};

//********************************************************************
//********************************************************************

template< typename K, typename V > 
AbstractMapI<K, V>::~AbstractMapI() 
{}

//********************************************************************

template< typename K, typename V > 
AbstractMapI<K, V>::AbstractMapI() 
{}

//********************************************************************

template< typename K, typename V > 
void AbstractMapI<K, V>::clear() 
{
	this->entrySet()->clear();
}

//********************************************************************

template< typename K, typename V > 
bool AbstractMapI<K, V>::containsKey(const K& key) const
{
	return this->keySet()->contains(key);
}

//********************************************************************

template< typename K, typename V > 
bool AbstractMapI<K, V>::containsValue(const V& value) const 
{
	return this->values()->contains(value);
}

//********************************************************************

template< typename K, typename V > 
bool AbstractMapI<K, V>::equals(const Object& object) const
{
	Map<K, V> m;
	if (object->instanceOf(m))
	{
		return this->entrySet()->equals(m->entrySet());
	}
	else
		return false;
}

//********************************************************************

template< typename K, typename V > 
bool AbstractMapI<K, V>::isEmpty() const 
{
	return 0 == this->size();
}

//********************************************************************

template< typename K, typename V > 
void AbstractMapI<K, V>::putAll(const Map<K, V>& c) 
{
	Iterator< MapEntry<K, V> > i = c->entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<K, V>& current = i->next();
		put(current.key, current.value);
	}
}

//********************************************************************

template< typename K, typename V > 
V AbstractMapI<K, V>::remove(const K& key) throw (IndexOutOfBoundsException) 
{
	if (this->entrySet()->contains(MapEntry<K, V>(key)))
	{
		Iterator< MapEntry<K, V> > i = this->entrySet()->iterator();
		while (i->hasNext())
		{
			MapEntry<K, V>& current = i->next();
			if (key == current.key)
			{
				V value = current.value;
				i->remove();
				return value;
			}
		}
		throw IndexOutOfBoundsException();
	}
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename K, typename V > 
int AbstractMapI<K, V>::size() const 
{
	return this->entrySet()->size();
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
