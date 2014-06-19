// ExecutorCompletionService.h

//********************************************************************

#ifndef jlj_util_concurrent_ExecutorCompletionService_h
#define jlj_util_concurrent_ExecutorCompletionService_h

//********************************************************************

#include <jlj/util/concurrent/CompletionService.h>
#include <jlj/util/concurrent/Executor.h>
#include <jlj/util/concurrent/LinkedBlockingQueue.h>
#include <jlj/util/concurrent/FutureTask.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename V > struct ExecutorCompletionService;

//********************************************************************

template< typename V >
class ExecutorCompletionServiceI 
	: public CompletionServiceI<V>
{
protected:
	~ExecutorCompletionServiceI();
	friend class TypedReference<ExecutorCompletionServiceI<V> >;

public:
	ExecutorCompletionServiceI(Executor);
	ExecutorCompletionServiceI(Executor, BlockingQueue<Future<V> >);

public:
	Future<V> poll() throw (InterruptedException);
	Future<V> poll(int milliseconds) throw (InterruptedException);
	Future<V> take() throw (InterruptedException);
	RunnableFuture<V> submit(Callable<V>) throw (RejectedExecutionException, NullPointerException);
	RunnableFuture<V> submit(Runnable, const V& result) throw (RejectedExecutionException, NullPointerException);

private:
	Executor executor;
	BlockingQueue<Future<V> > queue;

	class QueueingFutureI 
		: public FutureTaskI<V>
	{
	public:
		QueueingFutureI(
			Callable<V> callable, 
			ExecutorCompletionService<V> owner) throw (NullPointerException)
			: FutureTaskI<V>(callable)
			, owner(owner) { if (!callable) throw NullPointerException(L"QueueingFuture(callable)"); }

		QueueingFutureI(
			Runnable runnable, 
			const V& result,
			ExecutorCompletionService<V> owner) throw (NullPointerException)
			: FutureTaskI<V>(runnable, result)
			, owner(owner) { if (!runnable) throw NullPointerException(L"QueueingFuture(runnable)"); }

		bool done() 
		{ 
			ExecutorCompletionService<V> owner = this->owner;
			if (!!owner) owner->queue->put(this); 
			return true; 
		}

	private:
		WeakReference<ExecutorCompletionServiceI<V> > owner;

	};
};

//********************************************************************

template< typename V >
struct ExecutorCompletionService : StrongReference<ExecutorCompletionServiceI<V> >
{
	StrongReferenceTemplateDerivateImplementation(ExecutorCompletionService, V);
};

//********************************************************************
//********************************************************************

template< typename V >
ExecutorCompletionServiceI<V>::~ExecutorCompletionServiceI()
{}

//********************************************************************

template< typename V >
ExecutorCompletionServiceI<V>::ExecutorCompletionServiceI(Executor executor)
: executor(executor)
, queue(new LinkedBlockingQueueI<Future<V> >())
{}

//********************************************************************

template< typename V >
ExecutorCompletionServiceI<V>::ExecutorCompletionServiceI(Executor executor,
														  BlockingQueue<Future<V> > queue)
														  : executor(executor)
														  , queue(queue)
{}

//********************************************************************

template< typename V >
Future<V> ExecutorCompletionServiceI<V>::poll() throw (InterruptedException)
{
	return queue->poll();
}

//********************************************************************

template< typename V >
Future<V> ExecutorCompletionServiceI<V>::poll(int milliseconds) throw (InterruptedException)
{
	return queue->poll(milliseconds);
}

//********************************************************************

template< typename V >
Future<V> ExecutorCompletionServiceI<V>::take() throw (InterruptedException)
{
	return queue->take();
}

//********************************************************************

template< typename V >
RunnableFuture<V> ExecutorCompletionServiceI<V>::submit(Callable<V> callable) throw (RejectedExecutionException, NullPointerException)
{
	RunnableFuture<V> f = new QueueingFutureI(callable, this);
	executor->execute(f);
	return f;
}

//********************************************************************

template< typename V >
RunnableFuture<V> ExecutorCompletionServiceI<V>::submit(Runnable runnable, const V& result) throw (RejectedExecutionException, NullPointerException)
{
	RunnableFuture<V> f = new QueueingFutureI(runnable, result, this);
	executor->execute(f);
	return f;
}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
