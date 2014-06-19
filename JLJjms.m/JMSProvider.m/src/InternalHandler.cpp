// InternalHandler.cpp

//********************************************************************

#ifndef jmstools_jmsprovider_InternalHandler_cpp
#define jmstools_jmsprovider_InternalHandler_cpp

//********************************************************************

#include <InternalHandler.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

InternalHandlerI::~InternalHandlerI()
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"InternalHandlerI::~InternalHandlerI()");
}

//********************************************************************

InternalHandlerI::InternalHandlerI(Consumers consumers, jlj::nio::reactor::Connection connection, Message message)
: message(message)
, consumers(consumers)
, connection(connection)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"InternalHandlerI::InternalHandlerI()");
}

//********************************************************************

void InternalHandlerI::handle() throw (Exception)
{
	String queue = message->getCommandParameter(Message::COMMAND_REGISTER_QUEUE);

	switch(message->getCommand())
	{
	case Message::COMMAND_REGISTER:
		{
			consumers->addConsumer(queue, message->getReplyToRemoteID());
			Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"InternalHandlerI::handle() : REGISTER");
			sendAcknowledgment(Message::SUCCESS);
			break;
		}
	case Message::COMMAND_UNREGISTER:
		{
			consumers->removeConsumer(queue, message->getReplyToRemoteID());
			Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"InternalHandlerI::handle() : UNREGISTER");
			sendAcknowledgment(Message::SUCCESS);
			break;
		}
	case Message::COMMAND_NOT_DETERMINED:
	default:
		{
			sendAcknowledgment(Message::FAILURE);
			Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::SEVERE, L"InternalHandlerI::handle() cannot handle the command");
		}
	}         
}

//********************************************************************

void InternalHandlerI::sendAcknowledgment(int errorcode) throw (Exception)
{
	Message messageback = new MessageI();
	messageback->setDestinationName(message->getReplyToName());
	messageback->setDestinationType(message->getReplyToType());
	messageback->setDestinationJsonEncoding(message->getReplyToJsonEncoding());
	messageback->setMessageType(message->getMessageType());
	messageback->setErrorCode(errorcode);
	connection->send(messageback->convertToAnything(), messageback->getDestinationJsonEncoding());
}

//********************************************************************

NAMESPACE_END(jms)

#endif

// eof