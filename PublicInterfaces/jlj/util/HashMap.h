// HashMap.h

//********************************************************************

#ifndef HashMap_h
#define HashMap_h

//********************************************************************

#include <jlj/util/AbstractMap.h>
#include <jlj/util/ValueCollection.h>
#include <jlj/core/hashing/Hash.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V > struct HashMapEntry;

//********************************************************************

template< typename K, typename V > class HashMapI;

//********************************************************************

template< typename K, typename V > struct HashMap;

//********************************************************************

template< typename K, typename V, class X >
struct HashIteratorI : IteratorI<X>
{
public:
	bool hasNext() { return 0 != next; }

	void remove() 
	{
		if (0 == current)
		{
			next = 0;
			return;
		}
		if (map->modcount != expectedmodcount)
		{
			next = 0;
			return;
		}
		K key = current->key;
		current = 0;
		map->remove(key);
		expectedmodcount = map->modcount;
	}

	HashMapEntry<K, V>* nextEntry() throw (IllegalStateException) 
	{ 
		if (map->modcount != expectedmodcount)
		{
			next = 0;
			throw IllegalStateException(WITHDETAILS(L"Unexpected modcount (probably called get() while iterating).")); 
			return 0;
		}

		HashMapEntry<K, V>* e = next;
		if (0 == e)
		{
			next = 0;
			return 0;
		}

		HashMapEntry<K, V>* n = e->next;
		HashMapEntry<K, V>** t = map->table;
		int i = index;
		while (0 == n && i > 0)
		{
			n = t[--i];
		}
		index = i;
		next = n;
		return current = e;
	}

protected:
	~HashIteratorI() {};

	HashIteratorI(HashMap<K, V> map) 
		: map(map)	
		, expectedmodcount(map->modcount)
	{
		HashMapEntry<K, V>** t = map->table;
		int i = map->capacity;
		HashMapEntry<K, V>* n = 0;
		if (0 != map->size()) 
		{
			while (i > 0 && (0 == (n = t[--i])))
				;
		}
		next = n;
		index = i;
	}

private:
	HashMap<K, V> map;
	HashMapEntry<K, V>* next;                  
	HashMapEntry<K, V>* current;              
	int expectedmodcount;        
	int index;                   
};

//********************************************************************

template< typename K, typename V >
class HashMapI 
	: public AbstractMapI<K, V>
{
protected:
	~HashMapI();
	friend class TypedReference<HashMapI<K, V> >;

public:
	HashMapI();	
	HashMapI(int initialcapacity, double loadfactor);
	HashMapI(int initialcapacity);
	HashMapI(const Map<K, V>&);	

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
	void clear();
	int size() const;

public:
	virtual Iterator<K> newKeyIterator();
	virtual Iterator<V> newValueIterator();
	virtual Iterator<MapEntry<K, V> > newEntryIterator();

protected:
	inline void init(int initialcapacity, double loadfactor);
	inline void putAllForCreate(const Map<K, V>& m);
	inline void putForCreate(const K& key, const V& value);
	virtual void createEntry(int hash, const K& key, const V& value, int bucketindex); 
	virtual void addEntry(int hash, const K& key, const V& value, int bucketindex); 
	virtual void resize(int newcapacity);
	virtual void transfer(HashMapEntry<K, V>** oldtable, HashMapEntry<K, V>** table, int oldcapacity, int newcapacity);
	static inline int hash(const K& key);
	static inline int indexFor(int h, int length);

protected: // should be package-wide access
	int modcount;

protected:
	HashMapEntry<K, V>** table;
	int capacity;
	int threshold;
	int size_;
	double loadfactor;
	int maxbucketsize;

private:
	enum { DEFAULT_INITIAL_CAPACITY = 16 };
	enum { MAXIMUM_CAPACITY = 1 << 30 };
	enum { DEFAULT_LOAD_FACTOR_VALUE = 75 };

#define DEFAULT_LOAD_FACTOR (DEFAULT_LOAD_FACTOR_VALUE / 100.)

private:

	friend struct HashIteratorI<K, V, K>;
	friend struct HashIteratorI<K, V, V>;
	friend struct HashIteratorI<K, V, MapEntry<K, V> >;

	//********************************************************************

	struct KeyIteratorI : HashIteratorI<K, V, K>
	{
		inline KeyIteratorI(HashMap<K, V> map) : HashIteratorI<K, V, K>(map) {}
		inline K& next() { return HashIteratorI<K, V, K>::nextEntry()->key; }
	};

	//********************************************************************

	struct ValueIteratorI : HashIteratorI<K, V, V>
	{
		inline ValueIteratorI(HashMap<K, V> map) : HashIteratorI<K, V, V>(map) {}
		inline V& next() { return HashIteratorI<K, V, V>::nextEntry()->value; }
	};

	//********************************************************************

	struct EntryIteratorI : HashIteratorI<K, V, MapEntry<K, V> >
	{
		inline EntryIteratorI(HashMap<K, V> map) : HashIteratorI<K, V, MapEntry<K, V> >(map) {}
		inline MapEntry<K, V>& next() {	return *HashIteratorI<K, V, MapEntry<K, V> >::nextEntry(); }
	};

	//********************************************************************

	class Keys : public AbstractSetI<K> 
	{
	public:
		~Keys() {}
		Keys(HashMap<K, V> map) : map(map) {}		
		bool add(const K&) { return false; }
		Iterator<K> iterator() { return map->newKeyIterator(); }
		Iterator<K> iterator() const { return map->newKeyIterator(); }
		int size() const { return map->size(); }
		bool contains(const K& k) const { return map->containsKey(k); }
		void clear() { map->clear(); }
	private:
		HashMap<K, V> map;
	}; 

	//********************************************************************

	class Values : public AbstractCollectionI<V> 
	{
	public:
		~Values() {}
		Values(HashMap<K, V> map) : map(map) {}		
		bool add(const V&) { return false; }
		Iterator<V> iterator() { return map->newValueIterator(); }
		Iterator<V> iterator() const { return map->newValueIterator(); }
		int size() const { return map->size(); }
		bool contains(const V& v) const { return map->containsValue(v); }
		void clear() { map->clear(); }
	private:
		HashMap<K, V> map;
	}; 

	//********************************************************************

	class Entries : public AbstractSetI<MapEntry<K, V> > 
	{
	public:
		~Entries() {}
		Entries(HashMap<K, V> map) : map(map) {}		
		bool add(const MapEntry<K, V>&) { return false; }
		Iterator< MapEntry<K, V> > iterator() { return map->newEntryIterator(); }
		Iterator< MapEntry<K, V> > iterator() const { return map->newEntryIterator(); }
		int size() const { return map->size(); }
		bool contains(const MapEntry<K, V>& e) const { return map->containsKey(e.key); }
		void clear() { map->clear(); }
	private:
		HashMap<K, V> map;
	}; 

};

//********************************************************************

template< typename K, typename V >
struct HashMap : StrongReference<HashMapI<K, V> > 
{
	StrongReferenceTemplate2DerivateImplementation(HashMap, K, V);
	operator Map<K, V>() { return get(); }
	operator Map<K, V>() const { return get(); }
};

//********************************************************************

template< typename K, typename V >
struct HashMapEntry : MapEntry<K, V>
{
	inline HashMapEntry() {} 
	inline HashMapEntry(
		int hash,
		const K& key,
		const V& value,
		HashMapEntry<K, V>* next);

	virtual void recordAccess(Map<K, V>) {}
	virtual void recordRemoval(Map<K, V>) {}

	int hash;
	HashMapEntry<K, V>* next;
};

//********************************************************************
//********************************************************************

template< typename K, typename V > 
HashMapEntry<K, V>::HashMapEntry(int hash,
								 const K& key,
								 const V& value,
								 HashMapEntry<K, V>* next) 
								 : hash(hash)
								 , MapEntry<K, V>(key, value)
								 , next(next)
{}

//********************************************************************
//********************************************************************

template< typename K, typename V > 
HashMapI<K, V>::~HashMapI() 
{
	for (int j = 0; j < capacity; ++j) { delete table[j]; table[j] = 0; }
	delete table;
}

//********************************************************************

template< typename K, typename V > 
HashMapI<K, V>::HashMapI() 
{
	init(DEFAULT_INITIAL_CAPACITY, DEFAULT_LOAD_FACTOR);
}

//********************************************************************

template< typename K, typename V > 
HashMapI<K, V>::HashMapI(int initialcapacity, double loadfactor) 
{
	init(initialcapacity, loadfactor);
}

//********************************************************************

template< typename K, typename V > 
HashMapI<K, V>::HashMapI(int initialcapacity) 
{
	init(initialcapacity, DEFAULT_LOAD_FACTOR);
}

//********************************************************************

template< typename K, typename V > 
HashMapI<K, V>::HashMapI(const Map<K, V>& m) 
{
	int othercapacity = m->size();
	int capacity = DEFAULT_INITIAL_CAPACITY > othercapacity ? DEFAULT_INITIAL_CAPACITY : othercapacity;
	init(capacity, DEFAULT_LOAD_FACTOR);
	putAllForCreate(m);
}

//********************************************************************

template< typename K, typename V > 
void HashMapI<K, V>::init(int initialcapacity, 
						  double loadfactor)
{
	if (0 > initialcapacity)
		initialcapacity = DEFAULT_INITIAL_CAPACITY;
	if (initialcapacity > MAXIMUM_CAPACITY)
		initialcapacity = MAXIMUM_CAPACITY;
	if (0. >= loadfactor)
		loadfactor = DEFAULT_LOAD_FACTOR;
	else
		this->loadfactor = loadfactor;
	capacity = DEFAULT_INITIAL_CAPACITY;
	while (capacity < initialcapacity) 
		capacity <<= 1;   
	threshold = (int)(capacity * loadfactor);
	size_ = 0;
	modcount = 0;
	maxbucketsize = 1;
	table = new HashMapEntry<K, V>*[capacity];
	memset((char*)table, 0, sizeof(HashMapEntry<K, V>*) * capacity); 
}


//********************************************************************

template< typename K, typename V > 
void HashMapI<K, V>::putAllForCreate(const Map<K, V>& m)
{
	for (Iterator< MapEntry<K, V> > i = m->entrySet()->iterator(); i->hasNext(); ) 
	{
		MapEntry<K, V> e = i->next();
		putForCreate(e.key, e.value);
	}
}

//********************************************************************

template< typename K, typename V > 
void HashMapI<K, V>::putForCreate(const K& key, const V& value)
{
	int h = hash(key);
	int i = indexFor(h, capacity);

	bool bucketgrowth = false;
	int j = 0;
	for (HashMapEntry<K, V>* e = table[i]; 0 != e; e = e->next) 
	{
		if (++j > maxbucketsize) 
		{
			maxbucketsize = j;  
			bucketgrowth = true;
		}
		if (e->hash == h && key == e->key) 
		{
			e->value = value;
			return;
		}
	}

	createEntry(h, key, value, i);
}

//********************************************************************

template< typename K, typename V > 
void HashMapI<K, V>::createEntry(int h, 
								 const K& key, 
								 const V& value, 
								 int bucketindex) 
{
	table[bucketindex] = new HashMapEntry<K, V>(h, key, value, table[bucketindex]);
	++size_;
}

//********************************************************************

template< typename K, typename V > 
void HashMapI<K, V>::addEntry(int h, 
							  const K& key, 
							  const V& value, 
							  int bucketindex) 
{
	table[bucketindex] = new HashMapEntry<K, V>(h, key, value, table[bucketindex]);
	if (++size_ >= threshold)
	{
		resize(2 * capacity); // that's what we call geometric growth
	};
}

//********************************************************************

template< typename K, typename V > 
void HashMapI<K, V>::resize(int newcapacity) 
{
	HashMapEntry<K, V>** oldtable = table;
	int oldcapacity = capacity;

	if (0 < newcapacity)
	{
		capacity = newcapacity;

		table = new HashMapEntry<K, V>*[capacity];
		memset((char*)table, 0, sizeof(HashMapEntry<K, V>*) * capacity); 

		threshold = (int)(capacity * loadfactor);
		maxbucketsize = 1;

		if (0 != oldtable) 
		{
			transfer(oldtable, table, oldcapacity, newcapacity);
		}
	}
	else
	{
		capacity = 0;
		table = 0;
	}

	if (0 != oldtable)
	{
		for (int j = 0; j < oldcapacity; ++j) 
		{ 
			delete oldtable[j]; oldtable[j] = 0; 
		}
		delete oldtable;
	}
}

//********************************************************************

template< typename K, typename V > 
void HashMapI<K, V>::transfer(HashMapEntry<K, V>** oldtable,
							  HashMapEntry<K, V>** table,
							  int oldcapacity,
							  int newcapacity)
{
	if (0 < newcapacity)
	{
		for (int j = 0; j < oldcapacity; ++j) 
		{
			HashMapEntry<K, V>* e = oldtable[j];
			oldtable[j] = 0;
			if (0 != e) 
			{
				do {
					HashMapEntry<K, V>* next = e->next;
					int i = indexFor(e->hash, newcapacity);  
					e->next = table[i];
					table[i] = e;
					e = next;
				} while (0 != e);
			}
		}	
	}
	else
	{
		for (int j = 0; j < oldcapacity; ++j) 
		{
			HashMapEntry<K, V>* e = oldtable[j];
			oldtable[j] = 0;
			if (0 != e) 
			{
				do {
					HashMapEntry<K, V>* next = e->next;
					delete e;
					e = next;
				} while (0 != e);
			}
		}	
	}
}

//********************************************************************

template< typename K, typename V > 
int HashMapI<K, V>::hash(const K& key)
{
	return key->hashCode();
}

//********************************************************************

template< typename K, typename V > 
int HashMapI<K, V>::indexFor(int h, int capacity)
{
	return h & (capacity - 1);
}

//********************************************************************

template< typename K, typename V > 
Object HashMapI<K, V>::clone() const 
{
	Map<K, V> c = new HashMapI<K, V>(size() / .75);

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
bool HashMapI<K, V>::containsKey(const K& key) const
{
	int h = hash(key);
	int i = indexFor(h, capacity);
	for (HashMapEntry<K, V>* e = table[i]; 0 != e; e = e->next) 
	{
		if (e->hash == h && key == e->key) 
		{
			return true;
		}
	}
	return false;
}

//********************************************************************

template< typename K, typename V > 
bool HashMapI<K, V>::containsValue(const V& value) const 
{
#undef MAPACCEPTSNULLVALUE
#if MAPACCEPTSNULLVALUE
	if (isNull(value)) 
	{
		for (int i = 0; i < capacity ; ++i)
		{
			for (HashMapEntry<K, V>* e = table[i]; 0 != e; e = e->next) 
			{
				if (isNull(e->value))
				{
					return true;
				}
			}
		}
		return false;
	}
	else
	{
#endif
		for (int i = 0; i < capacity ; ++i)
		{
			for (HashMapEntry<K, V>* e = table[i]; 0 != e; e = e->next) 
			{
				if (value == e->value)
				{
					return true;
				}
			}
		}
		return false;
#if MAPACCEPTSNULLVALUE
	}
#endif
}

//********************************************************************

template< typename K, typename V > 
V HashMapI<K, V>::get(const K& key) const throw (IndexOutOfBoundsException)
{
	int h = hash(key);
	int i = indexFor(h, capacity);
	for (HashMapEntry<K, V>* e = table[i]; true; e = e->next) 
	{
		if (0 == e)
		{
			throw IndexOutOfBoundsException(WITHDETAILS(String(L"index : ") + key->toString()));
		}
		if (e->hash == h && key == e->key) 
		{
			e->recordAccess((HashMapI<K, V>*) this);
			return e->value;
		}
	}
	throw IndexOutOfBoundsException(WITHDETAILS(String(L"index : ") + key->toString()));
}

//********************************************************************

template< typename K, typename V > 
bool HashMapI<K, V>::put(const K& key, const V& value) 
{
	int h = hash(key);
	int i = indexFor(h, capacity);

	bool bucketgrowth = false;
	int j = 0;
	for (HashMapEntry<K, V>* e = table[i]; 0 != e; e = e->next) 
	{
		if (++j > maxbucketsize) 
		{
			maxbucketsize = j;  
			bucketgrowth = true;
		}
		if (e->hash == h && key == e->key) 
		{
			e->value = value;
			e->recordAccess(this);
			return false;
		}		
	}

	++modcount;
	addEntry(h, key, value, i);

	if (bucketgrowth && 4 < maxbucketsize)
	{
		resize(2 * capacity);
	}

	return true;
}

//********************************************************************

template< typename K, typename V > 
V HashMapI<K, V>::remove(const K& key) throw (IndexOutOfBoundsException)
{
	int h = hash(key);
	int i = indexFor(h, capacity);

	HashMapEntry<K, V>* prev = table[i];
	HashMapEntry<K, V>* e = prev; 
	while (0 != e)
	{
		HashMapEntry<K, V>* next = e->next; 
		if (e->hash == h && key == e->key)
		{
			V value = e->value;

			++modcount;
			--size_;
			if (prev == e)
			{
				table[i] = next;
			}
			else
			{
				prev->next = next;
			}
			e->recordRemoval(this);
			delete e;
			return value;
		}
		prev = e;
		e = next;
	}	

	throw IndexOutOfBoundsException(WITHDETAILS(L"index : " + String(i)));
}

//********************************************************************

template< typename K, typename V > 
Iterator<K> HashMapI<K, V>::newKeyIterator()
{
	return new KeyIteratorI(this);
}

//********************************************************************

template< typename K, typename V > 
Iterator<V> HashMapI<K, V>::newValueIterator() 
{
	return new ValueIteratorI(this);
}

//********************************************************************

template< typename K, typename V > 
Iterator< MapEntry<K, V> > HashMapI<K, V>::newEntryIterator() 
{
	return new EntryIteratorI(this);
}

//********************************************************************

template< typename K, typename V > 
Set<K> HashMapI<K, V>::keySet() const
{
	return new Keys(this);
}

//********************************************************************

template< typename K, typename V > 
Collection<V> HashMapI<K, V>::values() const
{
	return new Values(this);
}

//********************************************************************

template< typename K, typename V > 
Set<MapEntry<K, V> > HashMapI<K, V>::entrySet() const
{
	return new Entries(this);
}

//********************************************************************

template< typename K, typename V > 
void HashMapI<K, V>::clear() 
{
	++modcount;

	resize(0);
	resize(DEFAULT_INITIAL_CAPACITY);
	size_ = 0;

	maxbucketsize = 1;
}

//********************************************************************

template< typename K, typename V > 
int HashMapI<K, V>::size() const 
{
	return size_;
}

//********************************************************************

#undef DEFAULT_LOAD_FACTOR

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
