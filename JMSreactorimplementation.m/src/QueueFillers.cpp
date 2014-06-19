// QueueFillers.cpp

//********************************************************************

#ifndef QueueFillers_cpp
#define QueueFillers_cpp

//********************************************************************

#include <jmstools/implementation/reactor/QueueFillers.h>

#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/LinkedList.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util;
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

QueueFillersI::~QueueFillersI()
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueFillersI::~QueueFillersI()");
}

//********************************************************************

QueueFillersI::QueueFillersI()
: roundrobin(0)
{	
	Map<String, SynchronizedList<QueueFiller> > map = new LinkedHashMapI<String, SynchronizedList<QueueFiller> >();
	queuefillers = Collections::synchronizedMap(map);
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueFillersI::QueueFillersI()");
}

//********************************************************************

void QueueFillersI::addQueueFiller(const String& queuename, QueueFiller queuefiller)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::INFO, L"QueueFillersI::addQueueFiller(): queuename: " + queuename);

	if (queuefillers->containsKey(queuename))
	{
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::INFO, L"QueueFillersI::addQueueFiller(): key already exist, add QueueFiller to the list");
		queuefillers->get(queuename)->add(queuefiller);
	}
	else
	{
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::INFO, L"QueueFillersI::addQueueFiller(): new entry");
		List<QueueFiller> l = new LinkedListI<QueueFiller>();
		SynchronizedList<QueueFiller> list = Collections::synchronizedList(l);
		list->add(queuefiller);
		queuefillers->put(queuename, list);
	}
}

//********************************************************************

void QueueFillersI::removeQueueFiller(const String& queuename, QueueFiller queuefiller)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::INFO, L"QueueFillersI::removeQueueFiller(): queuename: " + queuename);

	if (queuefillers->containsKey(queuename))
	{			
		queuefillers->get(queuename)->remove(queuefiller);
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::INFO, L"QueueFillersI::removeQueueFiller(): remove queuename: " + queuename);

		if (0 == queuefillers->get(queuename)->size())
		{
			queuefillers->remove(queuename);
			Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::INFO, L"QueueFillersI::removeQueueFiller(): removed with entry : " + queuename);
		}
	}
}

//********************************************************************

QueueFiller QueueFillersI::getQueueFiller(const String& queuename) throw (Exception)
{
	List<QueueFiller> copy; 
	synchronized(queuefillers->get(queuename)->getLock())
	{
		copy = new LinkedListI<QueueFiller>(queuefillers->get(queuename));
	}
	roundrobin->set(++roundrobin % copy->size());
	return copy->get(roundrobin);
}

//********************************************************************

void QueueFillersI::broadcast(ExecutorService<jlj::jms::Message> threadpool, 
							  Message message) throw (Exception)
{
	synchronized(queuefillers->getLock())
	{
		Iterator<SynchronizedList<QueueFiller> > i = queuefillers->values()->iterator();
		while (i->hasNext())
		{
			Iterator<QueueFiller> j = i->next()->iterator();
			while (j->hasNext())
			{
				QueueFiller current = j->next();
				current->put(threadpool, message->convertToAnything());
			}
		}
	}
}

//********************************************************************

void QueueFillersI::broadcastAbort(ExecutorService<jlj::jms::Message> threadpool, int timeout) throw (Exception)
{
	Message message = new MessageI();
	message->setErrorCode(Message::FAILURE);
	message->setErrorMessage(L"QueueFillersI::broadcastAbort()");

	broadcast(threadpool, message);

	int total = 0;
	const int INTERVAL = 50;

	while (0 < queuefillers->size() && total < timeout)
	{
		Thread::sleep(INTERVAL);
		total += INTERVAL;
	}
}

//********************************************************************

void QueueFillersI::broadcastToExpired(ExecutorService<jlj::jms::Message> threadpool, 
									   Message message) throw (Exception)
{
	synchronized(queuefillers->getLock())
	{
		Iterator<SynchronizedList<QueueFiller> > i = queuefillers->values()->iterator();
		while (i->hasNext())
		{
			Iterator<QueueFiller> j = i->next()->iterator();
			while (j->hasNext())
			{
				QueueFiller current = j->next();
				if (current->hasExpired())
				{
					current->put(threadpool, message->convertToAnything());
				}
			}
		}
	}
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
