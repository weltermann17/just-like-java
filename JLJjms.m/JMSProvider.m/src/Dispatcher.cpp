//DispatcherI.cpp

//********************************************************************

#ifndef jmstools_jmsprovider_Dispatcher_cpp
#define jmstools_jmsprovider_Dispatcher_cpp

//********************************************************************

#include <Dispatcher.h>
#include <InternalHandler.h>
#include <ControlHandler.h>
#include <Forwarder.h>
#include <Consumers.h>
#include <jlj/nio/reactor/Connection.h>
#include <jlj/nio/reactor/Acceptor.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jlj/io/FileOutputStream.h>
#include <jlj/net/InetAddress.h>
#include <jlj/util/concurrent/ThreadPoolExecutor.h>
#include <jlj/util/List.h>
#include <jlj/util/NullAnything.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/logging/Level.h>
#include <jlj/util/logging/StreamHandler.h>
#include <jlj/util/logging/ConsoleHandler.h>
using namespace jlj::util::logging;
using namespace jmstools::implementation::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

DispatcherI::~DispatcherI()
{
	logger->log(Level::FINEST, L"DispatcherI::~DispatcherI()");
}

//********************************************************************

DispatcherI::DispatcherI(int minimumpoolsize,
						 int maximumpoolsize,
						 int keepalivetime,
						 int reactortimeout,
						 int acceptortimeout,
						 int dispatchertimeout,
						 const String& providerhost,
						 int providerport) throw (jlj::lang::Exception)
						 : dispatchertimeout(dispatchertimeout)
						 , dispatchingstopped(new LinkedBlockingQueueI<int>())
						 , logger(Logger::getLogger(jlj::jms::JMS_LOGGER_NAME))
{
	threadpool = new ThreadPoolExecutorI<int>(
		minimumpoolsize, 
		maximumpoolsize, 
		keepalivetime, 
		new concurrent::LinkedBlockingQueueI<Runnable>());

	reactor = jlj::nio::reactor::Reactor::open(threadpool, reactortimeout);

	const int backlog = 0; // not used
	StringBuffer hosts;
	hosts->append(providerhost);
	if (-1 < hosts->indexOf(L",")) 
	{
		int i = 0;
		int b = 0;
		while (-1 < b)
		{
			int e = hosts->indexOf(L",", b);
			String host = -1 == e ? hosts->substring(b) : hosts->substring(b, e);
			b = -1 == e ? -1 : e + 1;
			InetAddress inetaddress = InetAddress::getByName(host);
			Acceptor acceptor = jlj::nio::reactor::Acceptor::open(providerport, backlog, inetaddress); 
			acceptor->setTimeout(acceptortimeout);
			acceptor->registerWith(reactor);
			logger->all(L"Bind address [" + String(++i) + L"] : " + inetaddress->getHostName() + L"(" + inetaddress->getHostAddress() + L"):" + String(providerport));
		}
	}
	else 
	{
		InetAddress inetaddress = InetAddress::getByName(providerhost);
		Acceptor acceptor = jlj::nio::reactor::Acceptor::open(providerport, backlog, inetaddress); 
		acceptor->setTimeout(acceptortimeout);
		acceptor->registerWith(reactor);
		logger->all(L"Bind address : " + inetaddress->getHostName() + L"(" + inetaddress->getHostAddress() + L"):" + String(providerport));
	}
	logger->finest(L"DispatcherI::DispatcherI()");
}	

//********************************************************************

void DispatcherI::dispatch() throw (jlj::lang::Exception)
{
	bool aborted = false;
	int state = INITIAL;
	Anything anything;
	Message message;
	Connection connection;
	Consumers consumers = new ConsumersI();
	int dropped = 0;
	int handled = 0;
	int averagesizepersistantqueue = 0;
	int averagesizetemporaryqueue = 0;

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
					anything = reactor->receiveQueue()->poll(dispatchertimeout);
					state = !anything ? TIMEOUT : RECEIVED;
					break;
				}
			case TIMEOUT:
				{
					LOGSTATE(L"TIMEOUT");
					// cleaning
					anything.reset(0);
					message.reset(0);
					connection.reset(0);

					consumers->cleanStaleConnections(reactor);
					
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
					message = new MessageI(anything);
					state = determineType(message->getDestinationType());
					break;
				}
			case CONTROL_RECEIVED:
				{
					LOGSTATE(L"CONTROL_RECEIVED");
					connection = reactor->getConnectionByRemoteIdentifier(message->getReplyToRemoteID());
					Handler handler = new ControlHandlerI(threadpool, reactor, consumers, connection, message, dropped, handled);
					handler->handle();
					state = HANDLED;
					break;
				}
			case INTERNAL_RECEIVED: 
				{
					LOGSTATE(L"INTERNAL_RECEIVED");
					connection = reactor->getConnectionByRemoteIdentifier(message->getReplyToRemoteID());
					state = !connection ? BAD_DESTINATION_CONNECTION : GOOD_DESTINATION_CONNECTION; 
					break;
				}
			case GOOD_DESTINATION_CONNECTION:
				{  
					LOGSTATE(L"GOOD_DESTINATION_CONNECTION");
					Handler handler = new InternalHandlerI(consumers, connection, message);
					handler->handle();
					state = HANDLED;
					break;
				}
			case BAD_DESTINATION_CONNECTION:
				{
					LOGSTATE(L"BAD_DESTINATION_CONNECTION");
					state = DROPPED;
					break;
				}
			case PERSISTANT_QUEUE_RECEIVED:
				{
					LOGSTATE(L"PERSISTANT_QUEUE_RECEIVED");
					connection = consumers->getConnection(reactor, message->getDestinationName());
					state = !connection ? BAD_CONNECTION : GOOD_CONNECTION;
					break;
				}
			case TEMPORARY_QUEUE_RECEIVED:
				{
					LOGSTATE(L"TEMPORARY_QUEUE_RECEIVED");
					connection = reactor->getConnectionByRemoteIdentifier(message->getDestinationRemoteID());
					state = !connection ? BAD_CONNECTION : GOOD_CONNECTION; 
					break;
				}
			case GOOD_CONNECTION:
				{
					LOGSTATE(L"GOOD_CONNECTION");
					threadpool->execute(new ForwarderI(connection, anything, message->getDestinationJsonEncoding()));
					state = HANDLED;
					break;
				}
			case BAD_CONNECTION:
				{
					logger->log(Level::WARNING, L"BAD_CONNECTION");
					if (Message::DEST_PERSISTANT_QUEUE == message->getDestinationType())
					{
						connection = reactor->getConnectionByRemoteIdentifier(message->getReplyToRemoteID());
						state = !connection ? BAD_SOURCE_CONNECTION : GOOD_SOURCE_CONNECTION;
					}
					else
					{
					    state = DROPPED;
					}
					break;
				}                
			case GOOD_SOURCE_CONNECTION: 
				{
					LOGSTATE(L"GOOD_SOURCE_CONNECTION");
					Message reply = new MessageI();
					reply->setDestinationName(message->getReplyToName());
					reply->setDestinationType(message->getReplyToType());
					reply->setDestinationRemoteID(message->getReplyToRemoteID());
					reply->setDestinationJsonEncoding(message->getReplyToJsonEncoding());
					reply->setErrorCode(jmstools::implementation::reactor::Message::FAILURE);
					reply->setErrorMessage(L"JMSProvider: Cannot forward message. No connection to a MessageConsumer available for : " + message->getDestinationName());
					threadpool->execute(new ForwarderI(connection, reply->convertToAnything(), reply->getDestinationJsonEncoding()));
					state = HANDLED;
					break;
				}
			case BAD_SOURCE_CONNECTION:
				{
					logger->log(Level::WARNING, L"BAD_SOURCE_CONNECTION");	 
					state = DROPPED;
					break;
				}
			case HANDLED:
				{
					++handled;
					if (0 == handled % 100) logger->log(Level::SEVERE, L"handled : " + String(handled) + L" + dropped : " + String(dropped));
					state = POLLING;
					break;
				}
			case DROPPED:
				{
					++dropped;
					logger->log(Level::SEVERE, L"handled : " + String(handled) + L"   dropped : " + String(dropped) + L" +");
					state = POLLING;
					break;
				}
			case PERSISTANT_TOPIC_RECEIVED: // not yet handled
			case TEMPORARY_TOPIC_RECEIVED:
				{
					state = POLLING;
					break;
				}
			case FINAL:
			case ABORTED:
			default:
				{
					LOGSTATE(L"ABORTED");
					aborted = true;
					break;
				}
			}
		}
		catch (const Exception& e)
		{
			logger->log(Level::SEVERE, L"DispatcherI::dispatch() : " + e->toString());
			state = DROPPED;
		}
		catch (...)
		{
			logger->log(Level::SEVERE, L"DispatcherI::dispatch() : Unknown exception");
			state = DROPPED;
		}
	}

	dispatchingstopped->put(-1);
}

//********************************************************************

int DispatcherI::determineType(int type)
{
	switch(type)
	{
	case jmstools::implementation::reactor::Message::DEST_CONTROL:
		{
			return CONTROL_RECEIVED;
		}
	case jmstools::implementation::reactor::Message::DEST_INTERNAL:
		{
			return INTERNAL_RECEIVED;
		}
	case jmstools::implementation::reactor::Message::DEST_PERSISTANT_QUEUE:
		{
			return PERSISTANT_QUEUE_RECEIVED;
		}
	case jmstools::implementation::reactor::Message::DEST_TEMPORARY_QUEUE:
		{
			return TEMPORARY_QUEUE_RECEIVED;
		} 
	case jmstools::implementation::reactor::Message::DEST_PERSISTANT_TOPIC:
		{
			return PERSISTANT_TOPIC_RECEIVED;
		}
	case jmstools::implementation::reactor::Message::DEST_TEMPORARY_TOPIC:
		{
			return TEMPORARY_TOPIC_RECEIVED;
		}
	case jmstools::implementation::reactor::Message::DEST_NOT_DETERMINED:
	default:
		{
			logger->log(Level::SEVERE, L"Failed to determine type: " + String(type));
			return DROPPED;
		}
	}
}

//********************************************************************

void DispatcherI::run() 
{
	dispatch();
}

//********************************************************************

void DispatcherI::startDispatching() throw (jlj::lang::Exception)
{
	threadpool->execute(this);
}

//********************************************************************

void DispatcherI::stopDispatching(int timeout)
{
	reactor->receiveQueue()->put(new NullAnythingI());
	dispatchingstopped->poll(dispatchertimeout);
	reactor.reset(0);
}

//********************************************************************

void DispatcherI::runReactor() 
{
	reactor->run();
}

//********************************************************************

void DispatcherI::stopReactor() 
{
	logger->log(Level::SEVERE, L"stopReactor(): CTRL-C detected ...");
	reactor->initializeShutdown(0);
}

//********************************************************************

NAMESPACE_END(jmstools)

//********************************************************************

#endif

// eof
