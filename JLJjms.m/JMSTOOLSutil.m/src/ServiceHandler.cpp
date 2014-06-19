// ServiceHandler.cpp

//********************************************************************

#ifndef ServiceHandler_cpp
#define ServiceHandler_cpp

//********************************************************************

#include <jmstools/util/services/ServiceHandler.h>

//********************************************************************

#include <jlj/util/Collections.h>
#include <jlj/util/LinkedHashMap.h>
#include <jlj/util/LinkedHashSet.h>
#include <jlj/util/StopWatch.h>
#include <jlj/util/Date.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/logging/ConsoleHandler.h>
#include <jlj/util/logging/StreamHandler.h>
using namespace jlj::util::logging;
#include <jlj/util/concurrent/LinkedBlockingQueue.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/NullAnything.h>
using namespace jlj::util;
#include <jlj/jms/TextMessage.h>
#include <jlj/jms/MessageProducer.h>
#include <jlj/jms/QueueConnectionFactory.h>
using namespace jlj::jms;
#include <jlj/naming/InitialContext.h>
using namespace jlj::naming;
#include <signal.h>

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(util)
NAMESPACE_BEGIN(services)

//********************************************************************

AtomicInteger ServiceHandlerI::countrunning = new AtomicIntegerI(0);

//********************************************************************

ServiceHandlerI::~ServiceHandlerI() 
{
	countrunning->decrementAndGet();
	logger->log(Level::FINEST, L"ServiceHandlerI::~ServiceHandlerI()");
}

//********************************************************************

ServiceHandlerI::ServiceHandlerI(TextMessage textmessage, 
								 ServiceImplementor serviceimplementor,
								 QueueSession queuesession,
								 Logger logger) 
								 : textmessage(textmessage)
								 , serviceimplementor(serviceimplementor)
								 , queuesession(queuesession)
								 , logger(logger)
{
	countrunning->incrementAndGet();
}

//********************************************************************

void ServiceHandlerI::run() 
{
	Destination replyto;
	try
	{
		replyto = textmessage->getJMSReplyTo();
		if (!replyto)
		{
			serviceimplementor->doService(textmessage->getText());
		}
		else
		{
			if (logger->isLoggable(Level::FINEST)) logger->log(Level::FINEST, L"ServiceHandlerI::run() text: " + textmessage->getText());
			String replytext = serviceimplementor->doService(textmessage->getText());
			if (logger->isLoggable(Level::FINEST)) logger->log(Level::FINEST, L"ServiceHandlerI::run() replytext: " + replytext);
			TextMessage reply = queuesession->createTextMessage(replytext);

			Iterator<MapEntry<String, Anything> > i = serviceimplementor->getOutParameters()->entrySet()->iterator();
			while (i->hasNext())
			{
				MapEntry<String, Anything> current = i->next();
				reply->setStringProperty(current->key, current->value);
			}

			MessageProducer sender = queuesession->createProducer(replyto);
			sender->send(reply);
		}
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, e->toString());
		if (!!replyto)
		{
			try
			{
				MessageProducer sender = queuesession->createProducer(replyto);
				sender->send(queuesession->createTextMessage(e->toString()));
			}
			catch (const Exception& e)
			{
				logger->log(Level::SEVERE, e->toString());
			}
		}
	}
}

//********************************************************************

int ServiceHandlerI::getCountRunning()  
{
	return countrunning->get(); 
}

//********************************************************************

NAMESPACE_END(services)
NAMESPACE_END(util)
NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
