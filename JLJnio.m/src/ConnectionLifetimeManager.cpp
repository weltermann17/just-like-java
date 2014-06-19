// ConnectionLifetimeManager.cpp

//********************************************************************

#ifndef ConnectionLifetimeManager_cpp
#define ConnectionLifetimeManager_cpp

//********************************************************************

#include <jlj/nio/reactor/ConnectionLifetimeManager.h>

//********************************************************************

#include <jlj/lang/Thread.h>
using namespace jlj::lang;
#include <jlj/util/concurrent/locks/ReentrantLock.h>
using namespace jlj::util::concurrent::locks;
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/concurrent/LinkedBlockingQueue.h>
#include <jlj/util/Collections.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/Uuid.h>
using namespace jlj::util;
#include <jlj/util/logging/Logger.h>
#include <jlj/util/logging/ConsoleHandler.h>
using namespace jlj::util::logging;

#include <jlj/nio/reactor/Reactor.h>
#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/reactor/Multiplexer.h>
#include <jlj/nio/reactor/ChunkedOutputStream.h>
#include <jlj/nio/reactor/ChunkedInputStream.h>
using namespace jlj::nio::reactor;
#include <jlj/lang/lifetimemanagement/LifetimeManager.h>
using namespace jlj::lang::lifetimemanagement;

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(reactor)

//********************************************************************

ConnectionLifetimeManagerI::~ConnectionLifetimeManagerI()
{
	try
	{
		synchronized(chunkedinputstreams->getLock())
		{
			Iterator<ChunkedInputStream> i = chunkedinputstreams->values()->iterator();
			while (i->hasNext())
			{
				i->next()->pipe(0);
			}
		}
		closeSocketChannel();
		multiplexer.reset(0);
		connection.reset(0);
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ConnectionLifetimeManagerI::~ConnectionLifetimeManagerI() : " + e->toString());
	}
	logger->log(Level::FINEST, L"ConnectionLifetimeManagerI::~ConnectionLifetimeManagerI()");
}

//********************************************************************

ConnectionLifetimeManagerI::ConnectionLifetimeManagerI(const String& id,
													   SocketChannel socketchannel,
													   Reactor reactor)
													   : id(id)
													   , socketchannel(socketchannel)
													   , reactor(reactor)
													   , connection(new ConnectionI(id))
													   , multiplexer(new MultiplexerI(id))
													   , sendbuffers(reactor->getSendBuffers())
													   , states(INITIAL)
													   , events(new LinkedBlockingQueueI<EVENTS>())
													   , logger(Logger::getLogger(Reactor::REACTORS_LOGGER_NAME))
{
	Map<String, ChunkedInputStream> c = new LinkedHashMapI<String, ChunkedInputStream>();
	chunkedinputstreams = Collections::synchronizedMap(c);
}

//********************************************************************

bool ConnectionLifetimeManagerI::equals(const Object& object) const
{
	ConnectionLifetimeManager b;
	if (object->instanceOf(b))
	{
		return id->equals(b->toString());
	}
	else
	{
		return false;
	}
}

//********************************************************************

int ConnectionLifetimeManagerI::hashCode() const
{
	return id->hashCode();
}

//********************************************************************

String ConnectionLifetimeManagerI::toString() const
{
	return id;
}

//********************************************************************

Reactor ConnectionLifetimeManagerI::getReactor() const
{
	return reactor;
}

//********************************************************************

ExecutorService<int> ConnectionLifetimeManagerI::getExecutorService() const
{
	return reactor->getExecutorService();
}

//********************************************************************

Connection ConnectionLifetimeManagerI::getConnection() const
{
	return connection;
}

//********************************************************************

SocketChannel ConnectionLifetimeManagerI::getSocketChannel() const
{
	return socketchannel;
}

//********************************************************************

Multiplexer ConnectionLifetimeManagerI::getMultiplexer() const
{
	return multiplexer;
}

//********************************************************************

ChunkBuffers ConnectionLifetimeManagerI::getSendBuffers() const
{
	return sendbuffers;
}

//********************************************************************

ChunkedInputStream ConnectionLifetimeManagerI::getChunkedInputStream(const String& streamid) const
{
	if (chunkedinputstreams->containsKey(streamid))
	{
		return chunkedinputstreams->get(streamid);
	}
	else
	{
		ChunkedInputStream chunkedinputstream = new ChunkedInputStreamI(id);
		chunkedinputstreams->put(streamid, chunkedinputstream);
		chunkedinputstream->demultiplex();
		return chunkedinputstream;
	}
}

//********************************************************************

bool ConnectionLifetimeManagerI::isFinalized() 
{
	events->put(0 == chunkedinputstreams->size() ? OIC : NUL);
	runFiniteStateMachine();
	return DIS == (states & DIS);
}

//********************************************************************

bool ConnectionLifetimeManagerI::testIfSet(int test) 
{
	runFiniteStateMachine();
	return test == (states & test);
}

//********************************************************************

void ConnectionLifetimeManagerI::addEvent(EVENTS event)
{
	events->put(event);
}				  

//********************************************************************

void ConnectionLifetimeManagerI::runFiniteStateMachine()
{
	try
	{
		if (0 == events->size()) return;

		Queue<EVENTS> currentevents = new LinkedListI<EVENTS>();
		events->drainTo(currentevents);

		Iterator<EVENTS> e = currentevents->iterator();
		while (e->hasNext())
		{
			EVENTS event = e->next();

			switch (event)
			{
			case NUL:
				{
					break;
				}
			case NCS:
				{
					states->set(states | ECS);
					break;
				}
			case NCR:
				{
					states->set(states | ECR);
					break;
				}
			case NAS:
				{
					states->set(states | ICS);
					break;
				}
			case NAR:
				{
					states->set(states | ISR);
					markInputstreamsAsClosed();
					break;
				}
			case OIC:
				{		
					if ((0 == (states & ICR)) && (ISR == (states & ISR)) && checkInputstreamsAreClosed()) 
					{
						closeSocketChannel();
						states->set(states | ICR);
					}
					break;
				}
			case IOS:
			case IOR:
			case CCP:
				{
					closeSocketChannel();
					states->set(states | (ICS | ISR | ECS | ECR));
					break;
				}
			default:
				{
					logger->log(Level::SEVERE, L"runFiniteStateMachine : Invalid event " + getEventAsString(event));
					break;
				}
			}

			if ((ICS | ICR | ECS | ECR) == (states & (ICS | ICR | ECS | ECR)))
			{
				states->set(states | DIS);
				break;
			}
		}
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ConnectionLifetimeManagerI::runFiniteStateMachine() : " + e->toString());
	}
	catch (...)
	{
		logger->log(Level::SEVERE, L"ConnectionLifetimeManagerI::runFiniteStateMachine() : Unknown exception");
	}
}

//********************************************************************

String ConnectionLifetimeManagerI::getEventAsString(EVENTS event) const
{
	StringBuffer buf;
	switch (event)
	{
	case NUL:
		{
			buf->append(L"NUL");
			break;
		}
	case NCS:
		{
			buf->append(L"NCS");
			break;
		}
	case NCR:
		{
			buf->append(L"NCR");
			break;
		}
	case NAS:
		{
			buf->append(L"NAS");
			break;
		}
	case NAR:
		{
			buf->append(L"NAR");
			break;
		}
	case OIC:
		{
			buf->append(L"OIC");
			break;
		}
	case IOS:
		{
			buf->append(L"IOS");
			break;
		}
	case IOR:
		{
			buf->append(L"IOR");
			break;
		}
	case CCP:
		{
			buf->append(L"CCP");
			break;
		}
	default:
		{
			buf->append(L"???");
			break;
		}
	}
	return buf->toString();
}

//********************************************************************

String ConnectionLifetimeManagerI::getStatesAsString() const
{
	StringBuffer buf;
	buf->append(L"\n");

	for (int state = INITIAL + 1; FINAL != state; state <<= 1)
	{
		switch (state)
		{
		case INITIAL:
			{
				break;
			}
		case ICS:
			{
				buf->append(L"ICS ");
				break;
			}
		case ICR:
			{
				buf->append(L"ICR ");
				break;
			}
		case ECS:
			{
				buf->append(L"ECS ");
				break;
			}
		case ECR:
			{
				buf->append(L"ECR ");
				break;
			}
		case ISR:
			{
				buf->append(L"ISR ");
				break;
			}
		case DIS:
			{
				buf->append(L"DIS ");
				break;
			}
		default:
			{
				buf->append(L"XXX ");
				break;
			}
		}
	}
	buf->append(L"\n");

	for (int state = INITIAL + 1; FINAL != state; state <<= 1)
	{
		buf->append(state == (states & state) ? L" 1  " : L" 0  ");
	}

	return buf->toString();
}

//********************************************************************

void ConnectionLifetimeManagerI::closeSocketChannel()
{
	try
	{
		SelectionKey key = socketchannel->keyFor(reactor->getSelector());
		if (!!key) key->cancel();
		socketchannel->close();
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ConnectionLifetimeManagerI::closeSocketChannel() : " + e->toString());
	}
}

//********************************************************************

void ConnectionLifetimeManagerI::cleanup()
{
	try
	{
		synchronized(chunkedinputstreams->getLock())
		{
			Iterator<ChunkedInputStream> i = chunkedinputstreams->values()->iterator();
			while (i->hasNext())
			{
				bool closed = i->next()->isClosed();
				if (closed) i->remove();
			}
		}
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ConnectionLifetimeManagerI::cleanup() : " + e->toString());
	}
}

//********************************************************************

void ConnectionLifetimeManagerI::markInputstreamsAsClosed()
{
	try
	{
		synchronized(chunkedinputstreams->getLock())
		{
			Iterator<ChunkedInputStream> i = chunkedinputstreams->values()->iterator();
			while (i->hasNext())
			{
				i->next()->pipe(0);
			}
		}
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ConnectionLifetimeManagerI::markInputstreamsAsClosed() : " + e->toString());
	}
}

//********************************************************************

bool ConnectionLifetimeManagerI::checkInputstreamsAreClosed() const
{
	bool allclosed = true;
	try
	{
		synchronized(chunkedinputstreams->getLock())
		{
			Iterator<ChunkedInputStream> i = chunkedinputstreams->values()->iterator();
			while (i->hasNext())
			{
				bool closed = i->next()->isClosed();
				if (!closed)
				{
					allclosed = false;
					break;
				}
			}
		}
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ConnectionLifetimeManagerI::checkInputstreamsAreClosed() : " + e->toString());
	}
	return allclosed;
}

//********************************************************************
//********************************************************************

ConnectionLifetimeManager ConnectionLifetimeManager::getInstance(const String& id) throw (IndexOutOfBoundsException)
{
	Map<String, ConnectionLifetimeManager> map;
	LifetimeManager::getInstance(L"jlj")->get(L"connectionlifetimemanagers", map);
	return map->get(id);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
