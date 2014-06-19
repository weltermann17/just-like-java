// FutureTask.h

//********************************************************************

#ifndef jlj_util_concurrent_FutureTask_h
#define jlj_util_concurrent_FutureTask_h

//********************************************************************

#include <jlj/util/concurrent/RunnableFuture.h>
#include <jlj/util/concurrent/Callable.h>
#include <jlj/util/concurrent/LinkedBlockingQueue.h>
#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

using namespace jlj::util::concurrent::atomic;
#include <jlj/lang/Thread.h>


//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename V >
class FutureTaskI 
	: public RunnableFutureI<V>
{
protected:
	~FutureTaskI();
	friend class TypedReference<FutureTaskI>;

public:
	FutureTaskI(Callable<V> callable) throw (NullPointerException);	
	FutureTaskI(Runnable runnable, const V& result) throw (NullPointerException);

public:
	bool cancel(bool mayinterruptifrunning);
	V get() throw (InterruptedException, ExecutionException, CancellationException);
	V get(int milliseconds) throw (InterruptedException, ExecutionException, CancellationException, TimeoutException);
	bool isCancelled() const;
	bool isDone() const;
	void set(const V&);

	void run();

public:
	virtual bool done();

private:
	Callable<V> callable;
	Runnable runnable;
	V result;
	BlockingQueue<V> queue;
	AtomicBoolean isdone;
	AtomicBoolean iscancelled;

};

//********************************************************************

template< typename V >
struct FutureTask : StrongReference<FutureTaskI<V> > 
{
	StrongReferenceTemplateDerivateImplementation(FutureTask, V);
};

//********************************************************************
//********************************************************************

template< typename V > 
FutureTaskI<V>::~FutureTaskI()
{}

//********************************************************************

template< typename V > 
FutureTaskI<V>::FutureTaskI(Callable<V> callable) throw (NullPointerException)
: queue(new LinkedBlockingQueueI<V>(1))
, callable(callable)
, isdone(false)
, iscancelled(false)
{
	if (!callable) throw NullPointerException(L"FutureTaskI<V>::FutureTaskI(callable)");
}

//********************************************************************

template< typename V > 
FutureTaskI<V>::FutureTaskI(Runnable runnable, const V& result) throw (NullPointerException)
: queue(new LinkedBlockingQueueI<V>(1))
, runnable(runnable)
, result(result)
, isdone(false)
, iscancelled(false)
{
	if (!runnable) throw NullPointerException(L"FutureTaskI<V>::FutureTaskI(runnable, result)");
}

//********************************************************************

template< typename V > 
bool FutureTaskI<V>::cancel(bool mayinterruptifrunning)
{
	if (isDone()) return false;
	if (mayinterruptifrunning)
	{
		currentThread()->interrupt();
	}
	isdone->set(true);
	iscancelled->set(true);
	return true;
}

//********************************************************************

template< typename V > 
V FutureTaskI<V>::get() throw (InterruptedException, ExecutionException, CancellationException)
{
	return queue->take();
}

//********************************************************************

template< typename V > 
V FutureTaskI<V>::get(int milliseconds) throw (InterruptedException, ExecutionException, CancellationException, TimeoutException)
{
	if (isCancelled()) throw CancellationException(WITHDETAILS(L"FutureTaskI<V>::get(int milliseconds)"));
	return queue->poll(milliseconds);
}

//********************************************************************

template< typename V > 
bool FutureTaskI<V>::isCancelled() const
{
	return iscancelled->get();
}

//********************************************************************

template< typename V > 
bool FutureTaskI<V>::isDone() const
{
	return isdone->get();
}

//********************************************************************

template< typename V > 
void FutureTaskI<V>::run() 
{
	if (!isDone())
	{
		if (!currentThread()->isInterrupted())
		{
			try 
			{
				if (!!runnable)
				{
					runnable->run();
					queue->put(result);
				}
				else if (!!callable)
				{
					queue->put(callable->call());
				}

				isdone->set(done()); 
			}
			catch (const jlj::lang::Exception& e)
			{
				Logger::getLogger(Logger::GLOBAL_LOGGER_NAME)->log(Level::SEVERE, L"FutureTaskI<V>::run() : " + e->toString());
				isdone->set(true);
				iscancelled->set(true);
			}
			catch (...)
			{
				Logger::getLogger(Logger::GLOBAL_LOGGER_NAME)->log(Level::SEVERE, L"FutureTaskI<V>::run() : Unknown exception");
				isdone->set(true);
				iscancelled->set(true);
			}
		}
		else
		{
			Logger::getLogger(Logger::GLOBAL_LOGGER_NAME)->log(Level::SEVERE, L"FutureTaskI<V>::run() : isInterrupted() is true");
			isdone->set(true);
			iscancelled->set(true);
		}
	}
	else
	{
		Logger::getLogger(Logger::GLOBAL_LOGGER_NAME)->log(Level::WARNING, L"FutureTaskI<V>::run() : isDone() is true");
	}
}

//********************************************************************

template< typename V > 
bool FutureTaskI<V>::done()
{
	return true;
}

//********************************************************************

template< typename V > 
void FutureTaskI<V>::set(const V& result)
{
	this->result = result;
}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
