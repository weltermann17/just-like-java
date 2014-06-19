// QueueSession.cpp

//********************************************************************

#ifndef QueueSession_cpp
#define QueueSession_cpp

//********************************************************************

#include <jmstools/implementation/reactor/QueueSession.h>
#include <jmstools/implementation/reactor/TemporaryQueue.h>
#include <jmstools/implementation/reactor/QueueReceiver.h>
#include <jmstools/implementation/reactor/QueueSender.h>
#include <jmstools/implementation/reactor/QueueRequestor.h>
#include <jmstools/implementation/reactor/AnythingMessage.h>
#include <jmstools/implementation/reactor/TextMessage.h>

#include <jlj/util/StringAnything.h>
#include <jlj/util/logging/Logger.h> 
#include <jlj/util/Uuid.h>
#include <jlj/util/Date.h>

using namespace jlj::util;
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

QueueSessionI::~QueueSessionI()
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::~QueueSession()");
}

//********************************************************************

QueueSessionI::QueueSessionI(TransportationLayer transportationlayer, QueueFillers queuefillers) throw (jlj::jms::JMSException)
: transportationlayer(transportationlayer)
, queuefillers(queuefillers)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::QueueSession()");	
}

//********************************************************************

jlj::jms::MessageConsumer QueueSessionI::createConsumer(jlj::jms::Destination destination) throw (jlj::jms::JMSException)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::createConsumer(Destination)");
	try
	{
		Queue queue;
		destination->downcast(queue);

		return new QueueReceiverI(queue, this, queuefillers, transportationlayer);
	}
	catch (const Exception& e)
	{
		throw jlj::jms::JMSException(e->getMessage());
	}
}

//********************************************************************

jlj::jms::MessageProducer QueueSessionI::createProducer(jlj::jms::Destination destination) throw (jlj::jms::JMSException)
{	
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::createProducer(Destination)");
	try
	{
		Queue queue;
		destination->downcast(queue);
		return new QueueSenderI(queue, transportationlayer);
	}
	catch (const Exception& e)
	{
		throw jlj::jms::JMSException(e->getMessage());
	}
}

//********************************************************************

jlj::jms::AnythingMessage QueueSessionI::createAnythingMessage(Anything anything) throw (jlj::jms::JMSException)
{	
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::createAnythingMessage(Anything body)");

	AnythingMessage message = new AnythingMessageI();
	message->setAnything(anything);

	return message;
}

//********************************************************************

jlj::jms::TextMessage QueueSessionI::createTextMessage(const String& text) throw (jlj::jms::JMSException)
{		
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::createTextMessage(String body)");

	TextMessage message = new TextMessageI();
	message->setText(text);

	return message;
}

//********************************************************************

jlj::jms::Queue QueueSessionI::createQueue(const String& name) throw (jlj::jms::JMSException)
{ 
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::createQueue()");
	return new QueueI(name);
}

//********************************************************************

jlj::jms::TemporaryQueue QueueSessionI::createTemporaryQueue() throw (jlj::jms::JMSException)
{ 
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::createTemporaryQueue()");
	return new TemporaryQueueI(Uuid::randomUuid()->toString()->toLowerCase());
}

//********************************************************************

jlj::jms::QueueReceiver QueueSessionI::createReceiver(jlj::jms::Queue queue) throw (jlj::jms::JMSException)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::createReceiver(Queue queue)");	
	try
	{
		Queue spiqueue;
		queue->downcast(spiqueue);
		return new QueueReceiverI(spiqueue, this, queuefillers, transportationlayer);
	}
	catch (const Exception& e)
	{
		throw jlj::jms::JMSException(e->getMessage());
	}
}

//********************************************************************

jlj::jms::QueueSender QueueSessionI::createSender(jlj::jms::Queue queue) throw (jlj::jms::JMSException)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::createSender(Queue queue)");	
	try
	{
		Queue spiqueue;
		queue->downcast(spiqueue);
		return new QueueSenderI(spiqueue, transportationlayer);
	}
	catch (const Exception& e)
	{
		throw jlj::jms::JMSException(e->getMessage());
	}
}

//********************************************************************

jlj::jms::QueueRequestor QueueSessionI::createQueueRequestor(jlj::jms::Queue queue) throw (jlj::jms::JMSException)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSessionI::createQueueRequestor(Queue queue)");	
	return new QueueRequestorI(this, queue);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
