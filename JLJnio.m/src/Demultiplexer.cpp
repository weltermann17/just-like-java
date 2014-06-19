// Demultiplexer.cpp

//********************************************************************

#ifndef Demultiplexer_cpp
#define Demultiplexer_cpp

//********************************************************************

#include <jlj/nio/reactor/Demultiplexer.h>
#include <jlj/nio/reactor/ConnectionLifetimeManager.h>
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/StreamAnything.h>
#include <jlj/util/UTF8StringAnything.h>
#include <jlj/util/Uuid.h>
using namespace jlj::util;
#include <jlj/util/zip/ZipInputStream.h>
#include <jlj/util/zip/ZipOutputStream.h>
using namespace jlj::util::zip;
#include <jlj/lang/Thread.h>
#include <jlj/lang/UTF8StringBuffer.h>
using namespace jlj::lang;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

DemultiplexerI::~DemultiplexerI()
{
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::FINEST, L"DemultiplexerI::~DemultiplexerI()");
}

//********************************************************************

DemultiplexerI::DemultiplexerI(const String& id,
							   ChunkedInputStream chunkedinputstream)
							   : id(id)
							   , chunkedinputstream(chunkedinputstream)
{}

//********************************************************************

void DemultiplexerI::run()
{
	try
	{
		ConnectionLifetimeManager lifetimemanager = ConnectionLifetimeManager::getInstance(id);
		AnythingQueue receivequeue = lifetimemanager->getReactor()->receiveQueue();

		InputStream zippedinputstream = new ZipInputStreamI(chunkedinputstream);

		Anything anything = Anything::decodeInputStream(zippedinputstream);


		if (anything->isNull() || 
			(Anything::ANY_UTF8STRING == anything->type() && 6 == anything->size() && L"[null]" == anything->toString())) 
		{
			fireEvent(NAR);
			lifetimemanager->getConnection()->initializeShutdown();
		}
		else
		{
			if (Anything::ANY_UTF8STRING == anything->type()) 
			{
				anything = Anything::decodeJSON(anything->toUTF8String());
				receivequeue->put(anything);
			}
			else
			{
				receivequeue->put(anything);
			}
		}
	}
	catch (const Exception& e)
	{
		Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::SEVERE, L"DemultiplexerI::call() : " + e->toString());
	}
	catch (const std::exception& e)
	{
		Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::SEVERE, L"DemultiplexerI::call() : std::exception : " + String(e.what()));
	}
	catch (...)
	{
		Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::SEVERE, L"DemultiplexerI::call() : unknown exception : ");
	}

	chunkedinputstream->close();
	chunkedinputstream.reset(0);

	if (testIfStateSet(ISR)) fireEvent(OIC);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
