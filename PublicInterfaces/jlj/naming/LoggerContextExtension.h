// LoggerContextExtension.h

//********************************************************************

#ifndef jlj_naming_LoggerContextExtension_h
#define jlj_naming_LoggerContextExtension_h

//********************************************************************

#include <jlj/lang/Extension.h>
#include <jlj/lang/ClassLoader.h>
#include <jlj/naming/InitialContext.h>

using namespace jlj::lang;
using namespace jlj::util;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(naming)

//********************************************************************

class ExportedByJLJnaming LoggerContextExtensionI
	: public ExtensionI
{
public:
	~LoggerContextExtensionI();
	LoggerContextExtensionI();

public:
	Object extend(Object) throw (Exception);
};

//********************************************************************

StrongReferenceCommonDeclaration(LoggerContextExtension, ExportedByJLJnaming);
ClassLoaderInformationDeclaration(jlj::naming::LoggerContextExtension, 97405f3a63f9d4389b74b8072b4e3342, ExportedByJLJnaming); 

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
