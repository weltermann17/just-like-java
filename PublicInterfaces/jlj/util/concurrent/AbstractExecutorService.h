// AbstractExecutorService.h

//********************************************************************

#ifndef jlj_util_concurrent_AbstractExecutorService_h
#define jlj_util_concurrent_AbstractExecutorService_h

//********************************************************************

#include <jlj/util/concurrent/ExecutorService.h>
#include <jlj/util/concurrent/ExecutorCompletionService.h>
#include <jlj/util/concurrent/FutureTask.h>
#include <jlj/util/StopWatch.h>
#include <jlj/util/ArrayList.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename V >
class AbstractExecutorServiceI
	: public ExecutorServiceI<V>
{
public:
	~AbstractExecutorServiceI() = 0;
	AbstractExecutorServiceI();

public:
	List<Future<V> > invokeAll(const Collection<Callable<V> >&) throw (RejectedExecutionException, InterruptedException, NullPointerException, IllegalArgumentException);
	List<Future<V> > invokeAll(const Collection<Callable<V> >&, int milliseconds) throw (RejectedExecutionException, InterruptedException, NullPointerException, IllegalArgumentException);
	V invokeAny(const Collection<Callable<V> >&) throw (RejectedExecutionException, InterruptedException, ExecutionException, NullPointerException, IllegalArgumentException);
	V invokeAny(const Collection<Callable<V> >&, int milliseconds) throw (RejectedExecutionException, InterruptedException, ExecutionException, TimeoutException, NullPointerException, IllegalArgumentException);
	Future<V> submit(Callable<V>) throw (RejectedExecutionException, NullPointerException);
	Future<V> submit(Runnable) throw (RejectedExecutionException, NullPointerException);
	Future<V> submit(Runnable, const V& result) throw (RejectedExecutionException, NullPointerException);

protected:
	RunnableFuture<V> newTaskFor(Callable<V>) throw (NullPointerException);
	RunnableFuture<V> newTaskFor(Runnable, const V& result) throw (NullPointerException);
	friend class ExecutorCompletionService<V>;

	virtual BlockingQueue<Runnable> getQueue() const = 0;
	virtual bool isTerminating() const = 0;
	virtual V invokeAny(const Collection<Callable<V> >&, bool withtimeout, int milliseconds) throw (RejectedExecutionException, InterruptedException, TimeoutException, ExecutionException, NullPointerException, IllegalArgumentException);

};

//********************************************************************
//********************************************************************

template< typename V >
AbstractExecutorServiceI<V>::~AbstractExecutorServiceI() 
{}

//********************************************************************

template< typename V >
AbstractExecutorServiceI<V>::AbstractExecutorServiceI() 
{}

//********************************************************************

template< typename V >
List<Future<V> > AbstractExecutorServiceI<V>::invokeAll(const Collection<Callable<V> >& callables) 
throw (RejectedExecutionException, InterruptedException, NullPointerException, IllegalArgumentException)
{
	if (!callables) throw NullPointerException(WITHDETAILS(L"callables is null"));
	if (callables->isEmpty()) throw IllegalArgumentException(WITHDETAILS(L"callables is empty"));
	if (isShutdown()) throw RejectedExecutionException(WITHDETAILS(L"Is already shut down."));

	bool done = false;
	List<Future<V> > futures = new ArrayListI<Future<V> >(callables->size());

	try 
	{
		Iterator<Callable<V> > c = callables->iterator(); 
		while (c->hasNext())
		{
			if (isTerminating()) throw RejectedExecutionException(WITHDETAILS(L"Is terminating."));
			RunnableFuture<V> f = newTaskFor(c->next());
			futures->add(f);
			execute(f); 
			getQueue()->put(f);
		}

		while (0 < getQueue()->size())
		{
			if (isTerminating()) throw RejectedExecutionException(WITHDETAILS(L"Is terminating."));
			execute(getQueue()->take());
		}

		Iterator<Future<V> > f = futures->iterator(); 
		while (f->hasNext())
		{
			if (isTerminating()) throw RejectedExecutionException(WITHDETAILS(L"Is terminating."));
			Future<V> current = f->next();
			if (!current->isDone())
			{
				try { current->get(); } 
				catch (const CancellationException&) {}
				catch (const ExecutionException&) {}
			}
		}

		done = true;
		return futures;
	}
	catch (const jlj::lang::Exception&)
	{
		if (!done)
		{
			Iterator<Future<V> > f = futures->iterator(); 
			while (f->hasNext()) f->next()->cancel(true);
		}
		throw;
	}
}

//********************************************************************

template< typename V >
List<Future<V> > AbstractExecutorServiceI<V>::invokeAll(const Collection<Callable<V> >& callables, 
														int milliseconds) 
														throw (RejectedExecutionException, 
														InterruptedException, 
														NullPointerException, 
														IllegalArgumentException)
{
	if (!callables) throw NullPointerException(L"callables is null");
	if (callables->isEmpty()) throw IllegalArgumentException(L"callables is empty");
	if (isShutdown()) throw RejectedExecutionException(WITHDETAILS(L"Is already shut down."));

	bool done = false;
	List<Future<V> > futures = new ArrayListI<Future<V> >(callables->size());

	StopWatch stopwatch = new StopWatchI();
	bool timedout = stopwatch->getSeconds() * 1000. >= milliseconds;

	try 
	{
		Iterator<Callable<V> > c = callables->iterator(); 
		while (c->hasNext() && !timedout)
		{
			if (isTerminating()) throw RejectedExecutionException(WITHDETAILS(L"Is terminating."));
			RunnableFuture<V> f = newTaskFor(c->next());
			futures->add(f);
			getQueue()->offer(f, milliseconds);

			stopwatch->stop();
			timedout = stopwatch->getSeconds() * 1000. >= milliseconds;
		}


		while (0 < getQueue()->size() && !timedout)
		{
			if (isTerminating()) throw RejectedExecutionException(WITHDETAILS(L"Is terminating."));
			execute(getQueue()->take());

			stopwatch->stop();
			timedout = stopwatch->getSeconds() * 1000. >= milliseconds;
		}

		Iterator<Future<V> > f = futures->iterator(); 
		while (f->hasNext() && !timedout)
		{
			if (isTerminating()) throw RejectedExecutionException(WITHDETAILS(L"Is terminating."));
			Future<V> current = f->next();
			if (!current->isDone())
			{
				try { current->get(milliseconds); } 
				catch (const CancellationException&) {}
				catch (const ExecutionException&) {}
			}
		}

		if (timedout)
		{
			Iterator<Future<V> > f = futures->iterator(); 
			while (f->hasNext())
			{
				Future<V> current = f->next();
				if (!current->isDone()) current->cancel(true);
			}
		}
		else
		{
			done = true;
		}
		return futures;
	}
	catch (const jlj::lang::Exception&)
	{
		if (!done)
		{
			Iterator<Future<V> > f = futures->iterator(); 
			while (f->hasNext()) f->next()->cancel(true);
		}
		throw;
	}
}

//********************************************************************

template< typename V >
V AbstractExecutorServiceI<V>::invokeAny(const Collection<Callable<V> >& callables) 
throw (RejectedExecutionException, InterruptedException, ExecutionException, NullPointerException, IllegalArgumentException)
{
	return invokeAny(callables, false, 0);
}

//********************************************************************

template< typename V >
V AbstractExecutorServiceI<V>::invokeAny(const Collection<Callable<V> >& callables, 
										 int milliseconds) 
										 throw (RejectedExecutionException, 
										 InterruptedException, 
										 ExecutionException, 
										 TimeoutException, 
										 NullPointerException, 
										 IllegalArgumentException)
{
	return invokeAny(callables, true, milliseconds);
}

//********************************************************************

template< typename V >
V AbstractExecutorServiceI<V>::invokeAny(const Collection<Callable<V> >& callables,
										 bool withtimeout,
										 int milliseconds) 
										 throw (RejectedExecutionException, 
										 InterruptedException, 
										 TimeoutException, 
										 ExecutionException, 
										 NullPointerException, 
										 IllegalArgumentException)
{
	if (!callables) throw NullPointerException(WITHDETAILS(L"callables is null"));
	if (callables->isEmpty()) throw IllegalArgumentException(WITHDETAILS(L"callables is empty"));
	if (isShutdown()) throw RejectedExecutionException(WITHDETAILS(L"Is already shut down."));

	List<Future<V> > futures = new LinkedListI<Future<V> >();

	try
	{
		bool any = false;
		Future<V> f;
		CompletionService<V> completionservice = new ExecutorCompletionServiceI<V>(this);

		int active = 0;
		int jobs = callables->size();	
		Iterator<Callable<V> > c = callables->iterator(); 
		if (c->hasNext()) futures->add(completionservice->submit(c->next()));
		--jobs;
		++active;

		while (true)
		{
			if (isTerminating()) throw RejectedExecutionException(WITHDETAILS(L"Is terminating."));

			f = completionservice->poll();

			if (!f)
			{
				if (0 < jobs) 
				{
					--jobs;
					if (c->hasNext()) futures->add(completionservice->submit(c->next()));
					++active;
				}
				else if (0 == active)
				{
					throw ExecutionException(WITHDETAILS(L"AbstractExecutorServiceI<V>::invokeAny() no result"));
				}
				else if (withtimeout) 
				{
					f = completionservice->poll(milliseconds);
					if (!f)	throw TimeoutException(WITHDETAILS(L"AbstractExecutorServiceI<V>::invokeAny()"));
					break;
				}
				else
				{
					f = completionservice->take();
					break;
				}
			}
			else
			{
				break;
			}
		}

		Iterator<Future<V> > i = futures->iterator(); 
		while (i->hasNext()) i->next()->cancel(true);

		return f->get();
	}
	catch (const jlj::lang::Exception&)
	{
		Iterator<Future<V> > i = futures->iterator(); 
		while (i->hasNext()) i->next()->cancel(true);
		throw;
	}
}

//********************************************************************

template< typename V >
Future<V> AbstractExecutorServiceI<V>::submit(Callable<V> callable) 
throw (RejectedExecutionException, NullPointerException)
{
	if (isShutdown()) throw RejectedExecutionException(WITHDETAILS(L"Is already shut down."));
	RunnableFuture<V> f = newTaskFor(callable);
	execute(f);
	return f;
}

//********************************************************************

template< typename V >
Future<V> AbstractExecutorServiceI<V>::submit(Runnable runnable) 
throw (RejectedExecutionException, NullPointerException)
{
	if (isShutdown()) throw RejectedExecutionException(WITHDETAILS(L"Is already shut down."));
	RunnableFuture<V> f = newTaskFor(runnable, V());
	execute(f);
	return f;
}

//********************************************************************

template< typename V >
Future<V> AbstractExecutorServiceI<V>::submit(Runnable runnable, 
											  const V& result) 
											  throw (RejectedExecutionException, NullPointerException)
{
	if (isShutdown()) throw RejectedExecutionException(WITHDETAILS(L"Is already shut down."));
	RunnableFuture<V> f = newTaskFor(runnable, result);
	execute(f);
	return f;
}

//********************************************************************

template< typename V >
RunnableFuture<V> AbstractExecutorServiceI<V>::newTaskFor(Callable<V> callable) throw (NullPointerException)
{
	return new FutureTaskI<V>(callable);
}

//********************************************************************

template< typename V >
RunnableFuture<V> AbstractExecutorServiceI<V>::newTaskFor(Runnable runnable, 
														  const V& result) 
														  throw (NullPointerException)
{
	return new FutureTaskI<V>(runnable, result);
}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
