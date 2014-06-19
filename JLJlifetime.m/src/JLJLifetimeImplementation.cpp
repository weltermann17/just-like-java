// JLJLifetimeImplementation.cpp

//********************************************************************

#ifndef JLJLifetimeImplementation_cpp
#define JLJLifetimeImplementation_cpp

//********************************************************************

#include <jlj/lifetime/JLJLifetimeImplementation.h>
#include <jlj/lang/ClassLoader.h>
#include <jlj/lang/Thread.h>
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/Collections.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/Uuid.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/nio/reactor/Reactor.h>
#include <jlj/naming/InitialContext.h>
using namespace jlj::lang;
using namespace jlj::util;
using namespace jlj::util::logging;
using namespace jlj::nio::reactor;
using namespace jlj::naming;

//********************************************************************

#include <iostream>
using namespace std;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(lifetime)

//********************************************************************

void throwNullPointerException(const char* type) throw (Exception)
{
	StringBuffer clazzname(type);
	clazzname->replace(L"struct ", L"");
	clazzname->replace(L"class ", L"");
	clazzname->replaceAll(L" ", L"");
	StringBuffer s;
	s->append(L"variable of type <");
	s->append(clazzname);
	s->append(L"> is a null pointer");
	throw NullPointerException(String(s));
}

//********************************************************************
//********************************************************************

JLJLifetimeImplementationI::~JLJLifetimeImplementationI() throw (Exception)
{
	if (initialized) finalize();
}

//********************************************************************

JLJLifetimeImplementationI::JLJLifetimeImplementationI(const String& defaultconfiguration) throw (Exception)
: AbstractLifetimeImplementationI(L"jlj")
, initialized(false)
, defaultconfiguration(defaultconfiguration)
{}

//********************************************************************

void JLJLifetimeImplementationI::initialize() throw (Exception)
{
	if (initialized) throw Exception(WITHDETAILS(L"JLJLifetimeImplementationI::initialize() : already called before"));

	try 
	{
		initialized = true;

		// Exception handling

		GenericReference::exceptioncallback = throwNullPointerException;

		// Threads

		initializeThread();
		initializeThreadPool();

		// ClassLoader

		initializeClassLoader();

		// Uuids

		List<Uuid> uuidlist = new LinkedListI<Uuid>();
		getLifetimeManager()->put(L"uuids", Collections::synchronizedList(uuidlist));

		// Logging

		Map<String, Logger> loggerlist = new LinkedHashMapI<String, Logger>();
		getLifetimeManager()->put(L"loggers", Collections::synchronizedMap(loggerlist));
		Level::init();

		// NamedSingletons

		Map<String, Object> namedsingletons = new LinkedHashMapI<String, Object>();
		getLifetimeManager()->put(L"namedsingletons", Collections::synchronizedMap(namedsingletons));

		// Reactor

		initializeReactors();

		// Naming

		InitialContext initialcontext = InitialContext::newInstance();
		initialcontext->loadBindings(defaultconfiguration);
	}
	catch (const Exception&)
	{
		finalize();
		throw;
	}
}

//********************************************************************

void JLJLifetimeImplementationI::finalize() throw (Exception)
{
	if (!initialized) throw Exception(WITHDETAILS(L"JLJLifetimeImplementationI::finalize() : called without calling initialize() before"));

	try 
	{
		initialized = false;

		// Naming 

		InitialContext initialcontext = InitialContext::getInstance();
		initialcontext->unloadBindings();

		// Reactor

		finalizeReactors();

		// NamedSingletons

		getLifetimeManager()->remove(L"namedsingletons");

		// Logging									   

		getLifetimeManager()->remove(L"loggers");

		// Uuids

		getLifetimeManager()->remove(L"uuids");

		// ClassLoader

		finalizeClassLoader();    

		// Threads

		finalizeThreadPool(); 
	}
	catch (const Exception& e)
	{
		fprintf(stderr, "JLJLifetimeImplementationI::finalize() failed : %d\n%s\n", e->toString()->length(), constcharptr(e->toString()));
		throw;
	}
	catch (...)
	{
		fprintf(stderr, "JLJLifetimeImplementationI::finalize() : Unknown exception\n"); 
		throw;
	}
}

//********************************************************************

NAMESPACE_END(lifetime)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
