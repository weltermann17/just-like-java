// ArrayList.h

//********************************************************************

#ifndef ArrayList_h
#define ArrayList_h

//********************************************************************

#include <jlj/util/AbstractList.h>
#include <jlj/util/ArrayListIterator.h>


//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class ArrayListI
	: public AbstractListI<T>
{
protected:
	~ArrayListI();
	friend class TypedReference<ArrayListI<T> >;

public:
	ArrayListI();
	ArrayListI(const Collection<T>&);	
	ArrayListI(int capacity);

public:
	Object clone() const;
	bool add(const T&);
	void add(int index, const T&) throw (IndexOutOfBoundsException);
	void clear();
	T get(int index) const throw (IndexOutOfBoundsException);
	Iterator<T> iterator();
	Iterator<T> iterator() const;
	ListIterator<T> listIterator(bool tail);
	ListIterator<T> listIterator(bool tail) const;
	T removeAt(int index) throw (IndexOutOfBoundsException);
	int size() const;

protected:
	AbstractListI<T>* newList(int capacity) const;

private:
	void init(int capacity);
	void increaseCapacity(int minimum, int offset);

	/**
	* What is a good initial capacity?
	* Many implementations choose 16, 32 or 64.
	* We chose 16 for now.
	*/
	enum { INITIALCAPACITY = 16 }; 

	/**
	* What is a good growth rate?
	* In any case it is geometric and this can get out of hand.
	* We chose 1.25 for now. 
	*/
	enum { GROWTHRATE = 125 }; // divide by 100. to use

	T* array;
	int capacity;
	int sentinel;

};

//********************************************************************
//********************************************************************

template< typename T >
struct ArrayList : StrongReference<ArrayListI<T> >
{
	StrongReferenceTemplateDerivateImplementation(ArrayList, T);
};

//********************************************************************

template< typename T > 
ArrayListI<T>::~ArrayListI() 
{
	delete [] array;
}

//********************************************************************

template< typename T > 
ArrayListI<T>::ArrayListI() 
{
	init(INITIALCAPACITY);
}

//********************************************************************

template< typename T > 
ArrayListI<T>::ArrayListI(const Collection<T>& c) 
{
	init(c->size());
	addAll(c);
}

//********************************************************************

template< typename T > 
ArrayListI<T>::ArrayListI(int capacity_) 
{
	if (INITIALCAPACITY > capacity_) 
		capacity_ = INITIALCAPACITY;
	init(capacity_);
}

//********************************************************************

template< typename T > 
void ArrayListI<T>::init(int capacity_) 
{
	capacity = capacity_;
	array = new T[capacity];
	sentinel = 0;
}

//********************************************************************

template< typename T > 
void ArrayListI<T>::increaseCapacity(int minimum, int offset) 
{
	int newcapacity = (int)(capacity * (GROWTHRATE/100.));

	T* newarray = new T[newcapacity];
	ASSERT(0 != newarray);
	for (int i = 0; i < capacity; ++i)
	{
		if (i < minimum)
		{
			newarray[i] = array[i];
		}
		else
		{
			newarray[i + offset] = array[i];
		}
	}
	delete [] array;
	array = newarray;
	capacity = newcapacity;
}

//********************************************************************

template< typename T > 
Object ArrayListI<T>::clone() const
{
	List<T> c = new ArrayListI<T>(size());

	Iterator<T> i = iterator();
	while (i->hasNext())
	{
		c->add(i->next());
	} 

	return c;
}

//********************************************************************

template< typename T > 
bool ArrayListI<T>::add(const T& element) 
{
	if (sentinel == capacity)
	{
		increaseCapacity(sentinel, 0);
	}
	array[sentinel] = element;
	sentinel++;
	return true;
}

//********************************************************************

template< typename T > 
void ArrayListI<T>::add(int index, const T& element) throw (IndexOutOfBoundsException)
{
	if ((0 > index) || (index > sentinel)) throw IndexOutOfBoundsException(WITHDETAILS(L""));
	
	if (sentinel == capacity)
	{
		increaseCapacity(index, 1);
	}
	else
	{
		for (int i = sentinel; i > index; --i)
		{
			array[i] = array[i - 1];
		}
	}

	array[index] = element;
	sentinel++;
}

//********************************************************************

template< typename T > 
void ArrayListI<T>::clear()
{
	delete [] array;
	init(INITIALCAPACITY);
}

//********************************************************************

template< typename T > 
T ArrayListI<T>::get(int index) const throw (IndexOutOfBoundsException)
{
	if ((0 > index) || (index >= sentinel)) throw IndexOutOfBoundsException(WITHDETAILS(L""));
	return array[index];
}

//********************************************************************

template< typename T > 
T ArrayListI<T>::removeAt(int index) throw (IndexOutOfBoundsException)
{
	if ((0 > index) || (index >= sentinel)) throw IndexOutOfBoundsException(WITHDETAILS(L""));
	T element = array[index];
	for (int i = index; i < sentinel - 1; ++i)
	{
		array[i] = array[i + 1];
	}
	sentinel--;
	return element;
}

//********************************************************************

template< typename T > 
Iterator<T> ArrayListI<T>::iterator()  
{
	return new ArrayListIteratorI<T>(array, false, this);
}

//********************************************************************

template< typename T > 
Iterator<T> ArrayListI<T>::iterator() const
{
	return new ArrayListIteratorI<T>(array, false, this);
}

//********************************************************************

template< typename T > 
ListIterator<T> ArrayListI<T>::listIterator(bool tail)  
{
	return new ArrayListIteratorI<T>(array, tail, this);
}

//********************************************************************

template< typename T > 
ListIterator<T> ArrayListI<T>::listIterator(bool tail) const
{
	return new ArrayListIteratorI<T>(array, tail, this);
}

//********************************************************************

template< typename T > 
int ArrayListI<T>::size() const
{
	return sentinel;
}

//********************************************************************

template< typename T > 
AbstractListI<T>* ArrayListI<T>::newList(int capacity_) const
{
	return new ArrayListI<T>(capacity_);
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#include <jlj/util/impl/ArrayList.i.h>

//********************************************************************

#endif

// eof
