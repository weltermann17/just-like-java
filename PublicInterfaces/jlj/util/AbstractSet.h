// AbstractSet.h

//********************************************************************

#ifndef jlj_util_AbstractSet_h
#define jlj_util_AbstractSet_h

//********************************************************************

#include <jlj/util/Set.h>
#include <jlj/util/AbstractCollection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class AbstractSetI 
	: public virtual SetI<T>
	, public AbstractCollectionI<T>
{
public:
	~AbstractSetI();
	AbstractSetI();

public:
	bool removeAll(const Collection<T>&);

};

//********************************************************************
//********************************************************************

template< typename T > 
AbstractSetI<T>::~AbstractSetI() 
{}

//********************************************************************

template< typename T > 
AbstractSetI<T>::AbstractSetI() 
{}

//********************************************************************

/**
* This implementation determines which is the smaller of this set and the specified collection, 
* by invoking the size method on each. If this set has fewer elements, then the implementation 
* iterates over this set, checking each element returned by the iterator in turn to see if it 
* is contained in the specified collection. If it is so contained, it is removed from this set 
* with the iterator's remove method. If the specified collection has fewer elements, then the 
* implementation iterates over the specified collection, removing from this set each element 
* returned by the iterator, using this set's remove method.
*/

template< typename T > 
bool AbstractSetI<T>::removeAll(const Collection<T>& c)  
{
	bool hasChanged = false;
	if (this->size() < c->size())
	{
		Iterator<T> i = this->iterator();
		while (i->hasNext())
		{
			if (c->contains(i->next()))
			{
				i->remove();
				hasChanged = true;
			}
		}
	}
	else
	{
		Iterator<T> i = c->iterator();
		while (i->hasNext())
		{
			bool changed = this->remove(i->next());
			hasChanged = hasChanged || changed;
		}
	}
	return hasChanged;
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
