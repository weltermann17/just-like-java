// BlockingQueue.h

//********************************************************************

#ifndef jlj_util_concurrent_BlockingQueue_h
#define jlj_util_concurrent_BlockingQueue_h

//********************************************************************

#include <jlj/lang/Throwable.h>
using namespace jlj::lang;
#include <jlj/util/Queue.h>
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename E >
class BlockingQueueI 
	: public QueueI<E>
{
public:
	~BlockingQueueI() = 0;
	BlockingQueueI();

public:
	virtual bool add(const E&) throw (IllegalStateException) = 0;
	virtual bool offer(const E&) = 0;
	virtual E element() const throw (NoSuchElementException) = 0;
	virtual E peek() const = 0;
	virtual E remove() throw (NoSuchElementException) = 0;
	virtual E poll() = 0;

	virtual int drainTo(Collection<E>) = 0;
	virtual int drainTo(Collection<E>, int maxelements) = 0;
	virtual bool offer(const E&, int milliseconds) throw (InterruptedException) = 0;
	virtual void put(const E&) throw (InterruptedException) = 0;
	virtual int remainingCapacity() const = 0;
	virtual E poll(int milliseconds) throw (InterruptedException) = 0;
	virtual E take() throw (InterruptedException) = 0;

};

//********************************************************************

template< typename E >
struct BlockingQueue : StrongReference<BlockingQueueI<E> >
{
	StrongReferenceTemplateDerivateImplementation(BlockingQueue, E);
};

//********************************************************************

template< typename E > 
BlockingQueueI<E>::~BlockingQueueI() 
{}

//********************************************************************

template< typename E > 
BlockingQueueI<E>::BlockingQueueI() 
{}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
