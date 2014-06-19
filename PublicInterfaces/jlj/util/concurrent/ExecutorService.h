// ExecutorService.h

//********************************************************************

#ifndef jlj_util_concurrent_ExecutorService_h
#define jlj_util_concurrent_ExecutorService_h

//********************************************************************

#include <jlj/util/concurrent/Executor.h>
#include <jlj/util/concurrent/Future.h>
#include <jlj/util/concurrent/Callable.h>
#include <jlj/util/Collection.h>
#include <jlj/util/List.h>
using jlj::util::Collection;
using jlj::util::List;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename V >
class ExecutorServiceI 
	: public ExecutorI
{
public:
	~ExecutorServiceI() = 0;
	ExecutorServiceI();

public:
	virtual bool awaitTermination(int milliseconds) throw (InterruptedException) = 0;
	virtual List<Future<V> > invokeAll(const Collection<Callable<V> >&) throw (RejectedExecutionException, InterruptedException, NullPointerException, IllegalArgumentException) = 0;
	virtual List<Future<V> > invokeAll(const Collection<Callable<V> >&, int milliseconds) throw (RejectedExecutionException, InterruptedException, NullPointerException, IllegalArgumentException) = 0;
	virtual V invokeAny(const Collection<Callable<V> >&) throw (RejectedExecutionException, InterruptedException, ExecutionException, NullPointerException, IllegalArgumentException) = 0;
	virtual V invokeAny(const Collection<Callable<V> >&, int milliseconds) throw (RejectedExecutionException, InterruptedException, ExecutionException, TimeoutException, NullPointerException, IllegalArgumentException) = 0;
	virtual bool isShutdown() const = 0;
	virtual bool isTerminated() const = 0;
	virtual void shutdown() = 0;
	virtual List<Runnable> shutdownNow() = 0;
	virtual Future<V> submit(Callable<V>) throw (RejectedExecutionException, NullPointerException) = 0;
	virtual Future<V> submit(Runnable) throw (RejectedExecutionException, NullPointerException) = 0;
	virtual Future<V> submit(Runnable, const V& result) throw (RejectedExecutionException, NullPointerException) = 0;

};

//********************************************************************

template< typename V >
struct ExecutorService : StrongReference<ExecutorServiceI<V> > 
{
	StrongReferenceTemplateDerivateImplementation(ExecutorService, V);
};

//********************************************************************
//********************************************************************

template< typename V >
ExecutorServiceI<V>::~ExecutorServiceI()
{}

//********************************************************************

template< typename V >
ExecutorServiceI<V>::ExecutorServiceI()
{}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
