// AbstractList.h

//********************************************************************

#ifndef jlj_util_AbstractList_h
#define jlj_util_AbstractList_h

//********************************************************************

#include <jlj/util/List.h>
#include <jlj/util/AbstractCollection.h>
#include <jlj/lang/Throwable.h>
using jlj::lang::IndexOutOfBoundsException;
using jlj::lang::IllegalArgumentException;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class AbstractListI 
	: public ListI<T>
	, public AbstractCollectionI<T>
{
public:
	~AbstractListI() = 0;
	AbstractListI();

public:
	bool add(const T&) = 0;
	bool addAll(const Collection<T>&);

public:
	void add(int index, const T&) throw (IndexOutOfBoundsException) = 0;
	void addAll(int index, const Collection<T>&) throw (IndexOutOfBoundsException);
	bool equals(const Object&) const;
	int indexOf(const T&) const;
	int lastIndexOf(const T&) const;
	T removeAt(int index) throw (IndexOutOfBoundsException);
	T set(int index, const T&) throw (IndexOutOfBoundsException);
	List<T> subList(int from, int to) const throw (IndexOutOfBoundsException, IllegalArgumentException);

protected:
	void removeRange(int from, int to) throw (IndexOutOfBoundsException, IllegalArgumentException);
	virtual AbstractListI<T>* newList(int capacity) const = 0;

};

//********************************************************************

template< typename T > 
AbstractListI<T>::~AbstractListI() 
{}

//********************************************************************

template< typename T > 
AbstractListI<T>::AbstractListI() 
{}

//********************************************************************

template< typename T > 
bool AbstractListI<T>::addAll(const Collection<T>& c)
{
	return AbstractCollectionI<T>::addAll(c);
}

//********************************************************************

template< typename T > 
void AbstractListI<T>::addAll(int index, const Collection<T>& c) throw (IndexOutOfBoundsException)
{
	if (0 > index || index > size()) throw IndexOutOfBoundsException(WITHDETAILS(L""));
	Iterator<T> i = c->iterator();
	while (i->hasNext())
	{
		this->add(index++, i->next());
	}
}

//********************************************************************

template< typename T > 
bool AbstractListI<T>::equals(const Object& object) const
{
	Collection<T> c;

	if (object->instanceOf(c))
	{
		if (this->size() == c->size())
		{
			Iterator<T> i = this->iterator();
			Iterator<T> j = c->iterator();
			while (i->hasNext())
			{
				if (!(i->next() == j->next()))
				{
					return false;
				}				
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

//********************************************************************

template< typename T > 
int AbstractListI<T>::indexOf(const T& element) const 
{
	int j = -1;
	Iterator<T> i = this->iterator();
	while (i->hasNext())
	{
		j++;
		if (element == i->next())
		{
			return j;
		}
	}
	return -1;
}

//********************************************************************

template< typename T > 
int AbstractListI<T>::lastIndexOf(const T& element) const 
{
	int j = this->size();
	ListIterator<T> i = this->listIterator(true); // start at tail
	while (i->hasPrevious())
	{
		j--;
		if (element == i->previous())
		{
			return j;
		}
	}
	return -1;
}

//********************************************************************

template< typename T > 
T AbstractListI<T>::removeAt(int index) throw (IndexOutOfBoundsException)
{
	if (0 > index || index >= size()) throw IndexOutOfBoundsException(WITHDETAILS(L""));
	T* element = 0;
	Iterator<T> i = this->iterator();
	int j = 0;
	while (i->hasNext() && (j < index))
	{
		element = & (i->next());
		j++;
	}
	i->remove();
	return *element;
}

//********************************************************************

template< typename T > 
T AbstractListI<T>::set(int index, const T& element) throw (IndexOutOfBoundsException) 
{
	if (0 > index || index >= size()) throw IndexOutOfBoundsException(WITHDETAILS(L""));
	T old = this->get(index);
	this->removeAt(index);
	this->add(index, element);
	return old;
}

//********************************************************************

template< typename T > 
List<T> AbstractListI<T>::subList(int from, int to) const throw (IndexOutOfBoundsException, IllegalArgumentException)
{
	if (0 > from || from >= size() || 0 >= to || to > size()) throw IndexOutOfBoundsException(WITHDETAILS(L""));
	if (from >= to)	throw IllegalArgumentException(WITHDETAILS(L""));

	AbstractListI<T>* newlist = this->newList(to - from);

	Iterator<T> i = this->iterator();
	int j = 0;

	while (i->hasNext() && (j < to))
	{
		T& element = i->next();
		j++;
		if (from < j)
		{
			newlist->add(element);
		}
	}

	return newlist;
}

//********************************************************************

template< typename T > 
void AbstractListI<T>::removeRange(int from, int to) throw (IndexOutOfBoundsException, IllegalArgumentException) 
{
	if (0 > from || from >= size() || 0 >= to || to > size()) throw IndexOutOfBoundsException(WITHDETAILS(L""));
	if (from >= to)	throw IllegalArgumentException(WITHDETAILS(L""));

	Iterator<T> i = this->iterator();
	int j = 0;

	while (i->hasNext() && (j < to))
	{
		i->next();
		i->remove();
		j++;
	}
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
