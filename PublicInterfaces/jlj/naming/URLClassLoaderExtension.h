// URLClassLoaderExtension.h

//********************************************************************

#ifndef jlj_naming_URLClassLoaderExtension_h
#define jlj_naming_URLClassLoaderExtension_h

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

class ExportedByJLJnaming URLClassLoaderExtensionI
	: public ExtensionI
{
public:
	~URLClassLoaderExtensionI();
	URLClassLoaderExtensionI();

public:
	Object extend(Object) throw (Exception);
};

//********************************************************************

StrongReferenceCommonDeclaration(URLClassLoaderExtension, ExportedByJLJnaming);
ClassLoaderInformationDeclaration(jlj::naming::URLClassLoaderExtension, 764c7b673845886b393ee0b2c6fe7f73, ExportedByJLJnaming); 

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
