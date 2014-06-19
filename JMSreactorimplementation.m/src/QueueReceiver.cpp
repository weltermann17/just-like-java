// QueueReceiver.cpp

//********************************************************************

#ifndef QueueReceiver_cpp
#define QueueReceiver_cpp

//********************************************************************

#include <jmstools/implementation/reactor/QueueReceiver.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jmstools/implementation/reactor/AnythingMessage.h>
#include <jmstools/implementation/reactor/QueueRequestor.h>

#include <jlj/util/concurrent/Future.h>
#include <jlj/util/concurrent/FutureTask.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/Date.h>
#include <jlj/util/Uuid.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/StringAnything.h>

using namespace jlj::util;
using namespace jlj::util::logging;
using namespace jlj::util::concurrent;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

QueueReceiverI::~QueueReceiverI() throw (jlj::jms::JMSException)
{
	Logger logger = Logger::getLogger(jlj::jms::JMS_LOGGER_NAME);
	try
	{
		queuefillers->removeQueueFiller(queue->getQueueName(), queuefiller);

		if (transportationlayer->hasProvider() && Message::DEST_PERSISTANT_QUEUE == queue->getType())
		{
			QueueRequestor queuerequestor = new QueueRequestorI(session, new QueueI(L"INTERNAL", Message::DEST_INTERNAL));
			AnythingMessage message = new AnythingMessageI();
			message->setCommand(Message::COMMAND_UNREGISTER);
			message->setCommandParameter(Message::COMMAND_REGISTER_QUEUE, new StringAnythingI(queue->getQueueName()));
			queuerequestor->request(message, 2000)->downcast(message);
			if (Message::SUCCESS != message->getErrorCode())
			{
				throw jlj::jms::JMSException(WITHDETAILS(L"Unregistration failed : " + message->getErrorMessage()));
			}
			logger->log(Level::FINEST, L"QueueReceiverI::QueueReceiverI() : unregistered " + queue->toString());	
		}

		logger->log(Level::FINEST, L"QueueReceiverI::~QueueReceiverI(): " + queue->getQueueName());
	}
	catch (const jlj::jms::JMSException&)
	{
		throw;
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"QueueReceiverI::~QueueReceiverI(): " + e->toString());
	}
}

//********************************************************************

QueueReceiverI::QueueReceiverI(Queue queue, QueueSession session, 
							   QueueFillers queuefillers,
							   TransportationLayer transportationlayer) throw (jlj::jms::JMSException)
							   : queue(queue)
							   , session(session)
							   , queuefillers(queuefillers)
							   , queuefiller(new QueueFillerI())
							   , transportationlayer(transportationlayer)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueReceiverI::QueueReceiverI() : " + queue->getQueueName());	

	if (transportationlayer->hasProvider() && Message::DEST_PERSISTANT_QUEUE == queue->getType())
	{
		QueueRequestor queuerequestor = new QueueRequestorI(session, new QueueI(L"INTERNAL", Message::DEST_INTERNAL));
		AnythingMessage message = new AnythingMessageI();
		message->setCommand(Message::COMMAND_REGISTER);
		message->setCommandParameter(Message::COMMAND_REGISTER_QUEUE, new StringAnythingI(queue->getQueueName()));
		queuerequestor->request(message, 2000)->downcast(message);
		if (Message::SUCCESS != message->getErrorCode())
		{
			throw jlj::jms::JMSException(WITHDETAILS(L"Registration failed : " + message->getErrorMessage()));
		}
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueReceiverI::QueueReceiverI() : registered " + queue->toString());	
	}

	queuefillers->addQueueFiller(queue->getQueueName(), queuefiller);
}

//********************************************************************

jlj::jms::Message QueueReceiverI::receive() throw (jlj::jms::JMSException)
{
	Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueReceiverI::QueueReceiverI()");	
	return receive(-1);
}

//********************************************************************

jlj::jms::Message QueueReceiverI::receive(int timeout) throw (jlj::jms::JMSException)
{
	try
	{
		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueReceiverI::receive() : timeout : " + String(timeout) + L", " + queue->toString());	

		jlj::jms::Message result = queuefiller->get(timeout);

		Logger::getLogger(jlj::jms::JMS_LOGGER_NAME)->log(Level::FINEST, L"QueueReceiverI::receive() after get() : timeout : " + String(timeout) + queue->toString());	

		if (!result) throw jlj::jms::JMSException(WITHDETAILS(L"Null message received"));

		return result;
	}
	catch (const Exception& e)
	{
		throw jlj::jms::JMSException(e->toString());
	}
}

//********************************************************************

void QueueReceiverI::setMessageListener(jlj::jms::MessageListener listener) throw (jlj::jms::JMSException)
{
	queuefillers->removeQueueFiller(queue->getQueueName(), queuefiller);
	if (!!listener)
	{
		queuefiller = new QueueFillerI(listener);
		queuefillers->addQueueFiller(queue->getQueueName(), queuefiller);
	}
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
