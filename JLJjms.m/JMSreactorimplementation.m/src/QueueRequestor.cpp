// QueueRequestor.cpp

//********************************************************************

#ifndef QueueRequestor_cpp
#define QueueRequestor_cpp

//********************************************************************

#include <jmstools/implementation/reactor/QueueRequestor.h>
#include <jlj/util/logging/Logger.h>

using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

QueueRequestorI::~QueueRequestorI()
{}

//********************************************************************

QueueRequestorI::QueueRequestorI(jlj::jms::QueueSession session, jlj::jms::Queue queue) throw (jlj::jms::JMSException) 
{
	temporaryqueue = session->createTemporaryQueue();
	sender = session->createSender(queue);
	receiver = session->createReceiver(temporaryqueue);
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueRequestorI::QueueRequestorI()");
}

//********************************************************************

jlj::jms::Message QueueRequestorI::request(jlj::jms::Message message) throw (jlj::jms::JMSException)
{	
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueRequestorI::request(Message)");
	return request(message, -1);
}

//********************************************************************

jlj::jms::Message QueueRequestorI::request(jlj::jms::Message message, int timeout) throw (jlj::jms::JMSException)
{	
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueRequestorI::request(Message, int)");
	message->setJMSReplyTo(temporaryqueue);
	sender->send(message);
	return receiver->receive(timeout);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof