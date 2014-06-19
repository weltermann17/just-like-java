// Dispatcher.cpp

//********************************************************************

#ifndef jmstools_implementation_reactor_Dispatcher_cpp
#define jmstools_implementation_reactor_Dispatcher_cpp

//********************************************************************

#include <jlj/jms/Message.h> 
#include <jmstools/implementation/reactor/Dispatcher.h>
#include <jmstools/implementation/reactor/Message.h> 
#include <jmstools/implementation/reactor/InternalHandler.h>
#include <jmstools/implementation/reactor/ControlHandler.h>
#include <jmstools/implementation/reactor/QueueFiller.h>
#include <jlj/nio/reactor/Connection.h>
#include <jlj/net/InetAddress.h>
#include <jlj/util/concurrent/ThreadPoolExecutor.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/logging/ConsoleHandler.h>
using namespace jlj::util::logging;

//********************************************************************

NAMESPACE_BEGIN(jmstools)
NAMESPACE_BEGIN(implementation)
NAMESPACE_BEGIN(reactor)

//********************************************************************

DispatcherI::~DispatcherI()
{
	logger->log(Level::FINEST, L"DispatcherI::~DispatcherI()");
}

//********************************************************************

DispatcherI::DispatcherI(ExecutorService<jlj::jms::Message> threadpool,
						 TransportationLayer transportationlayer,
						 QueueFillers queuefillers,
						 BlockingQueue<int> dispatchingstopped, 
						 int dispatchertimeout)
						 : threadpool(threadpool)
						 , transportationlayer(transportationlayer)
						 , queuefillers(queuefillers)
						 , dispatchingstopped(dispatchingstopped)
						 , dispatchertimeout(dispatchertimeout)
						 , polled(0)
						 , handled(0)
						 , dropped(0)
						 , logger(Logger::getLogger(jlj::jms::JMS_LOGGER_NAME))
{
	logger->log(Level::FINEST, L"DispatcherI::DispatcherI()");
}	

//*******************************************************************

void DispatcherI::dispatch() throw (Exception)
{
	int state = INITIAL;
	bool aborted = false;
	Anything anything;
	String errormsg;

// #define LOGSTATE(s) logger->log(Level::FINEST, s);
#define LOGSTATE(s)

	while (!aborted)
	{
		try
		{
			switch (state)
			{
			case INITIAL:
				{
					LOGSTATE(L"INITIAL");
					state = POLLING;
					break;
				}
			case POLLING:
				{
					LOGSTATE(L"POLLING");
					anything = transportationlayer->receive(dispatchertimeout);
					state = !anything ? TIMEOUT : RECEIVED;
					break;
				}
			case TIMEOUT:
				{
					LOGSTATE(L"TIMEOUT");
					Message message = new MessageI();
					message->setErrorCode(Message::FAILURE);
					message->setErrorMessage(L"Timeout after msec " + String(dispatchertimeout));
					queuefillers->broadcastToExpired(threadpool, message);
					state = CLEANED;
					break;
				}
			case CLEANED:
				{
					LOGSTATE(L"CLEANED");
					state = POLLING;
					break;
				}
			case RECEIVED:
				{
					LOGSTATE(L"RECEIVED");
					++polled;
					state = anything->isNull() ? NULL_RECEIVED : TYPE_DETERMINED;
					break;
				}
			case NULL_RECEIVED:
				{
					LOGSTATE(L"NULL_RECEIVED");
					state = ABORTED;
					break;
				}
			case TYPE_DETERMINED:
				{
					LOGSTATE(L"TYPE_DETERMINED");
					logger->log(Level::FINEST, anything->toString());
					Message message	= new MessageI(anything);
					state = determineType(message->getDestinationType());
					break;
				}
			case CONTROL_RECEIVED:
				{
					LOGSTATE(L"CONTROL_RECEIVED");
					ControlHandler handler = new ControlHandlerI(transportationlayer, anything);
					handler->handle();
					state = HANDLED;
					break;
				}
			case QUEUE_RECEIVED:
				{
					LOGSTATE(L"QUEUE_RECEIVED");
					Message message	= new MessageI(anything);
					QueueFiller queuefiller = queuefillers->getQueueFiller(message->getDestinationName());
					if (!queuefiller)
					{
						logger->log(Level::WARNING, L"DispatcherI::dispatch() : no queuefiller found : " + message->getDestinationName());
						state = DROPPED;
					}
					else
					{
						queuefiller->put(threadpool, anything);
						state = HANDLED;
					}
					break;
				}
			case DROPPED:
				{
					LOGSTATE(L"DROPPED");
					++dropped;
					state = POLLING;
					break;
				}
			case HANDLED:
				{
					LOGSTATE(L"HANDLED");
					++handled;				
					state = POLLING;
					break;
				}
			case TOPIC_RECEIVED:
				{
					LOGSTATE(L"TOPIC_RECEIVED");
					state = POLLING;
					break;
				}
			case IOEXCEPTION:
				{
					LOGSTATE(L"IOEXCEPTION");
					Message message = new MessageI();
					message->setErrorCode(Message::FAILURE);
					message->setErrorMessage(errormsg);
					queuefillers->broadcast(threadpool, message);
					state = POLLING;
					break;
				}
			case FINAL:
			case ABORTED:
			default:
				{
					LOGSTATE(L"ABORTED");
					queuefillers->broadcastAbort(threadpool, dispatchertimeout);
					aborted = true;
					break;
				}
			}
		}
		catch (const IOException& e)
		{
			errormsg = e->toString();
			logger->log(Level::INFO, L"DispatcherI::dispatch() : " + errormsg);
			state = IOEXCEPTION;
		}
		catch (const Exception& e)
		{
			logger->log(Level::SEVERE, L"DispatcherI::dispatch() : " + e->toString());
			state = POLLING;
		}
		catch (...)
		{
			logger->log(Level::SEVERE, L"DispatcherI::dispatch() : Unknown exception");
			state = POLLING;
		}
	}
	                                                                        
	transportationlayer.reset(0);
	dispatchingstopped->put(-1);

	logger->log(Level::FINEST, L"DispatcherI::dispatch() : exit");
}

//********************************************************************

int DispatcherI::determineType(int type)
{
	switch (type)
	{
	case Message::DEST_CONTROL:
		{
			return CONTROL_RECEIVED;
		}
	case Message::DEST_PERSISTANT_QUEUE:
	case Message::DEST_TEMPORARY_QUEUE:
		{
			return QUEUE_RECEIVED;
		} 
	case Message::DEST_PERSISTANT_TOPIC:
	case Message::DEST_TEMPORARY_TOPIC:
		{
			return TOPIC_RECEIVED;
		}
	default:
		{
			return POLLING;
		}
	}
}

//********************************************************************

void DispatcherI::run()
{
	dispatch();
}

//********************************************************************

NAMESPACE_END(reactor)
NAMESPACE_END(implementation)
NAMESPACE_END(jmstools)

#endif

// eof
