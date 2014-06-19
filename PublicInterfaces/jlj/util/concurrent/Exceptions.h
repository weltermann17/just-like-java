// Exceptions.h

//********************************************************************

#ifndef jlj_util_concurrent_Exceptions_h
#define jlj_util_concurrent_Exceptions_h

//********************************************************************

#include <jlj/lang/Throwable.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(concurrent)

//********************************************************************

DeriveException(ExportedByJLJutil, ExecutionException, jlj::lang::Exception);
DeriveException(ExportedByJLJutil, TimeoutException, jlj::lang::Exception);
DeriveException(ExportedByJLJutil, CancellationException, jlj::lang::IllegalStateException);
DeriveException(ExportedByJLJutil, BrokenBarrierException, jlj::lang::Exception);
DeriveException(ExportedByJLJutil, RejectedExecutionException, jlj::lang::RuntimeException);

//********************************************************************

NAMESPACE_END(concurrent)
NAMESPACE_END(util)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
