// Exceptions.h

//********************************************************************

#ifndef jlj_net_Exceptions_h
#define jlj_net_Exceptions_h

//********************************************************************

#include <jlj/lang/Throwable.h>
#include <jlj/io/Exceptions.h>
using jlj::io::IOException;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(net)

//********************************************************************

DeriveException(ExportedByJLJnet, UnknownHostException, jlj::io::IOException);
DeriveException(ExportedByJLJnet, SocketException, jlj::io::IOException);
DeriveException(ExportedByJLJnet, SocketTimeoutException, jlj::io::InterruptedIOException);

//********************************************************************

NAMESPACE_END(net)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
