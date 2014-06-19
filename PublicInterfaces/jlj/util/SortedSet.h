// SortedSet.h

//********************************************************************

#ifndef SortedSet_h
#define SortedSet_h

//********************************************************************

#include <jlj/util/Set.h>
#include <jlj/util/Comparator.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T > struct SortedSet; 

//********************************************************************

template< typename T >
class SortedSetI 
	: public virtual SetI<T>
{
public:
	virtual ~SortedSetI() = 0;
	SortedSetI();

public:
	virtual const T& first() const = 0;
	virtual SortedSet<T> headSet(const T& to) throw (IndexOutOfBoundsException) = 0;
	virtual const T& last() const = 0;
	virtual SortedSet<T> subSet(const T& from, const T& to) throw (IndexOutOfBoundsException) = 0;
	virtual SortedSet<T> tailSet(const T& from) throw (IndexOutOfBoundsException) = 0;
	virtual Comparator<T> comparator() const = 0;

};

//********************************************************************

template< typename T >
struct SortedSet : StrongReference<SortedSetI<T> > 
{
	SortedSet();
	SortedSet(SortedSetI<T>*);

};

//********************************************************************

template< typename T > 
SortedSetI<T>::~SortedSetI() 
{}

//********************************************************************

template< typename T > 
SortedSetI<T>::SortedSetI() 
{}

//********************************************************************

template< typename T > 
SortedSet<T>::SortedSet() 
{}

//********************************************************************

template< typename T > 
SortedSet<T>::SortedSet(SortedSetI<T>* p) 
: StrongReference<SortedSetI<T> >(p)
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
