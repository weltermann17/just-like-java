// ThreadPoolExecutor.h

//********************************************************************

#ifndef jlj_util_concurrent_ThreadPoolExecutor_h
#define jlj_util_concurrent_ThreadPoolExecutor_h

//********************************************************************

#include <jlj/util/Collections.h>
#include <jlj/util/concurrent/BlockingQueue.h>
#include <jlj/util/concurrent/AbstractExecutorService.h>
#include <jlj/util/concurrent/atomic/AtomicBoolean.h>
using namespace jlj::util::concurrent;


//********************************************************************

#include <jlj/lang/Thread.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename V >
class ThreadPoolExecutorI
	: public AbstractExecutorServiceI<V>
{
protected:
	~ThreadPoolExecutorI();
	friend class TypedReference<ThreadPoolExecutorI<V> >;

public:
	ThreadPoolExecutorI(
		int minimumpoolsize,
		int maximumpoolsize,
		int keepalivetimeinmilliseconds,
		BlockingQueue<Runnable> queue);

public:
	void execute(Runnable) throw (RejectedExecutionException, NullPointerException);

	bool awaitTermination(int milliseconds) throw (InterruptedException);
	bool isShutdown() const;
	bool isTerminated() const;
	void shutdown();
	List<Runnable> shutdownNow();

	virtual bool purge();

protected:
	BlockingQueue<Runnable> getQueue() const;
	virtual bool isTerminating() const;
	virtual bool allDone() const;

private:
	int minimumpoolsize;
	int maximumpoolsize;
	int keepalivetime;
	BlockingQueue<Runnable> queue;
	List<Future<V> > remainingfutures;
	SynchronizedList<Thread> workers;
	AtomicBoolean isshutdown;
	AtomicBoolean isterminating;
	AtomicBoolean isterminated;
};

//********************************************************************

template< typename V >
struct ThreadPoolExecutor : StrongReference<ThreadPoolExecutorI<V> > 
{
	StrongReferenceTemplateDerivateImplementation(ThreadPoolExecutor, V);
};

//********************************************************************
//********************************************************************

template< typename V >
ThreadPoolExecutorI<V>::~ThreadPoolExecutorI() 
{
	awaitTermination(1000);
}

//********************************************************************

template< typename V >
ThreadPoolExecutorI<V>::ThreadPoolExecutorI(int minimumpoolsize,
										  int maximumpoolsize,
										  int keepalivetimeinmilliseconds,
										  BlockingQueue<Runnable> queue) 
										  : minimumpoolsize(minimumpoolsize)
										  , maximumpoolsize(maximumpoolsize)
										  , keepalivetime(keepalivetimeinmilliseconds)
										  , queue(queue)
										  , remainingfutures(new LinkedListI<Future<V> >())
										  , isshutdown(false)
										  , isterminating(false)
										  , isterminated(false)
{
	List<Thread> l = new LinkedListI<Thread>();
	workers = Collections::synchronizedList<Thread>(l);

	// creates a new or modifies an existing pool

	Thread::createPool(
		minimumpoolsize,
		maximumpoolsize,
		0,
		0,
		keepalivetime,
		keepalivetime,
		0);
}

//********************************************************************

template< typename V >
void ThreadPoolExecutorI<V>::execute(Runnable runnable) throw (RejectedExecutionException, NullPointerException)
{
	if (isshutdown->get()) RejectedExecutionException(WITHDETAILS(L"Already shutting down."));
	if (!runnable) throw NullPointerException(WITHDETAILS(L"Runnable is null."));

	purge();

	try
	{
		Thread worker = new ThreadI(runnable);
		workers->add(worker);
		worker->start();
	}
	catch (const NullPointerException&)
	{
		throw RejectedExecutionException(WITHDETAILS(L"Increase maximimpoolsize and/or keepalivetime."));
	}
}

//********************************************************************

template< typename V >
bool ThreadPoolExecutorI<V>::awaitTermination(int milliseconds) throw (InterruptedException)
{
	if (isTerminated()) return true;

	List<Runnable> runnables;
	if (!isShutdown()) 
	{
		runnables = shutdownNow();
	}

	isterminating->set(true);

	StopWatch stopwatch = new StopWatchI();
	bool timedout = stopwatch->getSeconds() * 1000. >= milliseconds;

	while (!timedout && (!purge() || !allDone()))
	{
		Thread::sleep(100);
		stopwatch->stop();
		timedout = stopwatch->getSeconds() * 1000. >= milliseconds;
	}

	if (allDone() && purge())
	{
		isterminated->set(true);
		return true;
	}
	else
	{
		return false;
	}
}

//********************************************************************

template< typename V >
bool ThreadPoolExecutorI<V>::isShutdown() const
{
	return isshutdown->get();
}

//********************************************************************

template< typename V >
bool ThreadPoolExecutorI<V>::isTerminating() const
{
	return isterminating->get();
}

//********************************************************************

template< typename V >
bool ThreadPoolExecutorI<V>::isTerminated() const
{
	return isterminated->get();
}

//********************************************************************

template< typename V >
void ThreadPoolExecutorI<V>::shutdown()
{
	isshutdown->set(true);
}

//********************************************************************

template< typename V >
List<Runnable> ThreadPoolExecutorI<V>::shutdownNow()
{
	shutdown();

	List<Runnable> runnables = new LinkedListI<Runnable>();

	if (0 < queue->size())
	{
		queue->drainTo(runnables);

		Iterator<Runnable> r = runnables->iterator();
		while (r->hasNext()) 
		{ 
			RunnableFuture<V> future;
			if (r->next()->instanceOf(future))
			{
				future->cancel(true);
				remainingfutures->add(future);
			}
		}
	}

	return runnables;
}

//********************************************************************

template< typename V >
BlockingQueue<Runnable> ThreadPoolExecutorI<V>::getQueue() const
{
	return queue;
}

//********************************************************************

template< typename V >
bool ThreadPoolExecutorI<V>::allDone() const
{
	Iterator<Future<V> > f = remainingfutures->iterator();
	while (f->hasNext()) 
	{ 
		if (!f->next()->isDone()) return false;
	}
	remainingfutures->clear();
	return true;
}

//********************************************************************

template< typename V >
bool ThreadPoolExecutorI<V>::purge() 
{
	int alive = 0;
	synchronized(workers->getLock())
	{
		if (0 < workers->size())
		{
			Iterator<Thread > i = workers->iterator();
			while (i->hasNext()) 
			{ 
				if (i->next()->isAlive()) 
				{
					++alive; 
				}
				else 
				{
					i->remove();
				}
			}
		}
	}
	return 0 == alive;
}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
