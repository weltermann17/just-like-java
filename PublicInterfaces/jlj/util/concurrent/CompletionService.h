// CompletionService.h

//********************************************************************

#ifndef jlj_util_concurrent_CompletionService_h
#define jlj_util_concurrent_CompletionService_h

//********************************************************************

#include <jlj/util/concurrent/ExecutorService.h>
#include <jlj/util/concurrent/RunnableFuture.h>
#include <jlj/util/concurrent/Callable.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename V >
class CompletionServiceI 
	: public ObjectI
{
public:
	~CompletionServiceI() = 0;
	CompletionServiceI();

public:
	virtual Future<V> poll() throw (InterruptedException) = 0;
	virtual Future<V> poll(int milliseconds) throw (InterruptedException) = 0;
	virtual Future<V> take() throw (InterruptedException) = 0;
	virtual RunnableFuture<V> submit(Callable<V>) throw (RejectedExecutionException, NullPointerException) = 0;
	virtual RunnableFuture<V> submit(Runnable, const V& result) throw (RejectedExecutionException, NullPointerException) = 0;

};

//********************************************************************

template< typename V >
struct CompletionService : StrongReference<CompletionServiceI<V> > 
{
	StrongReferenceTemplateDerivateImplementation(CompletionService, V);
};

//********************************************************************
//********************************************************************

template< typename V >
CompletionServiceI<V>::~CompletionServiceI()
{}

//********************************************************************

template< typename V >
CompletionServiceI<V>::CompletionServiceI()
{}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
