// Exceptions.h

//********************************************************************

#ifndef jlj_naming_Exceptions_h
#define jlj_naming_Exceptions_h

//********************************************************************

#include <JLJnaming.h>
#include <jlj/lang/Throwable.h>

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(naming)

//********************************************************************

DeriveException(ExportedByJLJnaming, NamingException, jlj::lang::Exception);
DeriveException(ExportedByJLJnaming, NameAlreadyBoundException, jlj::naming::NamingException);
DeriveException(ExportedByJLJnaming, NameNotFoundException, jlj::naming::NamingException);

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
