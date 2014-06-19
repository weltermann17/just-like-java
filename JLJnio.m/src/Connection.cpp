// Connection.cpp

//********************************************************************

#ifndef jlj_nio_reactor_Connection_cpp
#define jlj_nio_reactor_Connection_cpp

//********************************************************************

#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/reactor/ConnectionLifetimeManager.h>
#include <jlj/util/zip/ZipInputStream.h>
#include <jlj/util/zip/ZipOutputStream.h>
using namespace jlj::util::zip;
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/ListAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/UTF8StringAnything.h>
#include <jlj/util/RawAnything.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/IntAnything.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/Uuid.h>
using namespace jlj::util;
#include <jlj/util/concurrent/LinkedBlockingQueue.h>
using namespace jlj::util::concurrent;
#include <jlj/util/concurrent/atomic/AtomicInteger.h>
using namespace jlj::util::concurrent::atomic;
#include <jlj/io/FileWriter.h>
using namespace jlj::io;
#include <jlj/lang/Thread.h>
using namespace jlj::lang;
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
using namespace jlj::lang::lifetimemanagement;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

ConnectionI::~ConnectionI()
{
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::INFO, L"ConnectionI::~ConnectionI()");
}

//********************************************************************

ConnectionI::ConnectionI(const String& id)
: id(id)
{}

//********************************************************************

void ConnectionI::handleEvent(SelectionKey key) 
{
	Logger logger = Logger::getLogger(Reactor::REACTORS_LOGGER_NAME);

	try
	{
		try
		{
			if (testIfStateUnSet(DIS))
			{
				if (key->isReadable())
				{
					SocketChannel socketchannel;
					key->getChannel()->downcast(socketchannel);

					if (socketchannel->isOpen())
					{
						InputStream socketinputstream = socketchannel->socket()->getInputStream();
						if (0 >= socketinputstream->available())
						{
							fireEvent(CCP);
							return;
						}
						else
						{
							Multiplexer multiplexer = ConnectionLifetimeManager::getInstance(id)->getMultiplexer();
							multiplexer->parseBuffer(socketinputstream); 
						}
					}
					else
					{
						throw IOException(WITHDETAILS(L"SocketChannel not open."));
					}
				}
			}
		}
		catch (const IOException& e)
		{
			logger->log(Level::INFO, L"Connection::handleEvent() : " + e->toString());
			ConnectionLifetimeManager manager = ConnectionLifetimeManager::getInstance(id);
			Reactor reactor = manager->getReactor();
			if (manager->isFinalized()) reactor->cleanup();
			fireEvent(IOR);
			if (reactor->getPropagateIOExceptions())
			{
				Anything nothing;
				reactor->receiveQueue()->put(nothing);
			}
		}
		catch (const CancelledKeyException& e)
		{
			logger->log(Level::INFO, L"Connection::handleEvent() : " + e->toString());
			ConnectionLifetimeManager manager = ConnectionLifetimeManager::getInstance(id);
			Reactor reactor = manager->getReactor();
			if (manager->isFinalized()) reactor->cleanup();
			fireEvent(IOR);
			if (reactor->getPropagateIOExceptions())
			{
				Anything nothing;
				reactor->receiveQueue()->put(nothing);
			}
		}
		catch (const IndexOutOfBoundsException&)
		{
			// ignore
		}
		catch (const Exception& e)
		{
			logger->log(Level::SEVERE, L"Connection::handleEvent() : " + e->toString());
		}
		catch (...)
		{
			logger->log(Level::SEVERE, L"Connection::handleEvent() : unknown exception");
		}
	}
	catch (...)
	{
		// ignore
	}
} 

//********************************************************************

String ConnectionI::getLocalIdentifier() const throw (IOException)
{
	if (0 == localidentifier->length())
	{
		SocketChannel socketchannel = ConnectionLifetimeManager::getInstance(id)->getSocketChannel();
		Socket socket = socketchannel->socket();
		StringBuffer buffer;
		buffer->append(socket->getLocalAddress()->getHostAddress());
		buffer->append(L":");
		buffer->append(socket->getLocalPort());
		localidentifier = buffer->toString()->toLowerCase();
	}
	return localidentifier;
}

//********************************************************************

String ConnectionI::getRemoteIdentifier() const throw (IOException)
{
	if (0 == remoteidentifier->length())
	{
		SocketChannel socketchannel = ConnectionLifetimeManager::getInstance(id)->getSocketChannel();
		Socket socket = socketchannel->socket();
		StringBuffer buffer;
		buffer->append(socket->getRemoteSocketAddress()->getAddress()->getHostAddress());
		buffer->append(L":");
		buffer->append(socket->getRemoteSocketAddress()->getPort());
		remoteidentifier = buffer->toString()->toLowerCase();
	}
	return remoteidentifier;
}

//********************************************************************

void ConnectionI::initializeShutdown() throw (IOException)
{
	if (testIfStateSet(ICS)) return;

	send(new NullAnythingI(), false);
	fireEvent(NAS);
}

//********************************************************************

void ConnectionI::send(Anything anything, bool jsonencoding) throw (IOException)
{
	send(anything, jsonencoding, ZipUtility::NO_COMPRESSION);
}

//********************************************************************

void ConnectionI::send(Anything anything, bool jsonencoding, int compressionlevel) throw (IOException)
{
	try
	{
		if (testIfStateSet(ICS)) 
		{
			if (ConnectionLifetimeManager::getInstance(id)->isFinalized()) 
			{
				ConnectionLifetimeManager::getInstance(id)->getReactor()->cleanup();
			}
			throw IOException(WITHDETAILS(L"Internal send already closed."));
		}

		String streamid = anything->isNull() ? NULLSTREAMID : Uuid::randomUuid()->toString()->toLowerCase();

		OutputStream chunkedoutputstream = new ChunkedOutputStreamI(id, streamid);
		OutputStream zippedoutputstream = new ZipOutputStreamI(chunkedoutputstream, compressionlevel);

		if (jsonencoding && Anything::ANY_UTF8STRING != anything->type())
		{
			Anything json = new UTF8StringAnythingI(anything->toJSON());
			json->serialize(zippedoutputstream);
		}
		else
		{	
			anything->serialize(zippedoutputstream);
		}
	}
	catch (const IOException&)
	{
		throw;
	}
	catch (const IndexOutOfBoundsException&)
	{
		throw IOException(WITHDETAILS(L"Connection already closed."));
	}
	catch (const Exception& e)
	{
		Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::SEVERE, L"Connection::send() : " + e->toString());
	}
}

//********************************************************************

void ConnectionI::registerWith(Reactor reactor) 
{}

//********************************************************************

const String ConnectionI::NULLSTREAMID = L"00000000000000000000000000000000";

//********************************************************************
//********************************************************************

Connection Connection::open(SocketChannel socketchannel,
							Reactor reactor)
{
	Map<String, ConnectionLifetimeManager> lifetimemanagers;
	LifetimeManager::getInstance(L"jlj")->get(L"connectionlifetimemanagers", lifetimemanagers);

	String id = Uuid::randomUuid()->toString();

	ConnectionLifetimeManager connectionlifetimemanager = 
		new ConnectionLifetimeManagerI(
		id, 
		socketchannel, 
		reactor);

	lifetimemanagers->put(id, connectionlifetimemanager);
	reactor->addConnectionId(id);

	Connection connection = connectionlifetimemanager->getConnection();

	socketchannel->registerWith(
		reactor->getSelector(), 
		SelectionKey::OP_READ, 
		connection);

	reactor->getSelector()->wakeup();

	return connection;
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
