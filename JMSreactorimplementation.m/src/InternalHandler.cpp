// InternalHandler.cpp

//********************************************************************

#ifndef jmstools_implementation_reactor_InternalHandler_cpp
#define jmstools_implementation_reactor_InternalHandler_cpp

//********************************************************************

#include <jmstools/implementation/reactor/InternalHandler.h>
#include <jmstools/implementation/reactor/Message.h>

#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

InternalHandlerI::~InternalHandlerI()
{}

//********************************************************************

InternalHandlerI::InternalHandlerI(Anything anything)
: anything(anything)
, ishandled(false)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"InternalHandlerI::InternalHandlerI()");
}

//********************************************************************

void InternalHandlerI::handle() throw (Exception)
{
	Logger logger = Logger::getLogger(jlj::jms::JMS_LOGGER_NAME);
	    
	if (logger->isLoggable(Level::FINEST)) logger->log(Level::FINEST, L"InternalMessageHandlerI::handle() : msg = " + anything->toString());

	Message message = new MessageI(anything);
	int errorcode = message->getErrorCode();
    logger->log(Level::INFO, L"InternalMessageHandlerI::handle(): errorcode = " + errorcode);

	switch (errorcode)
	{
		case Message::SUCCESS:
		{
			logger->log(Level::SEVERE, L"InternalHandlerI::handle() : Message::SUCCESS");
			ishandled = true;
			break;
		}
		case Message::FAILURE:
		{
			logger->log(Level::SEVERE, L"InternalHandlerI::handle() : Message::FAILURE : " + message->getErrorMessage());
			ishandled = false;
			break;
		}
        default:
        {
			ishandled = false;
			logger->log(Level::SEVERE, L"InternalHandlerI::handle(): Unknown error code.");
			break;
        }
    }
}

//********************************************************************

bool InternalHandlerI::isHandled() const
{
	return ishandled;
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof