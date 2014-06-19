// RunnableFuture.h

//********************************************************************

#ifndef jlj_util_concurrent_RunnableFuture_h
#define jlj_util_concurrent_RunnableFuture_h

//********************************************************************

#include <jlj/util/concurrent/Future.h>
#include <jlj/lang/Runnable.h>
using jlj::lang::Runnable;


//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename V >
class  RunnableFutureI 
	: public FutureI<V>
	, public RunnableI
{
public:
	~RunnableFutureI() = 0;
	RunnableFutureI();

};

//********************************************************************

template< typename V >
struct RunnableFuture : StrongReference<RunnableFutureI<V> > 
{
	StrongReferenceTemplateDerivateImplementation(RunnableFuture, V);
};

//********************************************************************
//********************************************************************

template< typename V > 
RunnableFutureI<V>::~RunnableFutureI()
{}

//********************************************************************

template< typename V > 
RunnableFutureI<V>::RunnableFutureI()
{}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
