// AbstractCollection.h

//********************************************************************

#ifndef jlj_util_AbstractCollection_h
#define jlj_util_AbstractCollection_h

//********************************************************************

#include <jlj/util/Collection.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class AbstractCollectionI 
	: public virtual CollectionI<T>
{
public:
	~AbstractCollectionI() = 0;
	AbstractCollectionI();

public:
	bool addAll(const Collection<T>&);
	void clear();
	bool contains(const T&) const;
	bool containsAll(const Collection<T>&) const;
	bool equals(const Object&) const;
	bool isEmpty() const;
	bool remove(const T&);
	bool removeAll(const Collection<T>&);
	bool retainAll(const Collection<T>&);

};

//********************************************************************

template< typename T > 
AbstractCollectionI<T>::~AbstractCollectionI() 
{}

//********************************************************************

template< typename T > 
AbstractCollectionI<T>::AbstractCollectionI() 
{}

//********************************************************************

template< class T> 
bool AbstractCollectionI<T>::addAll(const Collection<T>& c) 
{
	Iterator<T> i = c->iterator();
	while (i->hasNext())
	{
		if (!this->add(i->next()))
		{
			return false;

		}
	}
	return true;
}

//********************************************************************

template< typename T > 
void AbstractCollectionI<T>::clear() 
{
	Iterator<T> i = this->iterator();
	while (i->hasNext())
	{
		i->next();
		i->remove();
	}
}

//********************************************************************

template< typename T > 
bool AbstractCollectionI<T>::contains(const T& element) const 
{
	Iterator<T> i = this->iterator();
	while (i->hasNext())
	{
		if (element == i->next())
		{
			return true;
		}
	}
	return false;
}

//********************************************************************

template< typename T > 
bool AbstractCollectionI<T>::containsAll(const Collection<T>& c) const 
{
	Iterator<T> i = c->iterator();
	while (i->hasNext())
	{
		if (!this->contains(i->next()))
		{
			return false;
		}
	}
	return true;
}

//********************************************************************

template< typename T > 
bool AbstractCollectionI<T>::equals(const Object& object) const 
{
	Collection<T> c;

	if (object->instanceOf(c))
	{
		if (size() == c->size())
		{
			Iterator<T> i = this->iterator();
			while (i->hasNext())
			{
				if (c->contains(i->next())) continue;
				return false;
			}
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

//********************************************************************

template< typename T > 
bool AbstractCollectionI<T>::isEmpty() const 
{
	return 0 == size();
}

//********************************************************************

template< typename T > 
bool AbstractCollectionI<T>::remove(const T& element) 
{
	Iterator<T> i = this->iterator();
	while (i->hasNext())
	{
		if (element == i->next())
		{
			i->remove();
			return true;
		}
	}
	return false;
}

//********************************************************************

template< typename T > 
bool AbstractCollectionI<T>::removeAll(const Collection<T>& c) 
{
	bool hasChanged = false;
	Iterator<T> i = c->iterator();
	while (i->hasNext())
	{
		bool changed = false;
		T& element = i->next();
		do
		{
			changed = this->remove(element);
			hasChanged = hasChanged || changed;
		}
		while (changed);
	}
	return hasChanged;
}

//********************************************************************

template< typename T > 
bool AbstractCollectionI<T>::retainAll(const Collection<T>& c) 
{
	bool hasChanged = false;
	Iterator<T> i = this->iterator();
	while (i->hasNext())
	{
		T& element = i->next();
		if (!c->contains(element))
		{
			i->remove();
			hasChanged = hasChanged || true;
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
