// QueueConnection.cpp

//********************************************************************

#ifndef QueueConnection_cpp
#define QueueConnection_cpp

//********************************************************************

#include <jmstools/implementation/reactor/QueueConnection.h>
#include <jmstools/implementation/reactor/QueueSession.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/nio/reactor/Connector.h>
#include <jlj/net/InetSocketAddress.h>
#include <jlj/util/NullAnything.h>

using namespace jlj::util;
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

QueueConnectionI::~QueueConnectionI()
{
	close();
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueConnectionI::~QueueConnectionI()");	
}

//********************************************************************

QueueConnectionI::QueueConnectionI(TransportationLayer transportationlayer, 
								   QueueFillers queuefillers ) throw (jlj::jms::JMSException)
								   : transportationlayer(transportationlayer)
								   , queuefillers(queuefillers)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueConnectionI::QueueConnectionI()");	
}

//********************************************************************

void QueueConnectionI::start() throw (jlj::jms::JMSException)
{}

//********************************************************************

void QueueConnectionI::stop() throw (jlj::jms::JMSException)
{}

//********************************************************************

void QueueConnectionI::close() throw (jlj::jms::JMSException)
{}

//********************************************************************

jlj::jms::Session QueueConnectionI::createSession() throw (jlj::jms::JMSException)
{
	return createQueueSession();
}

//********************************************************************

jlj::jms::QueueSession QueueConnectionI::createQueueSession() throw (jlj::jms::JMSException)
{
	return new QueueSessionI(transportationlayer, queuefillers);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
