// ReactorTransportationLayer.cpp

//********************************************************************

#ifndef ReactorTransportationLayer_cpp
#define ReactorTransportationLayer_cpp

//********************************************************************

#include <jmstools/implementation/reactor/ReactorTransportationLayer.h>
#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/reactor/Connector.h>
#include <jlj/util/concurrent/locks/ReentrantLock.h>
using namespace jlj::util::concurrent::locks;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

ReactorTransportationLayerI::~ReactorTransportationLayerI()
{
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::FINEST, L"ReactorTransportationLayerI::~ReactorTransportationLayerI()");
}

//********************************************************************

ReactorTransportationLayerI::ReactorTransportationLayerI(ExecutorService<int> threadpool,
														 const String& hostname, 
														 int port, 
														 int timeout, 
														 int shutdowntimeout) 
														 : hostname(hostname)
														 , port(port)
														 , shutdowntimeout(shutdowntimeout)
														 , lock(new ReentrantLockI())
														 , threadpool(threadpool)
{
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::FINEST, L"ReactorTransportationLayerI::ReactorTransportationLayerI()");
	reactor = Reactor::open(threadpool, timeout);
	reactor->setPropagateIOExceptions(true);
}

//********************************************************************

void ReactorTransportationLayerI::initialize() throw (IOException)
{
	threadpool->execute(reactor);
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::FINEST, L"ReactorTransportationLayerI::initialize()");
}

//********************************************************************

void ReactorTransportationLayerI::finalize() throw (IOException)
{
	reactor->initializeShutdown(shutdowntimeout);

	const int INTERVAL = 200;
	while (!reactor->isShutdown())
	{
		Thread::sleep(INTERVAL);
	}
}

//********************************************************************

void ReactorTransportationLayerI::send(Anything anything) throw (IOException)
{
	try
	{
		Logger logger =	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME);

		jlj::nio::reactor::Connection connection = reactor->getConnectionByRemoteIdentifier(connectionid);

		if (!connection)
		{
			Connector connector = Connector::open(reactor, reactor->getTimeout());
			synchronized(lock) 
			{
				connection = connector->connect(new InetSocketAddressI(hostname, port));
				connectionid = connection->getRemoteIdentifier();
			}
		}

		Message message = new MessageI(anything);
		message->setReplyToRemoteID(connection->getLocalIdentifier());
		connection->send(anything, message->getDestinationJsonEncoding());
	}
	catch (const IOException&)
	{
		synchronized(lock) connectionid = L"";
		throw;
	}
	catch (const Exception& e)
	{
		synchronized(lock) connectionid = L"";
		throw IOException(WITHDETAILS(e->toString()));
	}
}

//********************************************************************

Anything ReactorTransportationLayerI::receive(int timeout) throw (IOException)
{
	try
	{
		Logger logger =	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME);

		if (0 < connectionid->length())
		{
			synchronized(lock) 
			{
				jlj::nio::reactor::Connection connection = reactor->getConnectionByRemoteIdentifier(connectionid);
				if (!connection)
				{
					connectionid = L"";
					throw IOException(WITHDETAILS(L"Connection closed."));
				}
			}
		}
		Anything anything = reactor->receiveQueue()->poll(timeout);

		return anything;
	}
	catch (const IOException&)
	{
		throw;
	}
	catch (const Exception& e)
	{
		throw IOException(WITHDETAILS(e->toString()));
	}
}

//********************************************************************

bool ReactorTransportationLayerI::hasProvider() const
{
	return true;
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
