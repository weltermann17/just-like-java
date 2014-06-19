// LoggerContextExtension.cpp

//********************************************************************

#ifndef LoggerContextExtension_cpp
#define LoggerContextExtension_cpp

//********************************************************************

#include <jlj/naming/LoggerContextExtension.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util;
using namespace jlj::util::logging;

#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(naming)

//********************************************************************

ClassLoaderInformationDefinition(jlj::naming::LoggerContextExtension, 97405f3a63f9d4389b74b8072b4e3342); 

//********************************************************************

LoggerContextExtensionI::~LoggerContextExtensionI()
{}

//********************************************************************

LoggerContextExtensionI::LoggerContextExtensionI()
{}

//********************************************************************

Object LoggerContextExtensionI::extend(Object object) throw (Exception)
{
	Logger logger; object->downcast(logger);
	//cout << "logger:" << logger->getName() << endl;
	InitialContext initialcontext = InitialContext::newInstance();
	Iterator<MapEntry<String, Object> > i = initialcontext->listBindings(L"/")->entrySet()->iterator();
	while (i->hasNext())
	{	
		MapEntry<String, Object> current = i->next();
		StringAnything any;
		if (current->value->instanceOf(any))
		{
			if (any->toString()->toLowerCase() == logger->getName())
			{
				StringBuffer buf = current->key;
				initialcontext->lookup(buf->substring(0, buf->lastIndexOf(L"/name")), logger);
				//cout << "loggerconfig:" << buf->substring(0, buf->lastIndexOf(L"/name")) << endl;
				break;
			}
		}
	}
	return logger;
}

//********************************************************************

NAMESPACE_END(naming)
NAMESPACE_END(jlj)

//********************************************************************

#endif

//********************************************************************

// eof
