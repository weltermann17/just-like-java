// HashSetI.h

//********************************************************************

#ifndef HashSet_h
#define HashSet_h

//********************************************************************

#include <jlj/util/AbstractSet.h>
#include <jlj/util/LinkedHashMap.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class LinkedHashSetI;

//********************************************************************

template< typename T >
class HashSetI 
	: public AbstractSetI<T>
{
protected:
	~HashSetI();
	friend class TypedReference<HashSetI<T> >;

public:
	HashSetI();	

	HashSetI(const Collection<T>&);
	HashSetI(int initialcapacity, double loadfactor);
	HashSetI(int initialcapacity);

public:

public:
	Iterator<T> iterator();
	Iterator<T> iterator() const;
	bool contains(const T&) const;
	bool add(const T&);
	bool remove(const T&);
	void clear();
	int size() const;

private:
	HashSetI(int initialcapacity, double loadfactor, bool);
	friend class LinkedHashSetI<T>;

private:
	Map<T, int> map;

};

//********************************************************************

template< typename T >
struct HashSet : StrongReference<HashSetI<T> > 
{
	StrongReferenceTemplateDerivateImplementation(HashSet, T);
};

//********************************************************************
//********************************************************************

template< typename T > 
HashSetI<T>::~HashSetI() 
{}

//********************************************************************

template< typename T > 
HashSetI<T>::HashSetI() 
: map(new HashMapI<T, int>())
{}

//********************************************************************

template< typename T > 
HashSetI<T>::HashSetI(const Collection<T>& c) 
: map(new HashMapI<T, int>((c->size() / .75 + 1) > 16 ? (c->size() / .75 + 1) : 16))
{
	addAll(c);
}

//********************************************************************

template< typename T > 
HashSetI<T>::HashSetI(int initialcapacity,
					  double loadfactor) 
					  : map(new HashMapI<T, int>(initialcapacity, loadfactor))
{}

//********************************************************************

template< typename T > 
HashSetI<T>::HashSetI(int initialcapacity) 
: map(new HashMapI<T, int>(initialcapacity))
{}

//********************************************************************

template< typename T > 
HashSetI<T>::HashSetI(int initialcapacity,
					  double loadfactor,
					  bool) 
					  : map(new LinkedHashMapI<T, int>(initialcapacity, loadfactor))
{}

//********************************************************************

template< typename T > 
Iterator<T> HashSetI<T>::iterator()  
{
	return map->keySet()->iterator();
}

//********************************************************************

template< typename T > 
Iterator<T> HashSetI<T>::iterator() const
{
	return map->keySet()->iterator();
}

//********************************************************************

template< typename T > 
bool HashSetI<T>::contains(const T& element) const 
{
	return map->containsKey(element);
}

//********************************************************************

template< typename T > 
bool HashSetI<T>::add(const T& element) 
{
	return map->put(element, 0);
}

//********************************************************************

template< typename T > 
bool HashSetI<T>::remove(const T& element) 
{
	bool contained = map->containsKey(element);
	if (contained) map->remove(element);
	return contained;
}

//********************************************************************

template< typename T > 
void HashSetI<T>::clear() 
{
	map->clear();
}

//********************************************************************

template< typename T > 
int HashSetI<T>::size() const 
{
	return map->size();
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
