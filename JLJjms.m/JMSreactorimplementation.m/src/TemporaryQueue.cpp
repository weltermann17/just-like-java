// TemporaryQueue.cpp

//********************************************************************

#ifndef TemporaryQueue_cpp
#define TemporaryQueue_cpp

//********************************************************************

#include <jmstools/implementation/reactor/TemporaryQueue.h> 
#include <jmstools/implementation/reactor/Message.h> 
#include <jlj/util/logging/Logger.h> 
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

TemporaryQueueI::~TemporaryQueueI()
{}

//********************************************************************

TemporaryQueueI::TemporaryQueueI(const String& name)
: QueueI(name, Message::DEST_TEMPORARY_QUEUE)
{	
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"TemporaryQueueI::TemporaryQueueI()");
}

//********************************************************************

String TemporaryQueueI::toString() const
{
	return L"TemporaryQueueI<" + getQueueName() + L">";
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
