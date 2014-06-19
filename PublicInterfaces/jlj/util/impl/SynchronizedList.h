// SynchronizedList.h

//********************************************************************

#ifndef jlj_util_impl_SynchronizedList_h
#define jlj_util_impl_SynchronizedList_h

//********************************************************************

#include <jlj/util/impl/SynchronizedCollection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename V >
class SynchronizedListI 
	: public virtual ListI<V>
	, public SynchronizedCollectionI<V>
{
protected:
	~SynchronizedListI();
	friend class TypedReference<SynchronizedListI<V> >;

public:
	SynchronizedListI(List<V>);

public:
	bool equals(const Object& object) const;

public:	
	bool add(const V&);
	bool addAll(const Collection<V>&);
	void add(int index, const V&) throw (IndexOutOfBoundsException);
	void addAll(int index, const Collection<V>&) throw (IndexOutOfBoundsException);
	V get(int index) const throw (IndexOutOfBoundsException);
	int indexOf(const V&) const;
	ListIterator<V> listIterator(bool tail = false);
	ListIterator<V> listIterator(bool tail = false) const;
	int lastIndexOf(const V&) const;
	V removeAt(int index) throw (IndexOutOfBoundsException);
	V set(int index, const V&) throw (IndexOutOfBoundsException);
	List<V> subList(int from, int to) const throw (IndexOutOfBoundsException, IllegalArgumentException);

private:
	List<V> list;

};

//********************************************************************

template< typename V > 
struct SynchronizedList : StrongReference<SynchronizedListI<V> >
{
	StrongReferenceTemplateDerivateImplementation(SynchronizedList, V);
};

//********************************************************************
//********************************************************************

template< typename V > 
SynchronizedListI<V>::~SynchronizedListI() 
{}

//********************************************************************

template< typename V > 
SynchronizedListI<V>::SynchronizedListI(List<V> list)
: list(list)
, SynchronizedCollectionI<V>(list) 
{}

//********************************************************************

template< typename V > 
bool SynchronizedListI<V>::equals(const Object& object) const
{
	bool ok = false;
	List<V> b;
	synchronized(lock)
	{
		if (object->instanceOf(b))
		{
			ok = list->equals(b);
		}
	}
	return ok;
}

//********************************************************************

template< typename V > 
bool SynchronizedListI<V>::add(const V& v)
{
	return SynchronizedCollectionI<V>::add(v);
}

//********************************************************************

template< typename V > 
bool SynchronizedListI<V>::addAll(const Collection<V>& other)
{
	return SynchronizedCollectionI<V>::addAll(other);
}

//********************************************************************

template< typename V > 
void SynchronizedListI<V>::add(int index, 
							   const V& v) throw (IndexOutOfBoundsException)
{
	synchronized(lock) list->add(index, v);
}

//********************************************************************

template< typename V > 
void SynchronizedListI<V>::addAll(int index, 
								  const Collection<V>& other) throw (IndexOutOfBoundsException)
{
	synchronized(lock) list->addAll(index, other);
}

//********************************************************************

template< typename V > 
V SynchronizedListI<V>::get(int index) const throw (IndexOutOfBoundsException)
{
	V v;
	synchronized(lock) v = list->get(index);
	return v;
}

//********************************************************************

template< typename V > 
int SynchronizedListI<V>::indexOf(const V& v) const
{
	int i;
	synchronized(lock) i = list->indexOf(v);
	return i;
}

//********************************************************************

template< typename V > 
ListIterator<V> SynchronizedListI<V>::listIterator(bool tail)
{
	ListIterator<V> i;
	synchronized(lock) i = list->listIterator(tail);
	return i;
}

//********************************************************************

template< typename V > 
ListIterator<V> SynchronizedListI<V>::listIterator(bool tail) const
{
	ListIterator<V> i;
	synchronized(lock) i = list->listIterator(tail);
	return i;
}

//********************************************************************

template< typename V > 
int SynchronizedListI<V>::lastIndexOf(const V& v) const
{
	int i;
	synchronized(lock) i = list->lastIndexOf(v);
	return i;
}

//********************************************************************

template< typename V > 
V SynchronizedListI<V>::removeAt(int index) throw (IndexOutOfBoundsException)
{
	V v;
	synchronized(lock) v = list->removeAt(index);
	return v;
}

//********************************************************************

template< typename V > 
V SynchronizedListI<V>::set(int index,
							const V& v) throw (IndexOutOfBoundsException)
{
	V old;
	synchronized(lock) old = list->set(index, v);
	return old;
}

//********************************************************************

template< typename V > 
List<V> SynchronizedListI<V>::subList(int from, 
									  int to) const 
									  throw (IndexOutOfBoundsException, IllegalArgumentException)
{
	List<V> sublist;
	synchronized(lock) sublist = list->subList(from, to);
	return sublist;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
