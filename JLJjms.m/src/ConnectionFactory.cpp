// ConnectionFactory.cpp

//********************************************************************

#ifndef ConnectionFactory_cpp
#define ConnectionFactory_cpp

//********************************************************************

#include <jlj/jms/ConnectionFactory.h>
#include <jlj/lang/ClassLoader.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(jms)

//********************************************************************

ConnectionFactoryI::~ConnectionFactoryI()
{}

//********************************************************************

ConnectionFactory ConnectionFactory::lookup(const String& clazzname,
											const String& library, 
											const String& configuration) throw (Exception)
{	
	ClassLoader classloader = new ClassLoaderI();
	classloader->loadLibrary(library);
	ConnectionFactory connectionfactory;
	classloader->loadClass(clazzname)->newInstance()->downcast(connectionfactory);
	connectionfactory->setConfiguration(configuration);
	return connectionfactory;
}

//********************************************************************

NAMESPACE_END(jms)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
