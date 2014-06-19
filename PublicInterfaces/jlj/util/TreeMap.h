// TreeMap.h

//********************************************************************

#ifndef jlj_util_TreeMap_h
#define jlj_util_TreeMap_h

//********************************************************************

#include <jlj/util/AbstractMap.h>
#include <jlj/util/SortedMap.h>
#include <jlj/util/TreeKeySet.h>
#include <jlj/util/ValueCollection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
class TreeMapI 
	: public AbstractMapI<K, V>
	, public SortedMapI<K, V>
{
protected:
	~TreeMapI();
	friend class TypedReference<TreeMapI<K, V> >;

public:
	TreeMapI();
	TreeMapI(const Map<K, V>&);	
	TreeMapI(const SortedMap<K, V>&);	
	TreeMapI(const Comparator<K>&);	

	Object clone() const;

public:
	bool containsKey(const K&) const;
	bool containsValue(const V&) const;
	Set<MapEntry<K, V> > entrySet() const;
	V get(const K&) const throw (IndexOutOfBoundsException); 
	Set<K> keySet() const;
	bool put(const K&, const V&);
	V remove(const K&) throw (IndexOutOfBoundsException);
	Collection<V> values() const;

	const K& firstKey() const;
	SortedMap<K, V> headMap(const K& to) throw (IndexOutOfBoundsException);
	const K& lastKey() const;
	SortedMap<K, V> subMap(const K& from, const K& to) throw (IndexOutOfBoundsException);
	SortedMap<K, V> tailMap(const K& from) throw (IndexOutOfBoundsException);
	Comparator<K> comparator() const;

private:	
	TreeSetBase<MapEntry<K, V> > entries;
	TreeMapI(const TreeSetBase<MapEntry<K, V> >&);

};

//********************************************************************

template< typename K, typename V >
struct TreeMap : StrongReference<TreeMapI<K, V> > 
{
	StrongReferenceTemplate2DerivateImplementation(TreeMap, K, V);
	operator Map<K, V>() { return get(); }
	operator Map<K, V>() const { return get(); }
	operator SortedMap<K, V>() { return get(); }
	operator SortedMap<K, V>() const { return get(); }

};

//********************************************************************
//********************************************************************

template< typename K, typename V > 
TreeMapI<K, V>::~TreeMapI() 
{}

//********************************************************************

template< typename K, typename V > 
TreeMapI<K, V>::TreeMapI() 
: entries(new TreeSetI<MapEntry<K, V> >())
{}

//********************************************************************

template< typename K, typename V > 
TreeMapI<K, V>::TreeMapI(const Map<K, V>& m) 
: entries(new TreeSetI<MapEntry<K, V> >(m->entrySet()))
{}

//********************************************************************

template< typename K, typename V > 
TreeMapI<K, V>::TreeMapI(const SortedMap<K, V>& s) 
: entries(new TreeSetI<MapEntry<K, V> >(s->comparator()))
{
	entries->addAll(s->entrySet());
}

//********************************************************************

template< typename K, typename V > 
TreeMapI<K, V>::TreeMapI(const Comparator<K>& c) 
: entries(new TreeSetI<MapEntry<K, V> >(c))
{}

//********************************************************************

template< typename K, typename V > 
TreeMapI<K, V>::TreeMapI(const TreeSetBase<MapEntry<K, V> >& e) 
: entries(e)
{}

//********************************************************************

template< typename K, typename V > 
Object TreeMapI<K, V>::clone() const 
{
	Map<K, V> c = new TreeMapI<K, V>();

	Iterator< MapEntry<K, V> > i = entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<K, V>& current = i->next();
		c->put(current.key, current.value);
	}

	return c;
}

//********************************************************************

template< typename K, typename V > 
bool TreeMapI<K, V>::containsKey(const K& key) const
{
	return 0 != &entries->lookup(MapEntry<K, V>(key));
}

//********************************************************************

template< typename K, typename V > 
bool TreeMapI<K, V>::containsValue(const V& value) const 
{
	Iterator<MapEntry<K, V> > i = entries->iterator();
	while (i->hasNext())
	{
		if (value == i->next().value) 
		{
			return true;
		}
	}
	return false;	
}

//********************************************************************

template< typename K, typename V > 
Set<MapEntry<K, V> > TreeMapI<K, V>::entrySet() const
{
	return entries.get();
}

//********************************************************************

template< typename K, typename V > 
V TreeMapI<K, V>::get(const K& key) const throw (IndexOutOfBoundsException)
{
	MapEntry<K, V>* entry = &entries->lookup(MapEntry<K, V>(key));
	if (0 != entry)
		return entry->value;
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename K, typename V > 
Set<K> TreeMapI<K, V>::keySet() const
{
	return new TreeKeySetI<K, V>(entries);
}

//********************************************************************

template< typename K, typename V > 
bool TreeMapI<K, V>::put(const K& key, const V& value) 
{
	return entries->add(MapEntry<K, V>(key, value));
}

//********************************************************************

template< typename K, typename V > 
V TreeMapI<K, V>::remove(const K& key) throw (IndexOutOfBoundsException)
{
	MapEntry<K, V>* entry = &entries->lookup(MapEntry<K, V>(key));
	if (0 != entry)
	{
		V value = entry->value;
		entries->remove(MapEntry<K, V>(key));
		return value;
	}
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename K, typename V > 
Collection<V> TreeMapI<K, V>::values() const
{
	return new ValueCollectionI<K, V>(entries);
}

//********************************************************************

template< typename K, typename V > 
const K& TreeMapI<K, V>::firstKey() const
{
	return entries->first().key;
}

//********************************************************************

template< typename K, typename V > 
SortedMap<K, V> TreeMapI<K, V>::headMap(const K& to_) throw (IndexOutOfBoundsException)
{
	if (this->containsKey(to_))
	{
		MapEntry<K, V> f;
		MapEntry<K, V> t = entries->lookup(MapEntry<K, V>(to_));
		std::cout << "headMap() in TreeMapI.h has to be checked again because of TreeSetFromTo" << std::endl;
		return new TreeMapI<K, V>(new TreeSetFromToI<MapEntry<K, V> >(f, t, false, true, false, entries)); 
	}
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename K, typename V > 
const K& TreeMapI<K, V>::lastKey() const
{
	return entries->last().key;
}

//********************************************************************

template< typename K, typename V > 
SortedMap<K, V> TreeMapI<K, V>::subMap(const K& from_, const K& to_) throw (IndexOutOfBoundsException)
{
	if (this->containsKey(from_) && this->containsKey(to_))
	{

		std::cout << "subMap() in TreeMapI.h has to be checked again because of TreeSetFromTo" << std::endl;
		return new TreeMapI<K, V>(new TreeSetFromToI<MapEntry<K, V> >(
			entries->lookup(MapEntry<K, V>(from_)), 
			entries->lookup(MapEntry<K, V>(to_)), 
			true, true,false, entries)); 
	}
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename K, typename V > 
SortedMap<K, V> TreeMapI<K, V>::tailMap(const K& from_) throw (IndexOutOfBoundsException)
{
	if (this->containsKey(from_))
	{
		MapEntry<K, V> f = entries->lookup(MapEntry<K, V>(from_));
		MapEntry<K, V> t;
		std::cout << "tailMap() in TreeMapI.h has to be checked again because of TreeSetFromTo" << std::endl;
		return new TreeMapI<K, V>(new TreeSetFromToI<MapEntry<K, V> >(f, t, true, false, false, entries)); 
	}
	else
		throw IndexOutOfBoundsException();
}

//********************************************************************

template< typename K, typename V > 
Comparator<K> TreeMapI<K, V>::comparator() const 
{
	SortedSet<K> keys;
	if (keySet()->instanceOf(keys))
	{
		return keys->comparator();
	}
	else
	{
		return new TreeSetI<K>::lessthancomparator();
	}
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
