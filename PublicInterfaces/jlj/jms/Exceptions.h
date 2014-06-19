// JMSException.h

//********************************************************************

#ifndef jlj_jms_JMSException_h
#define jlj_jms_JMSException_h

//********************************************************************

#include <jlj/lang/Throwable.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

DeriveException(ExportedByJLJjms, JMSException, jlj::lang::Exception);
DeriveException(ExportedByJLJjms, JMSRegistrationFailedException, JMSException);

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
