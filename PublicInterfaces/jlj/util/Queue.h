// Queue.h

//********************************************************************

#ifndef Queue_h
#define Queue_h

//********************************************************************

#include <jlj/util/Collection.h>
using jlj::lang::NullPointerException;
using jlj::lang::NoSuchElementException;
using jlj::lang::IllegalStateException;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)

//********************************************************************

template< typename E >
class QueueI 
	: public virtual CollectionI<E>
{
public:
	virtual ~QueueI() = 0;
	QueueI();

public:
	// virtual bool add(const E&) = 0; throw (IllegalStateException) // not possible because already defined without in Collection
	virtual bool offer(const E&) = 0;
	virtual E element() const throw (NoSuchElementException) = 0;
	virtual E peek() const = 0;
	virtual E remove() throw (NoSuchElementException) = 0;
	virtual E poll() = 0;

};

//********************************************************************

template< typename E >
struct Queue : StrongReference<QueueI<E> >
{
	StrongReferenceTemplateDerivateImplementation(Queue, E);
};

//********************************************************************

template< typename E > 
QueueI<E>::~QueueI() 
{}

//********************************************************************

template< typename E > 
QueueI<E>::QueueI() 
{}

//********************************************************************

NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
