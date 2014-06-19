// LinkedBlockingQueue.h

//********************************************************************

#ifndef jlj_util_concurrent_LinkedBlockingQueue_h
#define jlj_util_concurrent_LinkedBlockingQueue_h

//********************************************************************

#include <jlj/util/concurrent/BlockingQueue.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/QueueIterator.h>
using namespace jlj::util;
#include <jlj/util/concurrent/locks/ReentrantLock.h>
using namespace jlj::util::concurrent::locks;
#include <jlj/lang/UTF8String.h>
using namespace jlj::lang;
#include <jlj/core/ptypes/pasync.h>
#include <limits.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename E >
class LinkedBlockingQueueI 
	: public BlockingQueueI<E>
{
protected:
	~LinkedBlockingQueueI();
	friend class TypedReference<LinkedBlockingQueueI<E> >;

public:
	LinkedBlockingQueueI();
	LinkedBlockingQueueI(int capacity) throw (IllegalArgumentException);
	LinkedBlockingQueueI(const Collection<E>& copyfrom);

public:
	bool addAll(const Collection<E>&);
	void clear();
	bool contains(const E&) const;
	bool containsAll(const Collection<E>&) const ;
	bool isEmpty() const;
	Iterator<E> iterator() throw (UnsupportedOperationException);
	Iterator<E> iterator() const throw (UnsupportedOperationException);
	QueueIterator<E> queueIterator() throw (UnsupportedOperationException);
	bool remove(const E&);
	bool removeAll(const Collection<E>&);
	bool retainAll(const Collection<E>&);
	int size() const;

	bool add(const E&) throw (IllegalStateException);
	bool offer(const E&);
	E element() const throw (NoSuchElementException);
	E peek() const;
	E remove() throw (NoSuchElementException);
	E poll();

	int drainTo(Collection<E>);
	int drainTo(Collection<E>, int maxelements);
	bool offer(const E&, int milliseconds) throw (InterruptedException);
	void put(const E&) throw (InterruptedException);
	int remainingCapacity() const;
	E poll(int milliseconds) throw (InterruptedException);
	E take() throw (InterruptedException);

private:
	int capacity;
	LinkedList<E> list;
	Lock lock;
	pt::timedsem notfull;
	pt::timedsem notempty;
	enum { MAXIMUMCAPACITY = INT_MAX };

};

//********************************************************************

template< typename E >
struct LinkedBlockingQueue : StrongReference<LinkedBlockingQueueI<E> >
{
	StrongReferenceTemplateDerivateImplementation(LinkedBlockingQueue, E);
};

//********************************************************************
//********************************************************************

template< typename E > 
LinkedBlockingQueueI<E>::~LinkedBlockingQueueI() 
{
	capacity = 0;
	clear();
}

//********************************************************************

template< typename E > 
LinkedBlockingQueueI<E>::LinkedBlockingQueueI() 
: capacity(MAXIMUMCAPACITY)
, list(new LinkedListI<E>())
, lock(new ReentrantLockI())
, notfull(MAXIMUMCAPACITY)
, notempty(0)
{}

//********************************************************************

template< typename E > 
LinkedBlockingQueueI<E>::LinkedBlockingQueueI(int capacity) throw (IllegalArgumentException)
: capacity(capacity)
, list(new LinkedListI<E>())
, lock(new ReentrantLockI())
, notfull(capacity)
, notempty(0)
{
	if (!(0 < capacity && capacity <= MAXIMUMCAPACITY)) 
	{
		throw IllegalArgumentException(L"LinkedBlockingQueueI(int capacity) : 0 < capacity <= 32k"); 
	}
}

//********************************************************************

template< typename E > 
LinkedBlockingQueueI<E>::LinkedBlockingQueueI(const Collection<E>& copyfrom) 
: capacity(MAXIMUMCAPACITY - copyfrom->size())
, list(new LinkedListI<E>())
, lock(new ReentrantLockI())
, notfull(MAXIMUMCAPACITY - copyfrom->size())
, notempty(0)
{}

//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::addAll(const Collection<E>& copyfrom)
{
	if (0 < copyfrom->size())
	{
		Iterator<E> i = copyfrom->iterator();
		while (i->hasNext())
		{
			if (!add(i->next())) return false;
		}
	}
	return true;

}

//********************************************************************

template< typename E > 
void LinkedBlockingQueueI<E>::clear()
{
	synchronized(lock) list->clear();
}

//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::contains(const E& element) const
{
	bool ok = false;
	synchronized(lock) ok = list->contains(element);
	return ok;
}

//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::containsAll(const Collection<E>& c) const 
{
	Iterator<E> i = c->iterator();
	while (i->hasNext())
	{
		if (!contains(i->next())) return false;
	}
	return true;
}

//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::isEmpty() const
{
	return 0 == size();
}

//********************************************************************

template< typename E > 
Iterator<E> LinkedBlockingQueueI<E>::iterator() throw (UnsupportedOperationException)
{
	return new QueueIteratorI<E>(this);
}

//********************************************************************

template< typename E > 
Iterator<E> LinkedBlockingQueueI<E>::iterator() const throw (UnsupportedOperationException)
{
	return new QueueIteratorI<E>(this);
}

//********************************************************************

template< typename E > 
QueueIterator<E> LinkedBlockingQueueI<E>::queueIterator() throw (UnsupportedOperationException)
{
	return new QueueIteratorI<E>(this);
}


//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::remove(const E& element)
{
	bool ok = false;
	synchronized(lock) ok = list->remove(element);
	if (ok)   
	{
		notfull.signal();
	}
	return ok;
}

//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::removeAll(const Collection<E>& c)
{
	Iterator<E> i = c->iterator();
	while (i->hasNext())
	{
		if (!remove(i->next())) return false;
	}
	return true;
}

//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::retainAll(const Collection<E>& c)
{
	bool hasChanged = false;
	synchronized(lock)
	{
		Iterator<E> i = list->iterator();
		while (i->hasNext())
		{
			E element = i->next();
			if (!c->contains(element))
			{
				i->remove();
				hasChanged = hasChanged || true;
			}
		}
	}
	return hasChanged;
}

//********************************************************************

template< typename E > 
int LinkedBlockingQueueI<E>::size() const
{
	int s;
	synchronized(lock) s = list->size();
	return s;
}

//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::add(const E& element) throw (IllegalStateException)
{
	return offer(element, 0);
}

//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::offer(const E& element)
{
	return add(element);
}

//********************************************************************

template< typename E > 
E LinkedBlockingQueueI<E>::element() const throw (NoSuchElementException)
{
	E data;
	synchronized(lock) data = list->element();
	return data;
}

//********************************************************************

template< typename E > 
E LinkedBlockingQueueI<E>::peek() const
{
	E data;
	synchronized(lock) data = list->peek();
	return data;
}

//********************************************************************

template< typename E > 
E LinkedBlockingQueueI<E>::remove() throw (NoSuchElementException)
{
	E data;
	synchronized(lock) data = list->remove();
	notfull.signal();
	return data;
}

//********************************************************************

template< typename E > 
E LinkedBlockingQueueI<E>::poll()
{
	E data;
	try
	{
		synchronized(lock) data = list->poll();
		if (!data) 
		{
			notfull.signal();
		}
	}
	catch (...)
	{
		fprintf(stderr, "%s\n", constcharptr(UTF8String(WITHDETAILS(L"LinkedBlockingQueueI<E>::poll() : unknown exception"))));
	}
	return data;
}

//********************************************************************

template< typename E > 
int LinkedBlockingQueueI<E>::drainTo(Collection<E> copyto)
{
	int count = 0;
	synchronized(lock)
	{
		while (0 < size())
		{
			copyto->add(remove());
			count++;
		}
	}
	return count;
}

//********************************************************************

template< typename E > 
int LinkedBlockingQueueI<E>::drainTo(Collection<E> copyto, int maxelements)
{
	int count = 0;
	synchronized(lock)
	{
		while (0 < size() && count < maxelements)
		{
			copyto->add(remove());
			count++;
		}
	}
	return count;
}

//********************************************************************

template< typename E > 
bool LinkedBlockingQueueI<E>::offer(const E& element, 
									int milliseconds) throw (InterruptedException)
{
	try
	{
		if (notfull.wait(milliseconds))
		{
			lock->lock();
			list->add(element);
			lock->unlock();
			notempty.signal();
			return true;
		}
		else
		{
			return false;
		}
	}
	catch (...)
	{
		throw InterruptedException(WITHDETAILS(L"LinkedBlockingQueueI<E>::offer() : " + String(milliseconds)));
	}
}

//********************************************************************

template< typename E > 
void LinkedBlockingQueueI<E>::put(const E& element) throw (InterruptedException)
{
	try 
	{
		notfull.wait();
		lock->lock();
		list->add(element);
		lock->unlock();
		notempty.signal();
	}
	catch (...)
	{
		throw InterruptedException(WITHDETAILS(L"LinkedBlockingQueueI<E>::put()"));
	}
}

//********************************************************************

template< typename E > 
int LinkedBlockingQueueI<E>::remainingCapacity() const
{
	return capacity - size();
}

//********************************************************************

template< typename E > 
E LinkedBlockingQueueI<E>::poll(int milliseconds) throw (InterruptedException)
{
	try
	{
		if (notempty.wait(milliseconds))
		{
			lock->lock();
			E data = list->remove();
			lock->unlock();
			notfull.signal();
			return data;
		}
		else 
		{
			return E();
		}
	}
	catch (...)
	{
		throw InterruptedException(WITHDETAILS(L"LinkedBlockingQueueI<E>::poll() : " + String(milliseconds)));
	}
}

//********************************************************************

template< typename E > 
E LinkedBlockingQueueI<E>::take() throw (InterruptedException)
{
	try
	{
		notempty.wait();
		lock->lock();
		E data = list->remove();
		lock->unlock();
		notfull.signal();
		return data;
	}
	catch (...)
	{
		throw InterruptedException(WITHDETAILS(L"LinkedBlockingQueueI<E>::take()"));
	}
}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
