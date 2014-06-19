// AnythingMessage.cpp

//********************************************************************

#ifndef AnythingMessage_cpp
#define AnythingMessage_cpp

//********************************************************************

#include <jmstools/implementation/reactor/AnythingMessage.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

AnythingMessageI::~AnythingMessageI()
{}

//********************************************************************

AnythingMessageI::AnythingMessageI()
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"AnythingMessageI::AnythingMessageI()");
	setMessageType(Message::ANYTHING_MESSAGE);
}

//********************************************************************

AnythingMessageI::AnythingMessageI(Anything anything)
: AbstractMessageI(anything)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"AnythingMessageI::AnythingMessageI(Anything)");
}

//********************************************************************

Anything AnythingMessageI::getAnything() const
{
	return getBody();
}

//********************************************************************

void AnythingMessageI::setAnything(Anything anything)
{
	setBody(anything);
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
