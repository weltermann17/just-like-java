// Future.h

//********************************************************************

#ifndef jlj_util_concurrent_Future_h
#define jlj_util_concurrent_Future_h

//********************************************************************

#include <jlj/lang/Object.h>
#include <jlj/lang/Class.h>
using jlj::lang::ObjectI;
using jlj::lang::Object;
#include <jlj/lang/Throwable.h>
using jlj::lang::InterruptedException;
#include <jlj/util/concurrent/Exceptions.h>
#include <JLJutil.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename V >
class FutureI 
	: public virtual ObjectI
{
public:
	~FutureI() = 0;
	FutureI();

public:
	virtual bool cancel(bool mayinterruptifrunning) = 0;
	virtual V get() throw (InterruptedException, ExecutionException, CancellationException) = 0;
	virtual V get(int milliseconds) throw (InterruptedException, ExecutionException, CancellationException, TimeoutException) = 0;
	virtual bool isCancelled() const = 0;
	virtual bool isDone() const = 0;

};

//********************************************************************

template< typename V >
struct Future : StrongReference<FutureI<V> > 
{
	StrongReferenceTemplateDerivateImplementation(Future, V);
};

//********************************************************************
//********************************************************************

template< typename V > 
FutureI<V>::~FutureI()
{}

//********************************************************************

template< typename V > 
FutureI<V>::FutureI()
{}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
