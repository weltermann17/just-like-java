// Reactor.cpp

//********************************************************************

#ifndef Reactor_cpp
#define Reactor_cpp

//********************************************************************

#include <jlj/nio/reactor/Reactor.h>
#include <jlj/nio/reactor/ChunkBufferPool.h>
#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/reactor/ConnectionLifetimeManager.h>
#include <jlj/nio/channels/SelectionKey.h>
#include <jlj/nio/channels/SelectableChannel.h>
#include <jlj/util/concurrent/LinkedBlockingQueue.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/RawAnything.h>
#include <jlj/util/IntAnything.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/LinkedHashMap.h>
#include <jlj/io/InMemoryInputStream.h>
using namespace jlj::io;
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/ArrayList.h>
#include <jlj/util/Uuid.h>
#include <jlj/util/StopWatch.h>
#include <jlj/util/logging/ConsoleHandler.h>
using namespace jlj::util;
using namespace jlj::util::logging;
#include <jlj/lang/Thread.h>
using namespace jlj::lang;
#include <jlj/util/concurrent/locks/ReentrantLock.h>
using namespace jlj::util::concurrent::locks;
#include <signal.h>
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
using namespace jlj::lang::lifetimemanagement;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

void initializeReactors() throw (Exception)
{
	Map<String, ConnectionLifetimeManager> connectionlifetimemanagers = 
		new LinkedHashMapI<String, ConnectionLifetimeManager>();
	
	LifetimeManager::getInstance(L"jlj")->put(
		L"connectionlifetimemanagers", 
		Collections::synchronizedMap(connectionlifetimemanagers));
}

//********************************************************************

void finalizeReactors() throw (Exception)
{
	LifetimeManager::getInstance(L"jlj")->remove(L"connectionlifetimemanagers");
}

//********************************************************************
//********************************************************************

const int DEFAULTQUEUELIMIT = 100000;

//********************************************************************

ReactorI::~ReactorI() throw (IOException)
{
	cleanup();
	receivequeue->clear();
	Logger::getLogger(Reactor::REACTORS_LOGGER_NAME)->log(Level::FINEST, L"ReactorI::~ReactorI()");
}

//********************************************************************

ReactorI::ReactorI(ExecutorService<int> executorservice,
				   int timeout) throw (IOException)
				   : selector(Selector::open())
				   , timeout(timeout)
				   , shutdowntimeout(0)
				   , executorservice(executorservice)
				   , sendbuffers(new LinkedBlockingQueueI<ChunkBuffer>())
				   , receivequeue(new LinkedBlockingQueueI<Anything>())
				   , writingstopped(new LinkedBlockingQueueI<int>())
				   , logger(Logger::getLogger(Reactor::REACTORS_LOGGER_NAME))
				   , isshutdown(false)
				   , shutdownonlastconnectionclosed(false)
				   , propagateioexceptions(false)
				   , cleanupinterval(128)
{
	const int DEFAULTTIMEOUT = 15*1000;
	if (0 >= timeout) this->timeout = DEFAULTTIMEOUT;
	List<String> l = new LinkedListI<String>();
	connectionids = Collections::synchronizedList(l);
}

//********************************************************************

Selector ReactorI::getSelector() const
{
	return selector;
}

//********************************************************************

ExecutorService<int> ReactorI::getExecutorService() const
{
	return executorservice;
}

//********************************************************************

int ReactorI::getTimeout()	const
{
	return timeout;
}

//********************************************************************

void ReactorI::setTimeout(int milliseconds)
{
	timeout = milliseconds;
}

//********************************************************************

void ReactorI::run()
{
	isshutdown = false;
	startWriting();
	try
	{
		StopWatch stopwatch = new StopWatchI();
		bool shutdowninitialized = false;
		int count = 0;

		while (selector->isOpen())
		{
			try
			{
				int selected = selector->select(timeout);

				if (0 > selected) 
				{
					cleanup();
					selector->close(); 
					logger->log(Level::SEVERE, L"ReactorI::run() : break, selector returned " + String(selected));
					break; 
				}

				++count;

				if (0 == selected || cleanupinterval == count) 
				{
					count = 0;
					cleanup();
				}

				if (0 < shutdowntimeout)
				{
					if (!shutdowninitialized)
					{
						shutdowninitialized = true;
						const int INTERVALDURINGSHUTDOWN = 200;
						timeout = INTERVALDURINGSHUTDOWN;
						stopwatch->start();
					}
					else
					{
						stopwatch->stop();
						if ((shutdowntimeout / 1000.) < stopwatch->getSeconds())
						{
							cleanup();
							selector->close(); 
							logger->log(Level::INFO, L"ReactorI::run() : break, timeout = " + String(shutdowntimeout));
							break;
						}
					}
				}

				if (shutdownonlastconnectionclosed)
				{ 
					if (0 == connectionids->size()) 
					{
						selector->close(); 
						logger->log(Level::INFO, L"ReactorI::run() : break, connections = 0");
						break; 
					}
				}

				Set<SelectionKey> selectionkeys = selector->selectedKeys();

				Iterator<SelectionKey> keys = selectionkeys->iterator();
				while (keys->hasNext())
				{
					SelectionKey key = keys->next();
					keys->remove(); 

					Handler handler;
					key->attachment()->downcast(handler);
					handler->handleEvent(key);
				}
			}
			catch (const IndexOutOfBoundsException& e)    
			{
				logger->log(Level::FINEST, L"ReactorI::run() : " + e->toString());
			}
			catch (const SocketException& e)
			{
				logger->log(Level::FINE, L"ReactorI::run() : " + e->toString());
			}
			catch (const Exception& e)
			{
				logger->log(Level::SEVERE, L"ReactorI::run() : " + e->toString());
			}
			catch (const std::exception& e)
			{
				logger->log(Level::SEVERE, L"ReactorI::run() : std::exception : " + String(e.what()));
			}
			catch (...)
			{
				logger->log(Level::SEVERE, L"ReactorI::run() : Unknown exception.");
			}
		}
	}
	catch (...)
	{
		logger->log(Level::SEVERE, L"ReactorI::run() : Unknown exception.");
	}
	stopWriting();
	isshutdown = true;
	logger->log(Level::FINEST, L"ReactorI::run() : exit");
}

//********************************************************************

Connection ReactorI::getConnectionByRemoteIdentifier(const String& remoteidentifier) const
{
	Connection connection;

	try
	{
		Map<String, ConnectionLifetimeManager> lifetimemanagers;
		LifetimeManager::getInstance(L"jlj")->get(L"connectionlifetimemanagers", lifetimemanagers);

		List<String> ids;
		synchronized(connectionids->getLock()) ids = new LinkedListI<String>(connectionids);

		Iterator<String> i = ids->iterator();
		while (i->hasNext())
		{
			String id = i->next();
			try
			{
				ConnectionLifetimeManager lifetimemanager = lifetimemanagers->get(id);

				if (lifetimemanager->getConnection()->getRemoteIdentifier() == remoteidentifier)
				{
					if (!lifetimemanager->testIfSet(ConnectionLifetimeManagerI::ICS | ConnectionLifetimeManagerI::ECS))
					{
						connection = lifetimemanager->getConnection();
					}
					break;	
				}
			}
			catch (const IndexOutOfBoundsException&)
			{
				logger->log(Level::FINE, L"ReactorI::getConnectionByRemoteIdentifier() : id not found : " + id + L", remote id : " + remoteidentifier);
			}
		}	
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ReactorI::getConnectionByRemoteIdentifier() : " + e->toString());
	}

	return connection;
}

//********************************************************************

void ReactorI::cleanup() 
{
	try
	{
		Map<String, ConnectionLifetimeManager> lifetimemanagers;
		LifetimeManager::getInstance(L"jlj")->get(L"connectionlifetimemanagers", lifetimemanagers);

		List<String> ids;
		synchronized(connectionids->getLock()) ids = new LinkedListI<String>(connectionids);

		Iterator<String> i = ids->iterator();
		while (i->hasNext())
		{
			String id = i->next();
			try
			{
				ConnectionLifetimeManager lifetimemanager = lifetimemanagers->get(id);

				if (lifetimemanager->isFinalized()) 
				{
					lifetimemanagers->remove(id);
					i->remove();
				}
				else
				{
					lifetimemanager->cleanup();
				}
			}
			catch (const Exception& e)
			{
				logger->log(Level::FINEST, L"ReactorI::cleanup() : " + e->toString());
			}
		}	

		synchronized(connectionids->getLock()) 
		{
			connectionids->clear();
			connectionids->addAll(ids);
		}   
	}
	catch (const IndexOutOfBoundsException&)
	{
	}
	catch (const SocketException& e)
	{
		logger->log(Level::FINEST, L"ReactorI::cleanup() : " + e->toString());
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ReactorI::cleanup() : " + e->toString());
	}
}

//********************************************************************

ChunkBuffers ReactorI::getSendBuffers()  
{
	return sendbuffers;
}

//********************************************************************

AnythingQueue ReactorI::receiveQueue() 
{
	return receivequeue;
}

//********************************************************************

void ReactorI::addConnectionId(const String& id)
{
	connectionids->add(id);
}

//********************************************************************

void ReactorI::startWriting()
{
	Callable<int> callable(this);
	executorservice->submit(callable);
}

//********************************************************************

void ReactorI::stopWriting()
{
	logger->log(Level::FINEST, L"ReactorI::stopWriting()");

	receivequeue->put(new NullAnythingI());

	ChunkBuffer chunk = ChunkBufferPool::getInstance()->get();
	chunk->position = chunk->limit = ChunkBufferPoolI::DEFAULTCAPACITY;
	sendbuffers->put(chunk);

	writingstopped->poll(timeout);
}

//********************************************************************

int ReactorI::call() throw (Exception)
{
	char* idbuf = new char[Multiplexer::IDENTIFIERLENGTH];

	bool aborted = false;

	while (!aborted)
	{
		String id;

		try
		{
			ChunkBuffer chunk = sendbuffers->poll(timeout);

			if (0 == chunk) 
			{
				if (isShutdown()) 
				{
					aborted = true; 
				}
				else
				{
					continue;
				}
			}
			else
			{
				aborted = chunk->position == ChunkBufferPoolI::DEFAULTCAPACITY && chunk->limit == ChunkBufferPoolI::DEFAULTCAPACITY;
			}

			if (!aborted)
			{
				memcpy(idbuf, chunk->buffer, Multiplexer::IDENTIFIERLENGTH);
				id = String(idbuf, Multiplexer::IDENTIFIERLENGTH);

				try
				{
					if (testIfStateUnSet(ECS))
					{
						memcpy(idbuf, chunk->buffer + Multiplexer::IDENTIFIERLENGTH, Multiplexer::IDENTIFIERLENGTH);
						String streamid(idbuf, Multiplexer::IDENTIFIERLENGTH);

						try 
						{
							OutputStream socketoutputstream = 
								ConnectionLifetimeManager::getInstance(id)->getSocketChannel()->socket()->getOutputStream();

							socketoutputstream->write(chunk->buffer, chunk->position, chunk->limit);
							socketoutputstream->flush();

							if (ConnectionI::NULLSTREAMID == streamid) fireEvent(NCS); 
						}
						catch (const IOException&)
						{
							fireEvent(IOS);
							Anything nothing;
							receivequeue->put(nothing);
						}
					}
				}
				catch (const IndexOutOfBoundsException& e)
				{
					logger->log(Level::FINEST, L"ReactorI::call() : IOB : " + e->toString());
				}
			}

			ChunkBufferPool::getInstance()->makeIdle(chunk);
		}
		catch (const Exception& e)
		{
			logger->log(Level::SEVERE, L"ReactorI::call() : " + e->toString());
		}
		catch (const std::exception& e)
		{
			logger->log(Level::SEVERE, L"ReactorI::call() : std::exception : " + String(e.what()));
		}
		catch (...)
		{
			logger->log(Level::SEVERE, L"ReactorI::call() : Unknown exception.");
		}
	}
	delete [] idbuf;

	writingstopped->put(-1);
	logger->log(Level::FINEST, L"ReactorI::call() : stopped writing.");

	return 0;
}

//********************************************************************

bool ReactorI::isShutdown() const
{
	return isshutdown;
}

//********************************************************************

void ReactorI::initializeShutdown(int timeout)
{
	try
	{
		const int DEFAULTTIMEOUT = 15*1000;
		shutdowntimeout = DEFAULTTIMEOUT > timeout ? DEFAULTTIMEOUT : timeout;

		propagateioexceptions = true;

		selector->initializeShutdown();

		Map<String, ConnectionLifetimeManager> lifetimemanagers;
		LifetimeManager::getInstance(L"jlj")->get(L"connectionlifetimemanagers", lifetimemanagers);

		synchronized(connectionids->getLock())
		{
			logger->log(Level::FINEST, L"ReactorI::initializeShutdown() : open connections : " + String(connectionids->size()));
			Iterator<String> i = connectionids->iterator();
			while (i->hasNext())
			{
				String id = i->next();
				try
				{
					if (lifetimemanagers->containsKey(id))
					{
						lifetimemanagers->get(id)->getConnection()->initializeShutdown();
					}
				}
				catch (const Exception& e)
				{
					logger->log(Level::WARNING, L"ReactorI::initializeShutdown()" + e->toString());
				}
			}	
		}

		shutdownonlastconnectionclosed = true;
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ReactorI::initializeShutdown()" + e->toString());
	}
}

//********************************************************************

void ReactorI::setShutdownOnLastConnectionClosed(bool value)
{
	shutdownonlastconnectionclosed = value;
}

//********************************************************************

void ReactorI::setPropagateIOExceptions(bool value)
{
	propagateioexceptions = value;
}

//********************************************************************

bool ReactorI::getPropagateIOExceptions() const
{
	return propagateioexceptions;
}

//********************************************************************
//********************************************************************

const String Reactor::REACTORS_LOGGER_NAME = L"REACTOR_LOGGER";

//********************************************************************

Reactor Reactor::open(ExecutorService<int> executorservice,
					  int timeout) throw (IOException)
{
	return new ReactorI(executorservice, timeout);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
