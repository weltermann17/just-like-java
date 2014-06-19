// AbstractMessage.cpp

//********************************************************************

#ifndef jmstools_implementation_reactor_AbstractMessage_cpp
#define jmstools_implementation_reactor_AbstractMessage_cpp

//********************************************************************

#include <jmstools/implementation/reactor/AbstractMessage.h>
#include <jmstools/implementation/reactor/Queue.h>
#include <jlj/jms/Destination.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/LinkedList.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

AbstractMessageI::~AbstractMessageI()
{}

//********************************************************************

AbstractMessageI::AbstractMessageI()
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"AbstractMessageI::AbstractMessageI()");
}

//********************************************************************

AbstractMessageI::AbstractMessageI(Anything anything)
: jmstools::implementation::reactor::MessageI(anything)
{}

//********************************************************************

jlj::jms::Destination AbstractMessageI::getJMSDestination() const throw (jlj::jms::JMSException)
{	
	try 
	{
		return new QueueI(getDestinationName(), getDestinationType(), getDestinationRemoteID(), getDestinationJsonEncoding());
	}
	catch (const Exception& e)
	{
		throw jlj::jms::JMSException(WITHDETAILS(e->toString()));
	}
}

//********************************************************************

void AbstractMessageI::setJMSDestination(jlj::jms::Destination destination) throw (jlj::jms::JMSException)
{
    Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"AbstractMessageI::setJMSDestination :" + destination->toString());
	Queue queue;
	if (destination->instanceOf(queue))
	{
		setDestinationName(queue->getQueueName());
		setDestinationType(queue->getType());
		setDestinationRemoteID(queue->getIdentifier());
		setDestinationJsonEncoding(queue->getJsonEncoding());
	}
	else
	{
		throw jlj::jms::JMSException(L"Can not downdcast to spi::Queue");
	}
}

//********************************************************************

bool AbstractMessageI::propertyExists(const String& name) const throw (jlj::jms::JMSException)
{
	return jmstools::implementation::reactor::MessageI::propertyExists(name);
}

//********************************************************************

List<String> AbstractMessageI::getPropertyNames() const throw (jlj::jms::JMSException)
{
	return jmstools::implementation::reactor::MessageI::getPropertyNames();
}

//********************************************************************

String AbstractMessageI::getStringProperty(const String& name) const throw (jlj::jms::JMSException)
{	
	if (!propertyExists(name)) throw jlj::jms::JMSException(WITHDETAILS(L"property does not exist : " + name));
	return jmstools::implementation::reactor::MessageI::getStringProperty(name);
}

//********************************************************************

void AbstractMessageI::setStringProperty(const String& name, const String& value) throw (jlj::jms::JMSException)
{
	jmstools::implementation::reactor::MessageI::setStringProperty(name, value);
}

//********************************************************************

jlj::jms::Destination AbstractMessageI::getJMSReplyTo() const throw (jlj::jms::JMSException)
{	
	try 
	{
		return new QueueI(getReplyToName(), getReplyToType(), getReplyToRemoteID(), getReplyToJsonEncoding());
	}
	catch (const Exception& e)
	{
		throw jlj::jms::JMSException(WITHDETAILS(e->toString()));
	}
}

//********************************************************************

void AbstractMessageI::setJMSReplyTo(jlj::jms::Destination replyto) throw (jlj::jms::JMSException)
{
	Queue queue;
	if (replyto->instanceOf(queue))
	{
		setReplyToName(queue->getQueueName());
		setReplyToType(queue->getType());
		setReplyToRemoteID(queue->getIdentifier());
		setReplyToJsonEncoding(queue->getJsonEncoding());
	}
	else
	{
		throw jlj::jms::JMSException(L"Can not downdcast to spi::Queue");
	}
}


//********************************************************************

String AbstractMessageI::getJMSMessageID() const throw (jlj::jms::JMSException)
{
	return getMessageID();
}

//********************************************************************

double AbstractMessageI::getJMSTimestamp() const throw (jlj::jms::JMSException)
{
	return getTimestamp();
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
