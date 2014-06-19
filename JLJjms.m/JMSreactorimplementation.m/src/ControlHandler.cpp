// ControlHandler.cpp

//********************************************************************

#ifndef jmstools_implementation_reactor_ControlHandler_cpp
#define jmstools_implementation_reactor_ControlHandler_cpp

//********************************************************************

#include <jmstools/implementation/reactor/ControlHandler.h>
#include <jlj/lang/String.h>
#include <jlj/util/StringAnything.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/util/logging/Logger.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

ControlHandlerI::~ControlHandlerI()
{}

//********************************************************************

ControlHandlerI::ControlHandlerI(TransportationLayer transportationlayer, Anything anything)
: anything(anything)
, transportationlayer(transportationlayer)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"ControlMessageHandlerI::ControlMessageHandlerI()");
}

//********************************************************************

void ControlHandlerI::handle() throw (Exception)
{
	Logger logger = Logger::getLogger(jlj::jms::JMS_LOGGER_NAME);
	logger->log(Level::FINEST, L"ControlMessageHandlerI::handle()");

	Message message = new MessageI(anything);
	String command = message->getCommand();
	logger->log(Level::INFO, L"ControlMessageHandlerI::handle() Command: " + command);

	if (Message::COMMAND_SHUTDOWN == command)
	{
		transportationlayer->finalize();
	}
	else if (Message::COMMAND_SETGLOBALLOGLEVEL  == command)
	{
	}
	else if (Message::COMMAND_SETREACTORLOGLEVEL  == command)
	{                        	
	} 
	else if (Message::COMMAND_SETLOGLEVEL  == command)
	{
	}
	else
	{
		logger->log(Level::WARNING, L"Command does not exist");
	}
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof