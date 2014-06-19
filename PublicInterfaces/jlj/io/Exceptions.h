// Exceptions.h

//********************************************************************

#ifndef jlj_io_Exceptions_h
#define jlj_io_Exceptions_h

//********************************************************************

#include <jlj/lang/Throwable.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(io)

//********************************************************************

DeriveException(ExportedByJLJio, IOException, jlj::lang::Exception);
DeriveException(ExportedByJLJio, InterruptedIOException, jlj::io::IOException);

//********************************************************************

NAMESPACE_END(io)
NAMESPACE_END(jlj)

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

DeriveException(ExportedByJLJio, CancelledKeyException, jlj::lang::IllegalStateException);
DeriveException(ExportedByJLJio, ClosedSelectorException, jlj::lang::IllegalStateException);

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
