// MapEntry.h

//********************************************************************

#ifndef MapEntry_h
#define MapEntry_h

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename K, typename V >
struct MapEntry 
{
	inline ~MapEntry(); 

	inline MapEntry(); 
	inline MapEntry(const K&, const V&);
	inline MapEntry(const K&);
	
	inline MapEntry(const MapEntry<K, V>&);
	inline MapEntry<K, V>& operator=(const MapEntry<K, V>&);
	MapEntry<K, V>* operator->();

	inline const K& getKey() const;
	inline K& getKey();
	inline const V& getValue() const;
	inline V& getValue();

	inline bool operator==(const MapEntry<K, V>&) const;

	K key;
	V value;
};

//********************************************************************

template< typename K, typename V >
bool operator<(const MapEntry<K, V>&, const MapEntry<K, V>&);

//********************************************************************

template< typename K, typename V >
bool operator<(const MapEntry<K, V>& a, const MapEntry<K, V>& b) 
{
	return a.key < b.key;
}

//********************************************************************

template< typename K, typename V > 
MapEntry<K, V>::~MapEntry()
{}

//********************************************************************

template< typename K, typename V > 
MapEntry<K, V>::MapEntry()
{}

//********************************************************************

template< typename K, typename V > 
MapEntry<K, V>::MapEntry(const MapEntry<K, V>& copy)
: key(copy.key), value(copy.value) 
{}

//********************************************************************

template< typename K, typename V > 
MapEntry<K, V>& MapEntry<K, V>::operator=(const MapEntry<K, V>& assign) 
{
	if (this != &assign)
	{
		key = assign.key;
		value = assign.value;
	}
	return *this;
}

//********************************************************************

template< typename K, typename V > 
MapEntry<K, V>* MapEntry<K, V>::operator->() 
{
	return this;
}

//********************************************************************

template< typename K, typename V > 
MapEntry<K, V>::MapEntry(const K& k, const V& v)
: key(k), value(v)
{}

//********************************************************************

template< typename K, typename V > 
MapEntry<K, V>::MapEntry(const K& k)
: key(k)
{}

//********************************************************************

template< typename K, typename V > 
const K& MapEntry<K, V>::getKey() const  
{
	return key;
}

//********************************************************************

template< typename K, typename V > 
K& MapEntry<K, V>::getKey()   
{
	return key;
}

//********************************************************************

template< typename K, typename V > 
V& MapEntry<K, V>::getValue() 
{
	return value;
}

//********************************************************************

template< typename K, typename V > 
const V& MapEntry<K, V>::getValue() const
{
	return value;
}

//********************************************************************

template< typename K, typename V >
bool MapEntry<K, V>::operator==(const MapEntry<K, V>& b) const
{
	return key == b.key;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
