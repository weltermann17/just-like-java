// QueueFiller.cpp

//********************************************************************

#ifndef QueueFiller_cpp
#define QueueFiller_cpp

//********************************************************************

#include <jmstools/implementation/reactor/QueueFiller.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jmstools/implementation/reactor/AbstractMessage.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/concurrent/LinkedBlockingQueue.h>

using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

QueueFillerI::~QueueFillerI()
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueFillerI::~QueueFillerI()");
}

//********************************************************************

QueueFillerI::QueueFillerI(jlj::jms::MessageListener listener)
: listener(listener)
, timeout(-1)
, count(0)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueFillerI::QueueFillerI(MessageListener)");
}

//********************************************************************

QueueFillerI::QueueFillerI()
: queue(new LinkedBlockingQueueI<jlj::jms::Message>())
, timeout(-1)
, count(0)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueFillerI::QueueFillerI()");
}

//********************************************************************

jlj::jms::Message QueueFillerI::get(int timeout) const throw (jlj::lang::Exception)
{
	this->timeout->set(timeout);

	if (!!listener) throw jlj::lang::IllegalStateException(WITHDETAILS(L"Must not call get() with a listener"));

	if (0 == timeout)
	{
		return queue->take();
	}
	else
	{
		return queue->poll(timeout);
	}
}

//********************************************************************
//********************************************************************

class ExportedByJMSreactorimplementation MessageDelegatorI : public RunnableI
{
protected:
	~MessageDelegatorI() {}
	friend class TypedReference<MessageDelegatorI>; 

public:
	MessageDelegatorI(Anything anything, jlj::jms::MessageListener listener)
		: anything(anything)
		, listener(listener) {}

	void run()
	{
		Message message = new MessageI(anything);
		jlj::jms::Message jmsmessage;
		if (Message::SUCCESS == message->getErrorCode())
		{
			jmsmessage = message->convertToJMSMessage();
		}
		listener->onMessage(jmsmessage);
	}

private:
	Anything anything;
	jlj::jms::MessageListener listener;
};

StrongReferenceCommonDeclaration(MessageDelegator, ExportedByJMSreactorimplementation);

//********************************************************************

void QueueFillerI::put(ExecutorService<jlj::jms::Message> threadpool, Anything anything) throw (jlj::jms::JMSException)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueFillerI::put()");

	Anything clone;
	anything->clone()->downcast(clone);
	anything.reset(0);

	if (!listener)
	{
		Message message = new MessageI(clone);
		jlj::jms::Message jmsmessage;
		if (Message::SUCCESS == message->getErrorCode())
		{
			jmsmessage = message->convertToJMSMessage();
		}

		queue->put(jmsmessage);
	}
	else
	{
		threadpool->execute(new MessageDelegatorI(clone, listener));
	}	
}

//********************************************************************

bool QueueFillerI::hasExpired() const
{
	if (0 == timeout)
	{
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::INFO, L"QueueFillerI::hasExpired() : timeout : " + String(timeout) + L", count : " + String(count));
		return 1 < ++count;
	}
	else
	{
		return false;
	}

}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
