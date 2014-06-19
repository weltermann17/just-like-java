// Executor.h

//********************************************************************

#ifndef jlj_util_concurrent_Executor_h
#define jlj_util_concurrent_Executor_h

//********************************************************************

#include <jlj/lang/Object.h>
using jlj::lang::ObjectI;
using jlj::lang::Object;
#include <jlj/lang/Throwable.h>
using jlj::lang::NullPointerException;
#include <jlj/lang/Runnable.h>
using jlj::lang::Runnable;
#include <jlj/util/concurrent/Exceptions.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

class ExportedByJLJutil ExecutorI 
	: public ObjectI
{
public:
	~ExecutorI() = 0;
	ExecutorI();

public:
	virtual void execute(Runnable) throw (RejectedExecutionException, NullPointerException) = 0;

};

//********************************************************************

StrongReferenceCommonDeclaration(Executor, ExportedByJLJutil); 

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
