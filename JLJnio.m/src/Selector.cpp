// Selector.cpp

//********************************************************************

#ifndef jlj_nio_channels_Selector_cpp
#define jlj_nio_channels_Selector_cpp

//********************************************************************

#include <jlj/lang/Runnable.h>
#include <jlj/nio/channels/Selector.h>
#include <jlj/nio/channels/SelectionKey.h>
#include <jlj/nio/channels/SelectableChannel.h>
#include <jlj/nio/channels/ServerSocketChannel.h>
#include <jlj/util/LinkedHashSet.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/ArrayList.h>
#include <jlj/util/TreeMap.h>
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/Uuid.h>
using namespace jlj::util;
#include <jlj/lang/Thread.h>
using namespace jlj::lang;
#include <jlj/util/concurrent/locks/ReentrantLock.h>
using namespace jlj::util::concurrent::locks;
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

#ifdef WIN32
#  include <winsock2.h>
#else
#  include <sys/time.h>
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <unistd.h>
#  include <signal.h>
#  include <time.h>
#endif

//********************************************************************

NAMESPACE_BEGIN(jlj)
NAMESPACE_BEGIN(nio)
NAMESPACE_BEGIN(channels)

//********************************************************************

const char WAKEUP = 'W';
const char SHUTDOWN = 'S';

//********************************************************************

SelectorI::~SelectorI() throw (IOException)
{
	close();
}

//********************************************************************

SelectorI::SelectorI() throw (IOException)
: isopen(true)
, isshuttingdown(false)
, errorcount(0)
, lock(new ReentrantLockI())
, logger(Logger::getLogger(L"REACTOR_LOGGER"))
{
	struct ConnectOnce : public RunnableI
	{ 
		ConnectOnce(int port, pt::ipstream& localwriter) : port(port), localwriter(localwriter) {}

		void run()
		{
			Thread::yield();
			localwriter.set_ip(pt::ipaddress(127, 0, 0, 1));
			localwriter.set_port(port);
			localwriter.open();
		}

		pt::ipstream& localwriter;
		int port;

	};

	pt::ipstmserver localserver;
	localserver.bind(pt::ipaddress(127, 0, 0, 1), 0);
	localserver.poll();
	const pt::ipbindinfo& ipbindinfo = localserver.get_addr(0);
	int port = ((pt::ipbindinfo&)ipbindinfo).get_port();

	Thread t= new ThreadI(new ConnectOnce(port, localwriter)); 
	t->start();
	bool connected = localserver.serve(localreader);
	if (!connected) throw IOException(WITHDETAILS(L"Unable to establish loopback connection."));

	List<SelectionKey> s;
	s = new ArrayListI<SelectionKey>();
	selectablekeys = Collections::synchronizedList<SelectionKey>(s);
	s = new ArrayListI<SelectionKey>();
	selectedkeys = Collections::synchronizedList<SelectionKey>(s);
}

//********************************************************************

void SelectorI::close() throw (IOException)
{
	if (isOpen())
	{
		synchronized(lock) isopen = false;
		selectablekeys->clear(); 
		selectedkeys->clear();
		localreader.close();
		localwriter.close();
	}
}

//********************************************************************

bool SelectorI::isOpen() const
{
	bool result;
	synchronized(lock) result = isopen;
	return result;
}

//********************************************************************

Set<SelectionKey> SelectorI::keys() const throw (ClosedSelectorException)
{
	if (!isOpen()) throw ClosedSelectorException(WITHDETAILS(L"SelectorI::keys()"));
	return new LinkedHashSetI<SelectionKey>(selectablekeys);
}

//********************************************************************

int SelectorI::select() throw (IOException) 
{
	return select(-1);
}

//********************************************************************

int SelectorI::select(int milliseconds) throw (IOException) 
{
	try
	{
		timeval t;
		t.tv_sec = milliseconds / 1000;
		t.tv_usec = (milliseconds % 1000) * 1000;

		fd_set readfds;
		fd_set writefds;

		FD_ZERO(&readfds);
		FD_ZERO(&writefds);

		// always add the local pipe used for wakeup
		pt::pintptr wakeup = localreader.get_handle();
		FD_SET(wakeup, &readfds);

		Map<int, SelectionKey> acceptables = new TreeMapI<int, SelectionKey>();
		Map<int, SelectionKey> connectables = new TreeMapI<int, SelectionKey>();
		Map<int, SelectionKey> readables = new TreeMapI<int, SelectionKey>();
		Map<int, SelectionKey> writables = new TreeMapI<int, SelectionKey>();

		{
			List<SelectionKey> selectables;
			synchronized(selectablekeys->getLock()) selectables = new ArrayListI<SelectionKey>(selectablekeys);
			Iterator<SelectionKey> i = selectables->iterator();
			while (i->hasNext())
			{
				SelectionKey key = i->next();

				if (key->isValid() && key->getChannel()->isOpen())
				{
					key->readyOps(0);

					synchronized(lock) if (!isshuttingdown) fillSets(key, acceptables, SelectionKey::OP_ACCEPT, &readfds);
					fillSets(key, connectables, SelectionKey::OP_CONNECT, &writefds);
					fillSets(key, readables, SelectionKey::OP_READ, &readfds);
					fillSets(key, writables, SelectionKey::OP_WRITE, &writefds);
				}
			}
		}

		selectedkeys->clear();

		int selected = ::select(FD_SETSIZE, &readfds, &writefds, 0, (0 > milliseconds ) ? 0 : &t);

		if (0 < selected)
		{
			errorcount = 0;

			if (FD_ISSET(wakeup, &readfds)) 
			{
				char c = localreader.get();
				if (SHUTDOWN == c) 
				{
					synchronized(selectablekeys->getLock())
					{
						Iterator<SelectionKey> i = selectablekeys->iterator();
						while (i->hasNext()) i->next()->cancel();
					}
					close();
					return -1;
				}
			}

			for (bool once = true; once; once = false)
			{
				if (selected == fillKeys(readables, SelectionKey::OP_READ, &readfds)) break;
				if (selected == fillKeys(acceptables, SelectionKey::OP_ACCEPT, &readfds)) break;
				if (selected == fillKeys(writables, SelectionKey::OP_WRITE, &writefds)) break;
				if (selected == fillKeys(connectables, SelectionKey::OP_CONNECT, &writefds)) break;
			}
		}

		if (0 > selected) 
		{
			++errorcount;

			bool trytorecover = false;

			const wchar_t* msg;
			int error = errno;
			switch (error)
			{
			case EBADF:
				msg = L"EBADF";
			case EINVAL:
				msg = L"EINVAL";
			case EINTR:
				msg = L"EINTR";
			case EAGAIN:
				msg = L"EAGAIN";
			case EFAULT:
				msg = L"EFAULT";
#ifdef WIN32
			case ERROR_FILE_NOT_FOUND:
				msg = L"ERROR_FILE_NOT_FOUND";
#endif
			default:
				msg = L"UNKNOWN";
				trytorecover = true;
				break;
			}
			logger->log(Level::FINE, L"SelectorI::select() : errorcode = " + String(msg) + L" (" + String(error) + L"), recover = " + String(trytorecover ? L"true" : L"false"));

			const int MAXERRORS = 1000;
			if (MAXERRORS < errorcount) trytorecover = false;

			if (trytorecover) selected = 0;
		}

		return selected;
	}
	catch (const IOException&)
	{
		throw;
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"SelectorI::select() : " + e->toString());
		return 0;
	}
	catch (...)
	{
		logger->log(Level::SEVERE, L"SelectorI::select() : Unknown exception");
		return 0;
	}
}

//********************************************************************

Set<SelectionKey> SelectorI::selectedKeys() const throw (ClosedSelectorException)
{
	if (!isOpen()) throw ClosedSelectorException(WITHDETAILS(L"SelectorI::selectedKeys()"));
	return new LinkedHashSetI<SelectionKey>(selectedkeys);
}

//********************************************************************

int SelectorI::selectNow() throw (IOException) 
{
	return select(0);
}

//********************************************************************

Selector SelectorI::wakeup() throw (ClosedSelectorException)
{
	synchronized(lock)
	{
		if (isopen)
		{
			localwriter.put(WAKEUP);
			localwriter.flush();
		}
	}
	return this;
}

//********************************************************************

Selector SelectorI::shutdown() throw (ClosedSelectorException) 
{
	synchronized(lock)
	{
		if (isopen)
		{
			localwriter.put(SHUTDOWN);
			localwriter.flush();
		}
	}
	return this;
}

//********************************************************************

void SelectorI::initializeShutdown() 
{
	synchronized(lock)
	{
		isshuttingdown = true;
		wakeup();
	}
}

//********************************************************************

void SelectorI::registerWith(SelectionKey key) 
{
	if (!selectablekeys->contains(key)) selectablekeys->add(key);
}

//********************************************************************

void SelectorI::unregisterFrom(SelectionKey key) 
{
	if (selectablekeys->contains(key)) selectablekeys->remove(key);
}

//********************************************************************

void SelectorI::fillSets(const SelectionKey& key,
						 Map<int, SelectionKey> map, 
						 int op,
						 void* set)
{
	if (0 != (op & key->interestOps()))
	{
		int handle;
		if (SelectionKey::OP_ACCEPT == op)
		{
			ServerSocketChannel serversocketchannel;
			key->getChannel()->downcast(serversocketchannel);
			handle = serversocketchannel->socket()->getSocketHandle();
		}
		else
		{
			SocketChannel socketchannel;
			key->getChannel()->downcast(socketchannel);
			handle = socketchannel->socket()->getSocketHandle();
		}

		FD_SET(handle, (fd_set*) set);
		map->put(handle, key);
	}
}

//********************************************************************

int SelectorI::fillKeys(Map<int, SelectionKey> map, 
						int op,
						void* set)
{

	Iterator<MapEntry<int, SelectionKey> > i = map->entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<int, SelectionKey> current = i->next();
		if (FD_ISSET(current->key, (fd_set*) set))
		{
			SelectionKey key = current->value;
			key->readyOps(op, true);
			selectedkeys->add(key);
		}
	}
	map->clear();
	return selectedkeys->size();
}

//********************************************************************
//********************************************************************

Selector Selector::open() throw (IOException)
{
	return new SelectorI();
}

//********************************************************************

NAMESPACE_END(channels)
NAMESPACE_END(nio)
NAMESPACE_END(jlj)

//********************************************************************

#endif

// eof
