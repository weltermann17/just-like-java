// Queue.cpp

//********************************************************************

#ifndef Queue_cpp
#define Queue_cpp

//********************************************************************

#include <jmstools/implementation/reactor/Queue.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/util/logging/Logger.h>

using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

QueueI::~QueueI()
{}

//********************************************************************

QueueI::QueueI(const String& name)
: name(name)
, type(Message::DEST_PERSISTANT_QUEUE)
, jsonencoding(false)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueI::QueueI() : "+ name);
}

//********************************************************************

QueueI::QueueI(const String& name, int type)
: name(name)
, type(type)
, jsonencoding(false)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueI::QueueI()");
}

//********************************************************************

QueueI::QueueI(const String& name, int type, const String& identifier, bool jsonencoding)
: name(name)
, type(type)
, identifier(identifier)
, jsonencoding(jsonencoding)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueI::QueueI()");
}

//********************************************************************

String QueueI::getQueueName() const
{
	return name;
}

//********************************************************************

String QueueI::toString() const
{
	return L"QueueI<" + name + L">";
}

//********************************************************************

int QueueI::getType() const
{
	return type;
}

//********************************************************************

String QueueI::getIdentifier() const
{
	return identifier;
}

//********************************************************************

bool QueueI::getJsonEncoding() const
{
	return jsonencoding;
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
