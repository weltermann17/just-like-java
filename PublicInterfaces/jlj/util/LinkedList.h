// LinkedList.h

//********************************************************************

#ifndef jlj_util_LinkedList_h
#define jlj_util_LinkedList_h

//********************************************************************

#include <jlj/util/AbstractList.h>
#include <jlj/util/LinkedListIterator.h>
#include <jlj/util/Queue.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename T >
class LinkedListI 
	: public AbstractListI<T>
	, public virtual QueueI<T>
{
protected:
	~LinkedListI();
	friend class TypedReference<LinkedListI<T> >;

public:
	LinkedListI();
	LinkedListI(const Collection<T>&);	

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
	bool remove(const T&); 
	T removeAt(int index) throw (IndexOutOfBoundsException);
	int size() const;

	bool offer(const T&);
	T element() const throw (NoSuchElementException);
	T peek() const;
	T remove() throw (NoSuchElementException);
	T poll();

protected:
	AbstractListI<T>* newList(int capacity) const;

private:
	friend class LinkedListIteratorI<T>;

	LinkedListNode<T>* head;
	LinkedListNode<T>* tail;
	int sentinel;

};

//********************************************************************
//********************************************************************

template< typename T >
struct LinkedList : StrongReference<LinkedListI<T> >
{
	StrongReferenceTemplateDerivateImplementation(LinkedList, T);
};

//********************************************************************

template< typename T >
LinkedListNode<T>::~LinkedListNode() 
{}

//********************************************************************

template< typename T >
LinkedListNode<T>::LinkedListNode(const T& t, LinkedListNode<T>* n, LinkedListNode<T>* p) 
: next(n)
, prev(p)
, data(t) 
{
	if (0 != next)
	{
		next->prev = this;
	}
	if (0 != prev)
	{
		prev->next = this;
	}
}

//********************************************************************
//********************************************************************

template< typename T > 
LinkedListI<T>::~LinkedListI() 
{
	clear();
}

//********************************************************************

template< typename T > 
LinkedListI<T>::LinkedListI() 
: head(0)
, tail(0)
, sentinel(0)
{}

//********************************************************************

template< typename T > 
LinkedListI<T>::LinkedListI(const Collection<T>& c) 
: head(0)
, tail(0)
, sentinel(0)
{
	this->addAll(c);
}

//********************************************************************

template< typename T > 
Object LinkedListI<T>::clone() const 
{
	List<T> c = new LinkedListI<T>();

	Iterator<T> i = iterator();
	while (i->hasNext())
	{
		c->add(i->next());
	}

	return c;
}

//********************************************************************

template< typename T > 
bool LinkedListI<T>::add(const T& element)  
{
	if (0 != tail)
	{
		tail = new LinkedListNode<T>(element, 0, tail);
	}
	else
	{
		head = tail = new LinkedListNode<T>(element, 0, 0);
	}

	sentinel++;
	return true;
}

//********************************************************************

template< typename T > 
void LinkedListI<T>::add(int index, const T& element) throw (IndexOutOfBoundsException) 
{
	if ((0 > index) || (index > sentinel)) throw IndexOutOfBoundsException(WITHDETAILS(L""));

	if (index < sentinel)
	{
		LinkedListNode<T>* node = head;
		for (int i = 0; i < index; ++i) 
		{
			node = node->next;
		}	
		if (0 == index)
		{
			head = new LinkedListNode<T>(element, node, 0 != node ? node->prev : 0);
			if (0 == tail) tail = head;
		}
		else
		{
			new LinkedListNode<T>(element, node, 0 != node ? node->prev : 0);
		}
		sentinel++;
	}
	else
	{
		add(element);
	}
}

//********************************************************************

template< typename T > 
void LinkedListI<T>::clear()
{
	while (0 != head)
	{
		LinkedListNode<T>* helper = head;	
		head = head->next;
		delete helper;
	}
	head = tail = 0;
	sentinel = 0;
}

//********************************************************************

template< typename T > 
T LinkedListI<T>::get(int index) const throw (IndexOutOfBoundsException) 
{
	if ((0 > index) || (index >= sentinel)) throw IndexOutOfBoundsException(WITHDETAILS(L""));
	LinkedListNode<T>* node = head;
	for (int i = 0; i < index; ++i) 
	{
		node = node->next;
	}
	return node->data;
}

//********************************************************************

template< typename T > 
bool LinkedListI<T>::remove(const T& element)
{
	int index = indexOf(element);
	if (-1 < index)
	{
		removeAt(index);
		return true;
	}
	else
		return false;
}

//********************************************************************

template< typename T > 
T LinkedListI<T>::removeAt(int index) throw (IndexOutOfBoundsException)
{
	if ((0 > index) || (index >= sentinel)) 
	{
		throw IndexOutOfBoundsException(WITHDETAILS(L"index = " + String(index) + L", size = " + String(sentinel)));
	}
	ASSERT((0 <= index) && (index < sentinel));
	LinkedListNode<T>* node = head;
	for (int i = 0; i < index; ++i) 
	{
		node = node->next;
	}
	if (0 != node->next)
	{
		node->next->prev = node->prev;
	}
	if (0 != node->prev)
	{
		node->prev->next = node->next;
	}
	if (0 == index)
	{
		head = node->next;
	}
	if (sentinel - 1 == index)
	{
		tail = node->prev;
	}

	--sentinel;	
	T data = node->data;
	delete node;
	return data;
}

//********************************************************************

template< typename T > 
Iterator<T> LinkedListI<T>::iterator()  
{
	return new LinkedListIteratorI<T>(head, this);
}

//********************************************************************

template< typename T > 
Iterator<T> LinkedListI<T>::iterator() const
{
	return new LinkedListIteratorI<T>(head, this);
}

//********************************************************************

template< typename T > 
ListIterator<T> LinkedListI<T>::listIterator(bool startattail)  
{
	return new LinkedListIteratorI<T>(
		startattail ? tail : head, this);
}

//********************************************************************

template< typename T > 
ListIterator<T> LinkedListI<T>::listIterator(bool startattail) const
{
	return new LinkedListIteratorI<T>(
		startattail ? tail : head, this);
}

//********************************************************************

template< typename T > 
int LinkedListI<T>::size() const
{
	return sentinel;
}

//********************************************************************

template< typename T > 
AbstractListI<T>* LinkedListI<T>::newList(int capacity) const
{
	return new LinkedListI<T>();
}


//********************************************************************
//********************************************************************

template< typename T > 
bool LinkedListI<T>::offer(const T& element)
{
	return add(element);
}

//********************************************************************

template< typename T > 
T LinkedListI<T>::element() const throw (NoSuchElementException)
{
	if (0 != head)
	{
		return head->data;
	}
	else
	{
		throw NoSuchElementException(L"LinkedListI<T>::element()");
	}
}

//********************************************************************

template< typename T > 
T LinkedListI<T>::peek() const
{
	return 0 != head ? head->data : T();
}

//********************************************************************

template< typename T > 
T LinkedListI<T>::poll() 
{
	return 0 != head ? removeAt(0) : T();
}

//********************************************************************

template< typename T > 
T LinkedListI<T>::remove() throw (NoSuchElementException)
{
	if (0 != head)
	{
		return removeAt(0);
	}
	else
	{
		throw NoSuchElementException(L"LinkedListI<T>::remove()");
	}
}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
