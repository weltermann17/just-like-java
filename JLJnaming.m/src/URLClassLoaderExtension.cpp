// URLClassLoaderExtension.cpp

//********************************************************************

#ifndef URLClassLoaderExtension_cpp
#define URLClassLoaderExtension_cpp

//********************************************************************

#include <jlj/naming/URLClassLoaderExtension.h>
#include <jlj/util/StringAnything.h>
using namespace jlj::util;

#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(naming)

//********************************************************************

ClassLoaderInformationDefinition(jlj::naming::URLClassLoaderExtension, 764c7b673845886b393ee0b2c6fe7f73); 

//********************************************************************

URLClassLoaderExtensionI::~URLClassLoaderExtensionI()
{}

//********************************************************************

URLClassLoaderExtensionI::URLClassLoaderExtensionI()
{}

//********************************************************************

Object URLClassLoaderExtensionI::extend(Object) throw (Exception)
{
	InitialContext initialcontext = InitialContext::newInstance();
	if (initialcontext->isBound(L"/env/jlj_urlclassloader_cache"))
	{
		return initialcontext->lookup(L"/env/jlj_urlclassloader_cache");
	}
	else
	{
		return new StringAnythingI();
	}

}

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

//********************************************************************

// eof
