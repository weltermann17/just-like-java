// QueueSender.cpp

//********************************************************************

#ifndef QueueSender_cpp
#define QueueSender_cpp

//********************************************************************

#include <jmstools/implementation/reactor/QueueSender.h>
#include <jmstools/implementation/reactor/QueueConnection.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/util/logging/Logger.h>

using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

QueueSenderI::~QueueSenderI()
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSenderI::~QueueSenderI()");	
}

//********************************************************************

QueueSenderI::QueueSenderI(Queue queue, TransportationLayer transportationlayer)
: queue(queue)
, transportationlayer(transportationlayer)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSenderI::QueueSenderI()");	
}

//********************************************************************

void QueueSenderI::send(jlj::jms::Message message) throw (jlj::jms::JMSException)
{
	try
	{
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueSenderI::send()");	
		
		message->setJMSDestination(queue);

		Message spimessage;
		message->downcast(spimessage);
		transportationlayer->send(spimessage->convertToAnything());
	}
	catch (const Exception& e)
	{
		throw jlj::jms::JMSException(e->getMessage());
	}
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
