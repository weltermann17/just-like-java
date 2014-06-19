// Consumers.cpp

//********************************************************************

#ifndef jmstools_jmsprovider_Consumers_cpp
#define jmstools_jmsprovider_Consumers_cpp

//********************************************************************

#include <Consumers.h>
#include <Forwarder.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jmstools/implementation/reactor/AnythingMessage.h>
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/ArrayList.h>
#include <jlj/util/NullAnything.h>

//********************************************************************

#include <cstdlib>
#include <ctime>

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

ConsumersI::~ConsumersI()
{}

//********************************************************************

ConsumersI::ConsumersI()
: consumers(new LinkedHashMapI<String, List<String> >)
, roundrobin(0)
{}

//********************************************************************

void ConsumersI::addConsumer(const String& consumer, 
							 const String& remoteidentifier) throw (Exception)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"ConsumersI::addConsumer() : " + consumer + L" remote id :" + remoteidentifier);

	if (consumers->containsKey(consumer))
	{
		consumers->get(consumer)->add(remoteidentifier);
	}
	else
	{
		List<String> list = new LinkedListI<String>;
		list->add(remoteidentifier);
		consumers->put(consumer, list);
	}
}

//********************************************************************

void ConsumersI::removeConsumer(const String& consumer, 
								const String& remoteidentifier) throw (Exception)
{ 
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L" ConsumersI::removeConsumer() : " + consumer + L" remote id :" + remoteidentifier);

	if (consumers->containsKey(consumer))
	{			
		consumers->get(consumer)->remove(remoteidentifier);

		if (0 == consumers->get(consumer)->size())
		{
			consumers->remove(consumer);
		}
	}
}

//********************************************************************

void ConsumersI::cleanup(Reactor reactor, ExecutorService<int> threadpool) throw (Exception)
{
	Logger logger = Logger::getLogger(jlj::jms::JMS_LOGGER_NAME);

	logger->log(Level::FINEST, L"ConsumersI::cleanup() : consumers = " + String(size()));

	int count = 0;
	Iterator<MapEntry<String, List<String> > > allconsumers = consumers->entrySet()->iterator();
	while (allconsumers->hasNext())
	{
		MapEntry<String, List<String> > c = allconsumers->next();
		allconsumers->remove();
		String consumer = c.key;
		Iterator<String> i = c.value->iterator();
		while (i->hasNext())
		{
			String remoteid = i->next();
			i->remove();
			Connection connection = reactor->getConnectionByRemoteIdentifier(remoteid);
			if (!!connection)                                            
			{
				try
				{
					jmstools::implementation::reactor::AnythingMessage shutdown = new jmstools::implementation::reactor::AnythingMessageI();
					shutdown->setDestinationType(jmstools::implementation::reactor::Message::DEST_CONTROL);
					shutdown->setCommand(jmstools::implementation::reactor::Message::COMMAND_SHUTDOWN);
					threadpool->execute(new ForwarderI(connection, shutdown->convertToAnything(), false));
					Thread::sleep((0 == ++count % 5) ? 2000 : 200);
				}
				catch (const Exception& e)
				{
					logger->log(Level::WARNING, L"ConsumersI::cleanup() : " + e->toString());
				}
			}
		}
	}
}

//********************************************************************

int ConsumersI::size() const
{
	int s = 0;
	Iterator<List<String> > c = consumers->values()->iterator();
	while (c->hasNext())
	{
		Iterator<String> i = c->next()->iterator();
		while (i->hasNext()) 
		{
			i->next();
			++s;
		}
	}
	return s;
}

//********************************************************************

Connection ConsumersI::getConnection(Reactor reactor, 
									 const String& consumer) throw (Exception)
{
	Connection connection;

	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::INFO, L"ConsumersI::getConnection() : consumer = " + consumer + L", size : " + String(consumers->size()));

	if (consumers->containsKey(consumer))
	{
		List<String> remoteidentifiers = new ArrayListI<String>(consumers->get(consumer));

		while (0 < remoteidentifiers->size())
		{
			roundrobin = (++roundrobin) % remoteidentifiers->size();
			String remoteidentifier = remoteidentifiers->get(roundrobin);

			// Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::INFO, L"ConsumersI::getConnection() : size : " + String(remoteidentifiers->size()) + L", remoteid = " + remoteidentifier);

			connection = reactor->getConnectionByRemoteIdentifier(remoteidentifier);

			if (!connection)
			{
				removeConsumer(consumer, remoteidentifier);
				remoteidentifiers = consumers->containsKey(consumer) ? 
					new ArrayListI<String>(consumers->get(consumer)) : new ArrayListI<String>();
			}
			else
			{
				return connection;
			}
		}
	}

	return connection;
}

//********************************************************************

void ConsumersI::cleanStaleConnections(Reactor reactor) throw (Exception)
{
	int before = size();
	Iterator<MapEntry<String, List<String> > > i = consumers->entrySet()->iterator();
	while (i->hasNext())
	{
		MapEntry<String, List<String> > current = i->next();
		String consumer = current->getKey();
		Iterator<String> j =  current->getValue()->iterator(); 
		while (j->hasNext())
		{
			String remoteidentifier = j->next();
    		Connection connection;
			connection = reactor->getConnectionByRemoteIdentifier(remoteidentifier);
			if (!connection)
			{
				j->remove();
				Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->warning(L"Removed stale connection : " + consumer + L", " + remoteidentifier);
			}
		}
	}
	int after = size();
	if (0 < before - after) 
	{
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->warning(L"Removed stale connections : " + String(before - after) + L" of " + String(before));
	}
}

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof