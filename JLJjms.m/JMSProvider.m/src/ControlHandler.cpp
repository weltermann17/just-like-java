// ControlHandler.cpp

//********************************************************************

#ifndef jmstools_jmsprovider_ControlHandler_cpp
#define jmstools_jmsprovider_ControlHandler_cpp

//********************************************************************

#include <jlj/lang/Thread.h>
#include <ControlHandler.h>
#include <jlj/lang/String.h>
#include <jlj/util/StringAnything.h>
#include <jlj/util/MapAnything.h>
#include <jlj/util/IntAnything.h>
#include <jlj/util/logging/Logger.h>
#include <jlj/util/logging/ConsoleHandler.h>
#include <jmstools/implementation/reactor/Message.h>
#include <jmstools/implementation/reactor/AnythingMessage.h>
#include <jmstools/implementation/reactor/TextMessage.h>
#include <jlj/net/InetAddress.h>
#include <jlj/io/FileInputStream.h>
#include <jlj/io/FileOutputStream.h>

using namespace jlj::io;
using namespace jlj::util::logging;
using namespace jmstools::implementation::reactor;

//********************************************************************

NAMESPACE_BEGIN(jmstools)

//********************************************************************

int ControlHandlerI::oldhandled = 0;
int ControlHandlerI::olddropped = 0;
int ControlHandlerI::oldtotal = 0;

//********************************************************************

ControlHandlerI::~ControlHandlerI()
{}

//********************************************************************

ControlHandlerI::ControlHandlerI(ExecutorService<int> threadpool,
								 Reactor reactor, 
								 Consumers consumers,
								 Connection connection, 
								 Message message, 
								 int dropped, 
								 int handled)
								 : message(message)
								 , consumers(consumers)
								 , reactor(reactor)
								 , threadpool(threadpool)
								 , connection(connection)
								 , dropped(dropped)
								 , handled(handled)
{}

//********************************************************************

void ControlHandlerI::handleMonitor(bool nagios) throw (Exception)
{
	AnythingMessage messageback = new AnythingMessageI();
	messageback->setDestinationName(message->getReplyToName());
	messageback->setDestinationType(message->getReplyToType());
	messageback->setDestinationJsonEncoding(message->getReplyToJsonEncoding());

	Anything body = new MapAnythingI();
	String ip = InetAddress::getLocalHost()->getHostAddress();

	int divide = dropped + handled - oldtotal ;
	divide  = divide == 0 ? 1 : divide; 
	int percentdropped = (100 * (dropped - olddropped)) / divide;
	String message = L"OK - " + ip + L": "+
		L"totalhandled " + String(handled) + L"m, " +
		L"totaldropped " + String(dropped) + L"m, " +
		L"lasthandled " + String(handled - oldhandled) + L"m, " +
		L"lastdropped " + String(dropped - olddropped) + L"m, " +
		L"consumers " + String(consumers->size());			        
	if (nagios)
	{
		message = message + L" | " +
		L"totalhandled=" + String(handled) + L"c;;;; " +
		L"totaldropped=" + String(dropped) + L"c;;;; " +
		L"lasthandled=" + String(handled - oldhandled) + L"c;;;; " +
		L"lastdropped=" + String(dropped - olddropped) + L"c;;;; " +
		L"consumers=" + String(consumers->size());			        
	}
	
	body->put(L"MESSAGE",new StringAnythingI(message));			        

	olddropped = dropped;    
	oldhandled = handled;
	oldtotal = dropped + handled;

	body->put(L"ERRORCODE", new IntAnythingI(Message::SUCCESS));

	messageback->setBody(body);
	connection->send(messageback->convertToAnything(), messageback->getDestinationJsonEncoding());
}

//********************************************************************

void ControlHandlerI::handleShutdown() throw (Exception)
{
	AnythingMessage messageback = new AnythingMessageI();
	messageback->setDestinationName(message->getReplyToName());
	messageback->setDestinationType(message->getReplyToType());
	messageback->setDestinationJsonEncoding(message->getReplyToJsonEncoding());

	Anything body = new MapAnythingI();
	body->put(L"ERRORCODE", new IntAnythingI(Message::SUCCESS));
	messageback->setBody(body);

	reactor->setTimeout(200);
	connection->send(messageback->convertToAnything(), messageback->getDestinationJsonEncoding());
	Thread::sleep(500);

	consumers->cleanup(reactor, threadpool);
	reactor->initializeShutdown(15*1000); 
}

//********************************************************************

void ControlHandlerI::handle() throw (Exception)
{
	Logger logger = Logger::getLogger(jlj::jms::JMS_LOGGER_NAME);
	try
	{
		logger->log(Level::INFO, L"ControlHandlerI::handle() :");	

		switch(message->getCommand())
		{
		case jmstools::implementation::reactor::Message::COMMAND_MONITOR:
			{
				handleMonitor(false);
				break;
			}
			case jmstools::implementation::reactor::Message::COMMAND_MONITOR_FORNAGIOS:
			{
				handleMonitor(true);
				break;
			}
		case jmstools::implementation::reactor::Message::COMMAND_SHUTDOWN:
			{
				handleShutdown();
				break;
			}
		case jmstools::implementation::reactor::Message::COMMAND_SETGLOBALLOGLEVEL:
			{
				logger->log(Level::INFO, L"ControlHandlerI::handle() : COMMAND_SETGLOBALLOGLEVEL " + message->getCommandParameter(0)->toString());				
				logger->setLevel(Level::parse(message->getCommandParameter(0)));
				break;
			}
		case jmstools::implementation::reactor::Message::COMMAND_SETREACTORLOGLEVEL:
			{
				logger->log(Level::INFO, L"ControlHandlerI::handle() : COMMAND_SETREACTORLOGLEVEL " + message->getCommandParameter(0)->toString());	
				Logger::getLogger(jlj::nio::reactor::Reactor::REACTORS_LOGGER_NAME)->setLevel(Level::parse(message->getCommandParameter(0)));
				break;
			}
		case jmstools::implementation::reactor::Message::COMMAND_SETLOGLEVEL:
			{
				logger->log(Level::INFO, L"ControlHandlerI::handle() : COMMAND_SETLOGLEVEL " + message->getCommandParameter(0)->toString());	
				Logger::getLogger(jlj::nio::reactor::Reactor::REACTORS_LOGGER_NAME)->setLevel(Level::parse(message->getCommandParameter(0)));
				logger->setLevel(Level::parse(message->getCommandParameter(0)));
				break;
			} 
		case jmstools::implementation::reactor::Message::COMMAND_SETCONSOLEHANDLERLOGLEVEL:
			{
				logger->log(Level::INFO, 
					L"ControlHandlerI::handle() : COMMAND_SETCONSOLEHANDLERLOGLEVEL " + message->getCommandParameter(0)->toString());	

				logger->addHandler(
					new StreamHandlerI(new FileOutputStreamI(message->getCommandParameter(0))));

				break;
			}
		case jmstools::implementation::reactor::Message::COMMAND_NOT_DETERMINED:
		default:
			{
				logger->log(Level::SEVERE, L"ControlHandlerI::handle() cannot handle the command");
			}
		}	
	}
	catch (const Exception& e)
	{
		logger->log(Level::SEVERE, L"ControlHandlerI::handle() : " + e->toString());
	}
}

//********************************************************************

NAMESPACE_END(jmstools)


#endif

// eof