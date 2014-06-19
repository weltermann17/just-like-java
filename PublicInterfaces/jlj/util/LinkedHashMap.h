// LinkedHashMap.h

//********************************************************************

#ifndef LinkedHashMap_h
#define LinkedHashMap_h

//********************************************************************

#include <jlj/util/HashMap.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
struct LinkedHashMapEntry;

//********************************************************************

template< typename K, typename V >
class LinkedHashMapI;

//********************************************************************

template< typename K, typename V, class X >
class LinkedHashIteratorI
	: public IteratorI<X>
{
public:
	bool hasNext() { return map->sentinel != nextentry; }

	void remove() 
	{ 
		if (0 == lastreturned)
		{
			nextentry = 0;
			return;
		}
		if (map->modcount != expectedmodcount)
		{
			nextentry = 0;
			return;
		}
		map->remove(lastreturned->key);
		lastreturned = 0;
		expectedmodcount = map->modcount;
	}

	LinkedHashMapEntry<K, V>* nextEntry() 
	{
		if (map->modcount != expectedmodcount)
		{
			nextentry = 0;
			return 0;
		}
		if (map->sentinel == nextentry)
		{
			nextentry = 0;
			return 0;
		}
		LinkedHashMapEntry<K, V>* e = lastreturned = nextentry;
		nextentry = e->after;
		return e;		
	}

protected:
	~LinkedHashIteratorI() {};

	LinkedHashIteratorI(LinkedHashMapI<K, V>* map) 
		: map(map)	
		, expectedmodcount(map->modcount)
		, nextentry(map->sentinel->after)
		, lastreturned(0)
	{}

private:
	LinkedHashMapI<K, V>* map;
	LinkedHashMapEntry<K, V>* nextentry;                  
	LinkedHashMapEntry<K, V>* lastreturned;                  
	int expectedmodcount;       

};

//********************************************************************

template< typename K, typename V >
class LinkedHashMapI 
	: public HashMapI<K, V>
{
protected:
	~LinkedHashMapI();
	friend class TypedReference<LinkedHashMapI<K, V> >;

public:
	LinkedHashMapI();	

	LinkedHashMapI(int initialcapacity, double loadfactor);
	LinkedHashMapI(int initialcapacity);
	LinkedHashMapI(const Map<K, V>&);	
	LinkedHashMapI(int initialcapacity, double loadfactor, bool accessorder);

	Object clone() const;

public:
	bool containsValue(const V&) const;
	void clear();

public:
	Iterator<K> newKeyIterator();
	Iterator<V> newValueIterator();
	Iterator< MapEntry<K, V> > newEntryIterator();

protected:
	void createEntry(
		int hash, 
		const K& key, 
		const V& value, 
		int bucketindex); 
	void addEntry(
		int hash, 
		const K& key, 
		const V& value, 
		int bucketindex); 
	void transfer(
		HashMapEntry<K, V>** oldtable,
		HashMapEntry<K, V>** table,
		int oldcapacity,
		int newcapacity);

	// not really useful as it only removes 1 at a time
	virtual bool removeEldestEntry(const MapEntry<K, V>*) const;

private:
	bool accessorder;

	friend struct LinkedHashMapEntry<K, V>;

public: // should be package or private, public for HashIterator
	LinkedHashMapEntry<K, V>* sentinel;

private:

	//********************************************************************

	friend class LinkedHashIteratorI<K, V, K>;
	friend class LinkedHashIteratorI<K, V, V>;
	friend class LinkedHashIteratorI<K, V, MapEntry<K, V> >;

	//********************************************************************

	class KeyIterator 
		: public LinkedHashIteratorI<K, V, K>
	{
	public:
		KeyIterator(LinkedHashMapI<K, V>* map) : LinkedHashIteratorI<K, V, K>(map) {}

		K& next() 
		{
			return LinkedHashIteratorI<K, V, K>::nextEntry()->key;
		}
	};

	//********************************************************************

	class ValueIterator 
		: public LinkedHashIteratorI<K, V, V>
	{
	public:
		ValueIterator(LinkedHashMapI<K, V>* map) : LinkedHashIteratorI<K, V, V>(map) {}

		V& next() 
		{
			return LinkedHashIteratorI<K, V, V>::nextEntry()->value;
		}
	};

	//********************************************************************

	class EntryIterator 
		: public LinkedHashIteratorI<K, V,  MapEntry<K, V> >
	{
	public:
		EntryIterator(LinkedHashMapI<K, V>* map) : LinkedHashIteratorI<K, V,  MapEntry<K, V> >(map) {}

		MapEntry<K, V>& next() 
		{
			return *LinkedHashIteratorI<K, V, MapEntry<K, V> >::nextEntry();
		}
	};

};

//********************************************************************

template< typename K, typename V >
struct LinkedHashMap : StrongReference<LinkedHashMapI<K, V> > 
{
	StrongReferenceTemplate2DerivateImplementation(LinkedHashMap, K, V);
	operator HashMap<K, V>() { return get(); }
	operator HashMap<K, V>() const { return get(); }
};

//********************************************************************

template< typename K, typename V >
struct LinkedHashMapEntry : HashMapEntry<K, V>
{
	~LinkedHashMapEntry(); 

	LinkedHashMapEntry(); 
	LinkedHashMapEntry(
		int hash,
		const K& key,
		const V& value,
		HashMapEntry<K, V>* next);

	void recordAccess(Map<K, V>);
	void recordRemoval(Map<K, V>);

	void remove();
	void addBefore(LinkedHashMapEntry<K, V>* existing);

	LinkedHashMapEntry<K, V>* before;
	LinkedHashMapEntry<K, V>* after;

};

//********************************************************************
//********************************************************************

template< typename K, typename V > 
LinkedHashMapEntry<K, V>::~LinkedHashMapEntry() 
{}

//********************************************************************

template< typename K, typename V > 
LinkedHashMapEntry<K, V>::LinkedHashMapEntry() 
{
	before = after = this;
}

//********************************************************************

template< typename K, typename V > 
LinkedHashMapEntry<K, V>::LinkedHashMapEntry(int hash,
											 const K& key,
											 const V& value,
											 HashMapEntry<K, V>* next) 
											 : HashMapEntry<K, V>(hash
											 , key
											 , value
											 , next)
{}

//********************************************************************

template< typename K, typename V > 
void LinkedHashMapEntry<K, V>::recordAccess(Map<K, V> map) 
{
	StrongReference<LinkedHashMapI<K, V> > LinkedHashMapI;
	map->downcast(LinkedHashMapI);

	if (LinkedHashMapI->accessorder)
	{
		++(LinkedHashMapI->modcount);
		remove();
		addBefore(LinkedHashMapI->sentinel);
	}
}

//********************************************************************

template< typename K, typename V > 
void LinkedHashMapEntry<K, V>::recordRemoval(Map<K, V>) 
{
	remove();
}

//********************************************************************

template< typename K, typename V > 
void LinkedHashMapEntry<K, V>::remove() 
{
	before->after = after;
	after->before = before;
}

//********************************************************************

template< typename K, typename V > 
void LinkedHashMapEntry<K, V>::addBefore(LinkedHashMapEntry<K, V>* existing) 
{
	after = existing; 
	before = existing->before;
	before->after = this;
	after->before = this; 
}

//********************************************************************
//********************************************************************

template< typename K, typename V >  
LinkedHashMapI<K, V>::~LinkedHashMapI() 
{
	LinkedHashMapEntry<K, V>* e = sentinel->after;
	while (sentinel != e)
	{
		LinkedHashMapEntry<K, V>* after = e->after;
		delete e;		
		e = after;
	}
	HashMapI<K, V>::capacity = 0;
	delete sentinel;
}

//********************************************************************

template< typename K, typename V > 
LinkedHashMapI<K, V>::LinkedHashMapI() 
: accessorder(true)
, sentinel(new LinkedHashMapEntry<K, V>())
{}

//********************************************************************

template< typename K, typename V > 
LinkedHashMapI<K, V>::LinkedHashMapI(int initialcapacity,
									 double loadfactor) 
									 : accessorder(true)
									 , sentinel(new LinkedHashMapEntry<K, V>())
									 , HashMapI<K, V>(initialcapacity, loadfactor)
{}

//********************************************************************

template< typename K, typename V > 
LinkedHashMapI<K, V>::LinkedHashMapI(int initialcapacity) 
: accessorder(true)
, sentinel(new LinkedHashMapEntry<K, V>())
, HashMapI<K, V>(initialcapacity)
{}

//********************************************************************

template< typename K, typename V > 
LinkedHashMapI<K, V>::LinkedHashMapI(const Map<K, V>& m) 
: accessorder(true)
, sentinel(new LinkedHashMapEntry<K, V>())
, HashMapI<K, V>(m)
{}

//********************************************************************

template< typename K, typename V > 
LinkedHashMapI<K, V>::LinkedHashMapI(int initialcapacity,
									 double loadfactor,
									 bool accessorder) 
									 : accessorder(accessorder)
									 , sentinel(new LinkedHashMapEntry<K, V>())
									 , HashMapI<K, V>(initialcapacity, loadfactor)
{}

//********************************************************************

template< typename K, typename V > 
void LinkedHashMapI<K, V>::createEntry(int h, 
									   const K& key, 
									   const V& value, 
									   int bucketindex) 
{
	LinkedHashMapEntry<K, V>* e = 
		new LinkedHashMapEntry<K, V>(h, key, value, table[bucketindex]);
	HashMapI<K, V>::table[bucketindex] = e;
	e->addBefore(sentinel);
	++HashMapI<K, V>::size_;
}

//********************************************************************

template< typename K, typename V > 
void LinkedHashMapI<K, V>::addEntry(int h, 
									const K& key, 
									const V& value, 
									int bucketindex) 
{
	createEntry(h, key, value, bucketindex);
	LinkedHashMapEntry<K, V>* eldest = sentinel->after;
	if (removeEldestEntry(eldest))
	{
		remove(eldest->key);
	}
	else
	{
		if (HashMapI<K, V>::size_ >= HashMapI<K, V>::threshold)
		{
			resize(2 * HashMapI<K, V>::capacity);
		}
	}
}

//********************************************************************

template< typename K, typename V > 
void LinkedHashMapI<K, V>::transfer(HashMapEntry<K, V>** oldtable,
									HashMapEntry<K, V>** table,
									int oldcapacity,
									int newcapacity)
{
	if (0 < newcapacity)
	{
		int ii = 0;
		for (LinkedHashMapEntry<K, V>* e = sentinel->after; sentinel != e; e = e->after)
		{
			int j = indexFor(e->hash, oldcapacity); 
			oldtable[j] = 0;

			int i = indexFor(e->hash, newcapacity); 

			e->next = HashMapI<K, V>::table[i];
			HashMapI<K, V>::table[i] = e;
		}
	}
	else
	{
		clear();
	}
}

//********************************************************************

template< typename K, typename V > 
Iterator<K> LinkedHashMapI<K, V>::newKeyIterator()
{
	return new HashMapI<K, V>::KeyIteratorI(this);
}

//********************************************************************

template< typename K, typename V > 
Iterator<V> LinkedHashMapI<K, V>::newValueIterator() 
{
	return new HashMapI<K, V>::ValueIteratorI(this);
}

//********************************************************************

template< typename K, typename V > 
Iterator< MapEntry<K, V> > LinkedHashMapI<K, V>::newEntryIterator() 
{
	return new HashMapI<K, V>::EntryIteratorI(this);
}

//********************************************************************

template< typename K, typename V > 
Object LinkedHashMapI<K, V>::clone() const 
{
	Map<K, V> c = new LinkedHashMapI<K, V>(size() / .75);

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
bool LinkedHashMapI<K, V>::containsValue(const V& value) const 
{
#if MAPACCEPTSNULLVALUE
	if (isNull(value)) 
	{
		for (LinkedHashMapEntry<K, V>* e = sentinel->after; sentinel != e; e = e->after)
		{
			if (isNull(e->value)) 
			{
				return true;
			}
		}
	} 
	else 
	{
#endif
		for (LinkedHashMapEntry<K, V>* e = sentinel->after; sentinel != e; e = e->after)
		{
			if (value == e->value) 
			{
				return true;
			}
		}
#if MAPACCEPTSNULLVALUE
	}
#endif
	return false;
}

//********************************************************************

template< typename K, typename V > 
void LinkedHashMapI<K, V>::clear() 
{
	LinkedHashMapEntry<K, V>* e = sentinel->after;
	while (sentinel != e) 
	{
		LinkedHashMapEntry<K, V>* after = e->after;
		delete e;		
		e = after;
	} 
	memset((char*)HashMapI<K, V>::table, 0, sizeof(HashMapEntry<K, V>*) * HashMapI<K, V>::capacity); 
	++HashMapI<K, V>::modcount;
	HashMapI<K, V>::size_ = 0;
	sentinel->before = sentinel->after = sentinel;
}

//********************************************************************

template< typename K, typename V > 
bool LinkedHashMapI<K, V>::removeEldestEntry(const MapEntry<K, V>*) const
{
	return false;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
