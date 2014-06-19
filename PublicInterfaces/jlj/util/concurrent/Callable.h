// Callable.h

//********************************************************************

#ifndef jlj_util_concurrent_Callable_h
#define jlj_util_concurrent_Callable_h

//********************************************************************

#include <jlj/lang/Object.h>
using jlj::lang::ObjectI;
using jlj::lang::Object;
#include <jlj/lang/Throwable.h>
using jlj::lang::Exception;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

template< typename V >
class CallableI 
	: public virtual ObjectI
{
public:
	~CallableI() = 0;
	CallableI();

public:
	virtual V call() throw (jlj::lang::Exception) = 0;

};

//********************************************************************

template< typename V >
struct Callable : StrongReference<CallableI<V> > 
{
	StrongReferenceTemplateDerivateImplementation(Callable, V);
};

//********************************************************************
//********************************************************************

template< typename V > 
CallableI<V>::~CallableI()
{}

//********************************************************************

template< typename V > 
CallableI<V>::CallableI()
{}

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
